//
//  convolution.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#ifndef convolution_h
#define convolution_h

#include "image.hpp"

namespace filter {
    
    
    inline int bound_cast(int idx, int upper_bound){
        if( idx < 0 ){ return -idx; }
        else if( idx >= upper_bound ){ return upper_bound - (idx - upper_bound) - 1; }
        return idx;
    }
    
    template<typename kernel>
    void convolution( const img::image & input, img::image & output, const kernel & K ){
        
        int w = input.width(), h = input.height();
        
        if( input.width() != output.width() || input.height() != output.height() ){ output.setDims(w,h); }
        
        // loop through all pixels and perform
        // convolution computation
        for(int i = 0; i < w; ++i){
            for(int j = 0; j < h; ++j){
                
                // loop through neighborhood of pixel (i,j) and perform
                // convolution for this pixel based on the kernel
                int u1 = i - K.radius(), u2 = i + K.radius();
                int v1 = j - K.radius(), v2 = j + K.radius();
                double weight = 0.0, tot_weight = 0.0, c = 0.0;
                img::pixel conv;
                
                for(int u = u1; u <= u2; u++){
                    int ut = bound_cast(u, w);
                    for( int v = v1; v <= v2; v++){
                        int vt = bound_cast(v, h);
                        
                        // get weight from kernel
                        weight = K(u-i,v-j);
                        
                        // process convolved pixel
                        conv += (input(ut,vt)*weight);
                        
                        // add weight to total weight
                        double y = weight - c;
                        double t = y + tot_weight;
                        c = (t - tot_weight) - y;
                        tot_weight = t;
                    }
                }
                
                // normalize the pixel
                if( K.doNormalize() ){
                    conv /= tot_weight;
                }
                conv.normalize();
                conv[3] = 1.0;
                
                // set pixel value to output image
                output(i,j) = conv;
                
            }// for loop j
        }// for loop i
        
    }
    
    
    
}


#endif /* convolution_h */
