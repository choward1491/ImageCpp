//
//  png_image.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//
//  The MIT License (MIT)
//    Copyright © 2016 Christian Howard. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//

#ifndef image_hpp
#define image_hpp

#include "image_pixel.hpp"
#include <vector>
#include <string>

namespace img {
    
    enum Depth {
        _8bit = 8, _16bit = 16
    };
    
    
    class image {
    public:
        
        image();
        image(int width, int height, pixel p = pixel() );
        image( const std::string & load_img );
        
        void setDims( int width, int height );
        int width() const;
        int height() const;
        
        pixel & operator()(int r, int c);
        const pixel & operator()(int r, int c) const;
        pixel & pixelAt(int r, int c);
        const pixel & pixelAt(int r, int c) const;
        
        void save( const std::string & filename, Depth d = _16bit ) const;
        void load( const std::string & filename );
        
        void savePNG( const std::string & filename, Depth d = _16bit ) const;
        void loadPNG( const std::string & filename );
        
        void saveJPEG( const std::string & filename, Depth d = _16bit ) const;
        void loadJPEG( const std::string & filename );
        
        void elem_mult( const image & img, image & result ) const;
        
        void convert2grayscale();
        
    private:
        std::vector<pixel> pixels;
        int w, h;
        
        void write_png_file( const char *filename, const Depth & depth ) const;
        void read_png_file( const char *filename);
        void write_jpeg_file( const char *filename, const Depth & depth ) const;
        void read_jpeg_file( const char *filename);
        
    };
    
}

#endif /* png_image_hpp */
