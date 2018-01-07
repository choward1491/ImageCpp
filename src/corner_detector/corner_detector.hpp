//
//  corner_detector.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/18/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#ifndef corner_detector_h
#define corner_detector_h

#include <vector>
#include "image.hpp"


namespace detector {
    
    void corners( const img::image & orig,
                 std::vector<int> & x_out, std::vector<int> & y_out,
                 int max_pts = 500 );
    
}

#endif /* corner_detector_h */
