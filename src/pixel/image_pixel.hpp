//
//  image_pixel.hpp
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

#ifndef image_pixel_hpp
#define image_pixel_hpp


namespace img {
    
    
    class pixel {
    public:
        typedef double num_type;
        
        pixel();
        pixel(num_type r, num_type g, num_type b, num_type a = 1.0 );
        
        void setRed( num_type r);
        void setGreen( num_type g );
        void setBlue( num_type b );
        void setAlpha( num_type a );
        void normalize();
        void toGray();
        
        num_type & r();
        num_type & g();
        num_type & b();
        num_type & a();
        
        const num_type & r() const;
        const num_type & g() const;
        const num_type & b() const;
        const num_type & a() const;
        
        static pixel red();
        static pixel green();
        static pixel blue();
        static pixel cyan();
        static pixel white();
        static pixel black();
        static pixel clear();
        
        num_type & operator[](int idx);
        num_type operator[](int idx) const;
        pixel operator*( num_type scalar ) const;
        pixel operator*( const pixel & p ) const;
        void operator*=( num_type scalar );
        void operator/=( num_type scalar );
        void operator+=( const pixel & p );
        void operator-=( const pixel & p );
        
    private:
        num_type color[4];
    };
    
}

#endif /* image_pixel_hpp */
