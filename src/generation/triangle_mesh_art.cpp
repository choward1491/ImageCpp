//
//  triangle_mesh_art.cpp
//  image_proc_gen
//
//  Created by Christian Howard on 2/20/20.
//  Copyright © 2020 Christian Howard. All rights reserved.
//

#include <queue>
#include "triangle_mesh_art.hpp"

namespace art {
    
    void fillTriangleInImage( const Triangle<double> & tri,
                             img::image & new_img,
                             std::vector<bool> & hit_list)
    {
        int w = new_img.width(), h = new_img.height();
        std::queue<std::pair<int,int>> pixel_q;
        //std::vector<bool> add_list(w*h,false);
        
        // push triangle vertices and centroid
        int x_avg = 0, y_avg = 0;
        for(int i = 0; i < 3; ++i){
            int tx = (w-1)*tri.xv[i], ty = (h-1)*tri.yv[i];
            x_avg += tx;
            y_avg += ty;
            pixel_q.push( std::pair<int,int>(tx, ty) );
            //add_list[tri.xv[i] + tri.yv[i]*w] = true;
        }
        x_avg /= 3;
        y_avg /= 3;
        pixel_q.push( std::pair<int,int>(x_avg, y_avg) );
        //add_list[x_avg + y_avg*w] = true;
        
        // loop through a fill of the triangle and find the average color
        while( pixel_q.size() != 0 ){
            std::pair<int,int> coord = pixel_q.front(); pixel_q.pop();
            
            if( hit_list[coord.first + coord.second*w] ){ continue; }
            
            // process triangle
            double xc = static_cast<double>(coord.first)/w;
            double yc = static_cast<double>(coord.second)/h;
            if( tri.pointWithinTriangle2(xc, yc) ){
                
                double color_t[3] = {0};
                tri.color_interp(coord.first, coord.second, color_t);
                auto& pixel = new_img(coord.first, coord.second);
                pixel.a() = 1.0;
                pixel.r() = color_t[0];
                pixel.g() = color_t[1];
                pixel.b() = color_t[2];
                hit_list[coord.first + coord.second*w] = true;
                
                const int r = 1;
                for(int i = -r; i <= r; ++i){
                    for(int j = -r; j <= r; ++j){
                        int xn = coord.first + i;
                        int yn = coord.second + j;
                        if( (xn < w && xn >= 0 && yn < h && yn >= 0 ) && !hit_list[xn + yn*w] ) {
                            pixel_q.push( std::pair<int,int>(xn, yn) );
                            //add_list[xn + yn*w] = true;
                        }
                    }
                }
            }// end if tri
        }// end while
        
    }

    // method to set traits about the triangle art instance
    void triangle_art::set_dimensions(int width, int height) {
        w = width; h = height;
    }
    void triangle_art::set_grid_props(int num_cols, int num_rows) {
        ncols = num_cols; nrows = num_rows;
    }
    
    // methods to build the art
    void triangle_art::construct_img( img::image& I,
                                     std::function<double(double,double)> function,
                                     std::function<void(Triangle<>&)> colorer)
    {
        // resize the image based on dimensions
        I.setDims(w, h);
        
        // compute number of points and triangles being used
        int num_pts  = (ncols+1)*(nrows+1);
        int num_tris = 2*ncols*nrows;
        
        // assume domain in [0,1] X [0,1] grid. find the stepsizes
        double dx = 1.0 / ncols, dy = 1.0 / nrows;
        
        // specify coordinates for triangles being made
        Triangle<> tri;
        std::vector<double> x(num_pts), y(num_pts), z(num_pts);
        std::vector<bool> hit_list(w*h,false);
        
        // loop to construct the points
        for(int i = 0; i <= ncols; ++i){
            for(int j=0; j<= nrows; ++j){
                int k = i + j*(ncols+1);
                x[k] = i*dx;
                y[k] = j*dy;
                z[k] = function(x[k], y[k]);
            }// end j
        }// end i
        
        // loop to construct the triangles
        for(int i=0; i < ncols; ++i){
            for(int j=0; j < nrows; ++j){
                int itl = i + j*(ncols+1);
                int itr = (i + 1) + j*(ncols+1);
                int ibl = i + (j+1)*(ncols+1);
                int ibr = (i+1) + (j+1)*(ncols+1);
                
                int ku= 2*(i + (j-1)*ncols);
                int kl= 2*((i-1) + j*ncols);
                int kd= 2*(i + (j+1)*ncols);
                int k = 2*(i + j*ncols);
                
                tri.id = k;
                tri.vertices[0] = itl;
                tri.vertices[2] = ibr;
                tri.vertices[1] = itr;
                tri.neighbors[0] = k+1;
                tri.neighbors[1] = k+2;
                tri.neighbors[2] = ku+1;
                tri.makeNormal(x.data(), y.data(), z.data(), num_pts);
                colorer(tri);
                fillTriangleInImage(tri, I, hit_list);
                
                tri.id = k+1;
                tri.vertices[0] = itl;
                tri.vertices[2] = ibl;
                tri.vertices[1] = ibr;
                tri.neighbors[0] = k;
                tri.neighbors[1] = kl;
                tri.neighbors[2] = kd;
                tri.makeNormal(x.data(), y.data(), z.data(), num_pts);
                colorer(tri);
                fillTriangleInImage(tri, I, hit_list);
                
            }// end j
        }// end i
        
    }

}
