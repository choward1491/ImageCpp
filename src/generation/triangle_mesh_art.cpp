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
        double t = std::min(w,h);
        
        // compute number of points and triangles being used
        int num_pts  = (ncols+1)*(nrows+1);
        int num_tris = 2*ncols*nrows;
        
        // assume domain in [0,1] X [0,1] grid. find the stepsizes
        double dx = 2.0 / ncols, dy = 2.0 / nrows;
        int dxi = w / ncols, dyi = h / nrows;
        
        // specify coordinates for triangles being made
        Triangle<> tri;
        std::vector<double> x(num_pts), y(num_pts), z(num_pts);
        std::vector<bool> hit_list(w*h,false);
        
        // loop to construct the points
        for(int i = 0; i <= ncols; ++i){
            for(int j=0; j<= nrows; ++j){
                int k = i + j*(ncols+1);
                x[k] = (-1.0 + i*dx)*(w/t);
                y[k] = (-1.0 + j*dy)*(h/t);
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
                
                {
                    int xi[3] = {i*dxi, (i+1)*dxi, (i+1)*dxi};
                    int yi[3] = {j*dyi, j*dyi, (j+1)*dyi};
                    tri.id = k;
                    tri.vertices[0] = itl;
                    tri.vertices[2] = ibr;
                    tri.vertices[1] = itr;
                    tri.neighbors[0] = k+1;
                    tri.neighbors[1] = k+2;
                    tri.neighbors[2] = ku+1;
                    tri.set_int_coords(xi, yi, w, h);
                    tri.makeNormal(z.data(), num_pts);
                    colorer(tri);
                    tri.fill_in_image(I);
                }
                
                {
                    int xi[3] = {i*dxi, (i+1)*dxi, i*dxi};
                    int yi[3] = {j*dyi, (j+1)*dyi, (j+1)*dyi};
                    tri.id = k+1;
                    tri.vertices[0] = itl;
                    tri.vertices[2] = ibl;
                    tri.vertices[1] = ibr;
                    tri.neighbors[0] = k;
                    tri.neighbors[1] = kl;
                    tri.neighbors[2] = kd;
                    tri.set_int_coords(xi, yi, w, h);
                    tri.makeNormal(z.data(), num_pts);
                    colorer(tri);
                    tri.fill_in_image(I);
                }
                
                
            }// end j
        }// end i
        
    }

}
