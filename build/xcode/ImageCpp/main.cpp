//
//  main.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include <iostream>
#include "image.hpp"
#include "triangulation_filter.hpp"
//#include "convex_hull3D.h"
#include "convex_hull3d.hpp"
#include "delaunay_triangulation.hpp"

int main(int argc, const char * argv[]) {
    
    //img::image pic("/Users/cjh/Downloads/logo.png");
    //img::image pic("/Users/cjh/Downloads/me_test.png");
    img::image pic("/Users/cjh/Downloads/paint_smear.png");
    //img::image pic(500,500,img::pixel::red() );
    img::image tri_filter;
    filter::triangulation(pic, tri_filter, 5000);
    tri_filter.savePNG("/Users/cjh/Documents/test_tri_pic.png", img::_16bit);
    
    /*std::default_random_engine generator(17);
    std::uniform_int_distribution<unsigned int> distribution(0,UINT32_MAX);
    unsigned int N = 500;
    
    //std::vector<double> x{0, 1, 1, 0, 0.5}, y{ 0, 0, 1, 1, 0.5};
    std::vector<double> x, y;
    std::vector<Triangle<double>> triangles;
    for(unsigned int i = 0; i < N; ++i){
        x.push_back(static_cast<double>(distribution(generator)%10000)/1e4);
        y.push_back(static_cast<double>(distribution(generator)%10000)/1e4);
    }
    delaunay::triangulation(x, y, triangles);
    
    printf("Print Results:\n");
    for(int i = 0; i < triangles.size(); ++i){
        triangles[i].print(&x[0], &y[0]);
    }
    
    // save results
    */
    
    return 0;
}
