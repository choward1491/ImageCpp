//
//  image_pixel.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include "image_pixel.hpp"
#include <cmath>


namespace img {
    
    enum color {
        red_ = 0,green_,blue_,alpha_
    };
    
    pixel::pixel() {
        color[red_] = color[green_] = color[blue_] = color[alpha_] = 0.0;
    }
    pixel::pixel(num_type r, num_type g, num_type b, num_type a ) {
        setRed(r);
        setGreen(g);
        setBlue(b);
        setAlpha(a);
    }
    void pixel::setRed( num_type r) {
        color[red_] = r;
    }
    void pixel::setGreen( num_type g ) {
        color[green_] = g;
    }
    void pixel::setBlue( num_type b ) {
        color[blue_] = b;
    }
    void pixel::setAlpha( num_type a ) {
        color[alpha_] = a;
    }
    void pixel::normalize() {
        for(int i = 0; i < 4; ++i){
            if( color[i] > 1.0 ){ color[i] = 1.0; }
            else if( color[i] < 0.0 ){ color[i] = 0.0; }
        }
    }
    void pixel::toGray(){
        for(int k = 0; k < 3; ++k){
            if( color[k] < 0.04045 ){
                color[k] /= 12.92;
            }else{
                color[k] = std::pow((color[k]+0.055)/1.055,2.4);
            }
        }
        double Y = 0.2126*r() + 0.7152*g() + 0.0722*b();
        //double Y = ( r() + g() + b() )/3.0;
        if( Y <= 0.0031308 ){
            Y *= 12.92;
        }else{
            Y = 1.055*std::pow(Y,1.0/2.4) - 0.055;
        }
        for(int k = 0; k < 3; ++k){ color[k] = Y; }
        color[alpha_] = 1.0;
    }
    
    typename pixel::num_type & pixel::r() {
        return color[red_];
    }
    typename pixel::num_type & pixel::g() {
        return color[green_];
    }
    typename pixel::num_type & pixel::b() {
        return color[blue_];
    }
    typename pixel::num_type & pixel::a() {
        return color[alpha_];
    }
    const typename pixel::num_type & pixel::r() const {
        return color[red_];
    }
    const typename pixel::num_type & pixel::g() const {
        return color[green_];
    }
    const typename pixel::num_type & pixel::b() const {
        return color[blue_];
    }
    const typename pixel::num_type & pixel::a() const {
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
    
    typename pixel::num_type & pixel::operator[](int idx) {
        return color[idx];
    }
    typename pixel::num_type pixel::operator[](int idx) const {
        return color[idx];
    }
    
    pixel pixel::operator*( num_type scalar ) const {
        pixel p;
        for(int i = 0; i < 4; ++i){ p[i] = scalar*color[i]; }
        return p;
    }
    
    pixel pixel::operator*( const pixel & pr ) const {
        pixel p;
        for(int i = 0; i < 4; ++i){ p[i] = pr[i]*color[i]; }
        return p;
    }
    
    void pixel::operator*=( num_type scalar ) {
        for(int i = 0; i < 4; ++i){ color[i] *= scalar; }
    }
    void pixel::operator/=( num_type scalar ) {
        for(int i = 0; i < 4; ++i){ color[i] /= scalar; }
    }
    void pixel::operator+=( const pixel & p ) {
        for(int i = 0; i < 4; ++i){ color[i] += p[i]; }
    }
    void pixel::operator-=( const pixel & p ) {
        for(int i = 0; i < 4; ++i){ color[i] -= p[i]; }
    }
    //num_type color[4];
    
}
