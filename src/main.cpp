//
//  main.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include "image.hpp"
#include "triangulation_filter.hpp"
#include "generation/triangle_mesh_art.hpp"
#include <random>

double gaussian(double x, double y){
    double dx = (x-0.5), dy = (y - 0.5);
    //double out = std::exp( -2*(dx*dx + dy*dy));
    double out = std::sin( 20*sqrt(dx*dx + dy*dy) );
    return out;
}

void test_colorer(Triangle<>& tri){
    static std::uniform_real_distribution<double> U(0,1);
    static std::default_random_engine g;
    
    double n[] = {0, 0, -1};
    double s = 0.0;
    for(int i =0 ; i < 3; ++i){
        s += tri.normal[i]*n[i];
    }
    s = 0.5*(s + 1);
    //s = tri.xv[0];
    s = U(g);
    double color[3] = {0};
    double red[3] = {1.,0.3,0.3};
    double white[3] = {0.7,0.0,0.0};
    double purple[3] = {0.7,0,1.0};
    double purple2[3]= {0.5,0,0.8};
    
    if( s <= 0.9 ){
        s /= 0.9;
        for(int i = 0; i < 3; ++i){
            color[i] = (1-s)*white[i] + s*red[i];
            for(int j = 0; j < 3; ++j){
                tri.colors[i][j] = color[i];
            }
        }
    }else{
        s = (s - 0.9)*10;
        for(int i = 0; i < 3; ++i){
            color[i] = (1-s)*purple[i] + s*purple2[i];
            for(int j = 0; j < 3; ++j){
                tri.colors[i][j] = color[i];
            }
        }
    }
    
}


int main(int argc, const char * argv[]) {
    
    img::image pic;
    art::triangle_art art_test;
    art_test.set_dimensions(800, 300);
    art_test.set_grid_props(10, 5);
    art_test.construct_img(pic, gaussian, test_colorer);
    pic.savePNG("/Users/cjh/Documents/abstract1_new.png", img::_16bit);
    
    return 0;
}
