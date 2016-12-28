//
//  main.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include <iostream>
#include "image.hpp"

int main(int argc, const char * argv[]) {
    
    //img::image pic("/Users/christianjhoward/picTest1_n4.png");
    img::image pic(500,500,img::pixel::clear() );
    pic.savePNG("/Users/christianjhoward/test_clear.png", img::_16bit);
    
    return 0;
}
