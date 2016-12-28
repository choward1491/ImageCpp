//
//  image_pixel.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include "image_pixel.hpp"


namespace img {
    
    enum color {
        red_ = 0,green_,blue_,alpha_
    };
    
    pixel::pixel() {
        color[red_] = color[green_] = color[blue_] = color[alpha_] = 0.0;
    }
    pixel::pixel(float r, float g, float b, float a ) {
        setRed(r);
        setGreen(g);
        setBlue(b);
        setAlpha(a);
    }
    void pixel::setRed( float r) {
        color[red_] = r;
    }
    void pixel::setGreen( float g ) {
        color[green_] = g;
    }
    void pixel::setBlue( float b ) {
        color[blue_] = b;
    }
    void pixel::setAlpha( float a ) {
        color[alpha_] = a;
    }
    float & pixel::r() {
        return color[red_];
    }
    float & pixel::g() {
        return color[green_];
    }
    float & pixel::b() {
        return color[blue_];
    }
    float & pixel::a() {
        return color[alpha_];
    }
    const float & pixel::r() const {
        return color[red_];
    }
    const float & pixel::g() const {
        return color[green_];
    }
    const float & pixel::b() const {
        return color[blue_];
    }
    const float & pixel::a() const {
        return color[alpha_];
    }

    pixel pixel::red() {
        return pixel(1.0,0,0);
    }
    pixel pixel::green() {
        return pixel(0,1.0,0);
    }
    pixel pixel::blue() {
        return pixel(0,0,1.0);
    }
    pixel pixel::cyan() {
        return pixel(0,1.0,1.0);
    }
    pixel pixel::white() {
        return pixel(1.0,1.0,1.0);
    }
    pixel pixel::black() {
        return pixel(0,0,0);
    }
    pixel pixel::clear() {
        return pixel(1,1,1,0);
    }
    
    //float color[4];
    
}
