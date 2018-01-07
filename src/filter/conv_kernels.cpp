//
//  conv_kernels.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#include "conv_kernels.hpp"
#include <cmath>


namespace filter {
    
    // define gaussian kernel
    double gaussian::operator()( int u, int v ) const {
        double dx2 = static_cast<double>(u*u + v*v);
        return std::exp(-dx2/(radius_*radius_));
    }
    int gaussian::radius() const {
        return static_cast<int>(radius_);
    }
    void gaussian::setRadius(double r){
        radius_ = r;
    }
    
    bool gaussian::doNormalize() const {
        return true;
    }
    
    // define x derivative kernel
    double xderiv::operator()( int u, int v ) const {
        return static_cast<double>(u);
    }
    int xderiv::radius() const {
        return 1;
    }
    bool xderiv::doNormalize() const {
        return false;
    }
    
    
    // define y derivative kernel
    double yderiv::operator()( int u, int v ) const {
        return static_cast<double>(v);
    }
    int yderiv::radius() const {
        return 1;
    }
    bool yderiv::doNormalize() const {
        return false;
    }
    
}
