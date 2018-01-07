//
//  convex_hull3d.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/21/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#include "convex_hull3d.hpp"
namespace _3D {
    bool isTetrahedronVertices( const unsigned int *tetra0, unsigned int idx ){
        bool isVertex = false;
        for(int i = 0; i < 4; ++i){
            isVertex = isVertex || (tetra0[i] == idx);
        }
        return isVertex;
    }
}
