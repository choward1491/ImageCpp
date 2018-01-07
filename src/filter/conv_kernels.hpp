//
//  conv_kernels.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#ifndef conv_kernels_h
#define conv_kernels_h

namespace filter {
    
    class gaussian {
    public:
        double operator()( int u, int v ) const;
        int radius() const;
        void setRadius(double r);
        bool doNormalize() const;
    private:
        double radius_;
    };
    
    class xderiv {
    public:
        double operator()( int u, int v ) const;
        int radius() const;
        bool doNormalize() const;
    };
    
    class yderiv {
    public:
        double operator()( int u, int v ) const;
        int radius() const;
        bool doNormalize() const;
    };
    
}

#endif /* conv_kernels_h */
