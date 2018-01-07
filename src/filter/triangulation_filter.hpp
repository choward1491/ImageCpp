//
//  triangulation_filter.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#ifndef triangulation_filter_h
#define triangulation_filter_h

#include "image.hpp"

namespace filter {
    void triangulation( const img::image & orig_img, img::image & new_img, int num_pts_use = 500 );
}


#endif /* triangulation_filter_h */
