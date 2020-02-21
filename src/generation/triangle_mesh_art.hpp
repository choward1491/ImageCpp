//
//  triangle_mesh_art.hpp
//  image_proc_gen
//
//  Created by Christian Howard on 2/20/20.
//  Copyright © 2020 Christian Howard. All rights reserved.
//

#ifndef triangle_mesh_art_hpp
#define triangle_mesh_art_hpp

#include "../image/image.hpp"
#include "../utility/triangle.hpp"

namespace art {

class triangle_art {
public:
    
    // ctor/dtor
    triangle_art() = default;
    ~triangle_art() = default;
    
    // method to set traits about the triangle art instance
    void set_dimensions(int width, int height);
    void set_grid_props(int num_cols, int num_rows);
    
    // methods to build the art
    void construct_img( img::image& I,
                       std::function<double(double,double)> func,
                       std::function<void(Triangle<>&)> colorer);
    
private:
    
    int w, h;
    int ncols, nrows;
    
};

}

#endif /* triangle_mesh_art_hpp */
