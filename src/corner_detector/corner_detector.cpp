//
//  corner_detector.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#include "corner_detector.hpp"
#include "conv_kernels.hpp"
#include "convolution.hpp"
#include <queue>
#include <algorithm>


typedef std::pair<int, int>         idx_pair;
typedef std::pair<double,idx_pair>  corner_pair;


namespace std {
    
    template<>
    struct less< corner_pair > {
        
        bool operator()(const corner_pair &lhs,
                        const corner_pair &rhs) const
        {
            return lhs.first > rhs.first;
        }
        
    };
}

namespace detector {
    
    
    
    
    // Noble's corner measure
    inline double corner_coef( double ixx, double iyy, double ixy ){
        const double eps = 1e-8;
        const double trace = ixx + iyy;
        const double det   = ixx*iyy - ixy*ixy;
        return 2.0 * det / ( trace + eps );
    }
    
    void corners( const img::image & orig, std::vector<int> & x, std::vector<int> & y, int max_pts ) {
        
        
        // init dimensions and images
        int w = orig.width(), h = orig.height();
        img::image Ixx(w,h), Iyy(w,h), Ixy(w,h), M(w,h);
        
        // init priority queue for keeping track of top best coordinates
        std::priority_queue<corner_pair> min_heap;
        
        // init kernels for convolution
        filter::xderiv xd;
        filter::yderiv yd;
        filter::gaussian gauss;
        gauss.setRadius(2);
        
        // compute gradient convolutions and element wise products
        filter::convolution(orig, Ixx, xd);
        filter::convolution(orig, Iyy, yd);
        
        Ixx.savePNG("/Users/cjh/Documents/derivx.png");
        Iyy.savePNG("/Users/cjh/Documents/derivy.png");
        
        Ixx.elem_mult(Iyy, Ixy);
        Ixx.elem_mult(Ixx, Ixx);
        Iyy.elem_mult(Iyy, Iyy);
        
        Ixx.savePNG("/Users/cjh/Documents/derivxx.png");
        Ixy.savePNG("/Users/cjh/Documents/derivxy.png");
        Iyy.savePNG("/Users/cjh/Documents/derivyy.png");
        
        // compute gaussian convolutions on the convolved images
        filter::convolution(Ixx, Ixx, gauss);
        filter::convolution(Iyy, Iyy, gauss);
        filter::convolution(Ixy, Ixy, gauss);
        
        Ixx.savePNG("/Users/cjh/Documents/derivxx_s.png");
        Ixy.savePNG("/Users/cjh/Documents/derivxy_s.png");
        Iyy.savePNG("/Users/cjh/Documents/derivyy_s.png");
        
        // compute coordinates of best corner locations
        double Mp = 0.0, tmp = 0.0;
        for(int xx = 0; xx < w; ++xx ){
            for(int yy = 0; yy < h; ++yy){
                Mp = 0.0;
                for(int idx = 0; idx < 4; ++idx){
                    tmp = corner_coef( Ixx(xx,yy)[idx], Iyy(xx,yy)[idx], Ixy(xx,yy)[idx] );
                    Mp += tmp*tmp;
                }
                
                if( min_heap.size() < max_pts ){ min_heap.push( corner_pair(Mp, idx_pair(xx,yy)) ); }
                else {
                    double top = min_heap.top().first;
                    if( top < Mp ){
                        min_heap.pop();
                        min_heap.push( corner_pair(Mp, idx_pair(xx,yy)) );
                    }
                }// end if
                
            }// end for yy
        }// end for xx
        
        // loop through best coordinates and add them to std::vector outputs
        unsigned int num_pts = min_heap.size();
        x.resize(num_pts);
        y.resize(num_pts);
        
        img::image copy = orig;
        
        for(unsigned int k = 0; k < max_pts; ++k){
            const corner_pair & cp = min_heap.top();
            double cost = cp.first;
            x[k] = cp.second.first;
            y[k] = cp.second.second;
            copy(x[k],y[k]) = img::pixel::red();
            min_heap.pop();
        }// end for k
        
        copy.savePNG("/Users/cjh/Documents/orig_w_dots.png");
        
        
    }
    
}
