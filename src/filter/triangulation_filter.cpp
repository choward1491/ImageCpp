//
//  triangulation_filter.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#include "triangulation_filter.hpp"
#include "corner_detector.hpp"
//#include "convex_hull3D.h"
#include <stack>
#include <map>
#include <random>
#include "convex_hull3d.hpp"
#include "delaunay_triangulation.hpp"

namespace filter {
    
    
    void randEdgeCoordinates( int width, int height,
                             std::vector<int> & xpts, std::vector<int> & ypts,
                             int num_pts_per_edge )
    {
        std::default_random_engine generator(17);
        std::uniform_int_distribution<unsigned int> distribution(0,UINT32_MAX);
        
        // generate points for top edge
        for(int k = 0; k < num_pts_per_edge; ++k){
            xpts.push_back(distribution(generator)%width);
            ypts.push_back(0);
        }
        
        // generate points for bottom edge
        for(int k = 0; k < num_pts_per_edge; ++k){
            xpts.push_back(distribution(generator)%width);
            ypts.push_back(height-1);
        }
        
        // generate points for left edge
        for(int k = 0; k < num_pts_per_edge; ++k){
            xpts.push_back(0);
            ypts.push_back(distribution(generator)%height);
        }
        
        // generate points for right edge
        for(int k = 0; k < num_pts_per_edge; ++k){
            xpts.push_back(width-1);
            ypts.push_back(distribution(generator)%height);
        }
        
        // add corners
        xpts.push_back(0);
        xpts.push_back(0);
        xpts.push_back(width-1);
        xpts.push_back(width-1);
        
        ypts.push_back(0);
        ypts.push_back(height-1);
        ypts.push_back(height-1);
        ypts.push_back(0);
        
        
    }
    
    
    bool triangleIsValid( const Triangle<double> & tri,
                         const std::vector<int> & x, const std::vector<int> & y,
                         int width, int height )
    {
        bool isNotValid = true;
        
        for(int i = 0; i < 3; ++i){
            int x_ = x[tri.vertices[i]];
            int y_ = y[tri.vertices[i]];
            isNotValid = isNotValid && ( x_ == 0 || x_ == (width-1) || y_ == 0 || y_ == (height-1) );
        }
        
        return !isNotValid;
    }
    
    img::pixel getAverageColorWithinTriangle( const Triangle<double> & tri,
                                             const std::vector<int> & x, const std::vector<int> & y,
                                             const std::vector<double> & xf, const std::vector<double> & yf,
                                             const img::image & orig_img)
    {
        int w = orig_img.width(), h = orig_img.height();
        unsigned int size = 0;
        unsigned int num_pixels_in_tri = 0;
        img::pixel avg_color;
        std::stack<std::pair<int,int>> pixel_q;
        std::vector<bool> hit_list(w*h,false);
        //std::map<int, bool> hit_list;
        
        // push triangle vertices and centroid
        int x_avg = 0, y_avg = 0;
        for(int i = 0; i < 3; ++i){
            x_avg += x[tri.vertices[i]];
            y_avg += y[tri.vertices[i]];
            pixel_q.push( std::pair<int,int>(x[tri.vertices[i]], y[tri.vertices[i]]) );
            hit_list[x[tri.vertices[i]] + y[tri.vertices[i]]*w] = true;
        }
        x_avg /= 3;
        y_avg /= 3;
        pixel_q.push( std::pair<int,int>(x_avg, y_avg) );
        hit_list[x_avg + y_avg*w] = true;
        
        // loop through a fill of the triangle and find the average color
        while( pixel_q.size() != 0 ){
            std::pair<int,int> coord = pixel_q.top(); pixel_q.pop();
            hit_list[coord.first + coord.second*w] = true;
            
            // process triangle
            if( tri.pointWithinTriangle(coord.first, coord.second, &xf[0], &yf[0], xf.size()) ){
                
                avg_color += orig_img(coord.first, coord.second);
                num_pixels_in_tri++;
                
                // add neighbors that haven't been touched into the list
                int xdel[4] = {-1, 1, 0, 0};
                int ydel[4] = {0, 0, -1, 1};
                
                for(int i = 0; i < 4; ++i){
                    int xn = coord.first + xdel[i];
                    int yn = coord.second + ydel[i];
                    if( (xn < w && xn >= 0 && yn < h && yn >= 0 )
                        && !hit_list[xn + yn*w] )
                    {
                        pixel_q.push( std::pair<int,int>(xn, yn) );
                        hit_list[xn + yn*w] = true;
                    }
                }// end for i
            }// end if tri
            size = pixel_q.size();
        }// end while
        
        avg_color /= static_cast<double>(num_pixels_in_tri);
        avg_color.normalize();
        return avg_color;
    }
    
    
    void fillTriangleInImage( const Triangle<double> & tri,
                             const std::vector<int> & x, const std::vector<int> & y,
                             const std::vector<double> & xf, const std::vector<double> & yf,
                             const img::pixel & color,
                             img::image & new_img)
    {
        int w = new_img.width(), h = new_img.height();
        std::queue<std::pair<int,int>> pixel_q;
        unsigned int size = 0;
        
        std::vector<bool> hit_list(w*h,false);
        //std::map<int, bool> hit_list;
        
        // push triangle vertices and centroid
        int x_avg = 0, y_avg = 0;
        for(int i = 0; i < 3; ++i){
            x_avg += x[tri.vertices[i]];
            y_avg += y[tri.vertices[i]];
            pixel_q.push( std::pair<int,int>(x[tri.vertices[i]], y[tri.vertices[i]]) );
            hit_list[x[tri.vertices[i]] + y[tri.vertices[i]]*w] = true;
        }
        x_avg /= 3;
        y_avg /= 3;
        pixel_q.push( std::pair<int,int>(x_avg, y_avg) );
        hit_list[x_avg + y_avg*w] = true;
        
        // loop through a fill of the triangle and find the average color
        while( pixel_q.size() != 0 ){
            std::pair<int,int> coord = pixel_q.front(); pixel_q.pop();
            
            // process triangle
            if( tri.pointWithinTriangle(coord.first, coord.second, &xf[0], &yf[0], xf.size(), 1e-2) ){
                
                new_img(coord.first, coord.second) = color;
                
                // add neighbors that haven't been touched into the list
                int xdel[4] = {-1, 1, 0, 0};
                int ydel[4] = {0, 0, -1, 1};
                
                for(int i = 0; i < 4; ++i){
                    int xn = coord.first + xdel[i];
                    int yn = coord.second + ydel[i];
                    if( (xn < w && xn >= 0 && yn < h && yn >= 0 )
                       && !hit_list[xn + yn*w] )
                    {
                        pixel_q.push( std::pair<int,int>(xn, yn) );
                        hit_list[xn + yn*w] = true;
                    }
                }// end for i
            }// end if tri
        }// end while
        
        //new_img.savePNG("/Users/cjh/Documents/new_img_update.png");
        
    }
    
    
    void triangulation( const img::image & orig_img, img::image & new_img, int num_pts_use ) {
        
        // init dimensions and images
        int w = orig_img.width(), h = orig_img.height();
        new_img.setDims(w, h);
        std::vector<int> x,y;
        std::vector<double> xf, yf;
        std::vector<Triangle<double>> triangles;
        
        // find corner locations
        //detector::corners(orig_img, x, y, num_pts_use);
        std::default_random_engine generator(17);
        std::uniform_int_distribution<unsigned int> distribution(0,UINT32_MAX);
        for(int i = 0; i < num_pts_use; ++i){
            x.push_back(distribution(generator)%w);
            y.push_back(distribution(generator)%h);
        }
        
        // add random set of coordinates on the boundary of the image
        int num_edge_pts = num_pts_use / 20;
        randEdgeCoordinates(w, h, x, y, num_edge_pts);
        
        // triangulate the coordinates
        xf.resize(x.size()); yf.resize(y.size());
        for(int i = 0; i < xf.size(); ++i){
            xf[i] = static_cast<double>(x[i]);
            yf[i] = static_cast<double>(y[i]);
        }
        
        delaunay::triangulation(xf,yf,triangles);
        
        // loop through triangles, compute average color, and set the color within the image
        // within the triangle it is valid for
        for(unsigned int i = 0; i < triangles.size(); ++i){
            if( triangleIsValid(triangles[i], x, y, w, h) ){
                
                // find average color within triangle
                img::pixel avg_color = getAverageColorWithinTriangle( triangles[i],
                                                                      x, y, xf, yf,
                                                                      orig_img);
                
                // fill triangle in new image with average color
                fillTriangleInImage( triangles[i], x, y, xf, yf, avg_color, new_img);
                
            }// end if
            else{
                printf("yippie!\n");
            }
        }// end for i
        
        
    }
    
}
