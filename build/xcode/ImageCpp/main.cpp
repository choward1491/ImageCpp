//
//  main.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include "image.hpp"
#include "triangulation_filter.hpp"

int main(int argc, const char * argv[]) {
    
    img::image pic("/Users/cjh/Downloads/rose.png");
    img::image tri_filter;
    filter::triangulation(pic, tri_filter, 20000);
    tri_filter.savePNG("/Users/cjh/Documents/rose_new2.png", img::_16bit);
    
    return 0;
}
