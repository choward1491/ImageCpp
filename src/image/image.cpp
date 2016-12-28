//
//  png_image.cpp
//  ImageCpp
//
//  Created by Christian J Howard on 11/24/16.
//  Copyright © 2016 Christian Howard. All rights reserved.
//

#include "image.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>
#include <pngconf.h>
#include <stdint.h>

namespace img {
    
    image::image():h(0),w(0),pixels(0){
        
    }
    image::image(int rows_, int cols_, pixel p ):pixels(rows_*cols_,p){
        h = rows_; w = cols_;
    }
    
    image::image( const std::string & load_img ) {
        loadPNG(load_img);
    }
    
    void image::setDims( int rows_, int cols_ ){
        h = rows_; w = cols_;
        pixels.resize(h*w);
    }
    int image::height() const{
        return h;
    }
    int image::width() const{
        return w;
    }
    
    pixel & image::operator()(int r, int c) {
        return pixels[c + r*w];
    }
    const pixel & image::operator()(int r, int c) const {
        return pixels[c + r*w];
    }
    pixel & image::pixelAt(int r, int c) {
        return pixels[c + r*w];
    }
    const pixel & image::pixelAt(int r, int c) const {
        return pixels[c + r*w];
    }
    
    void image::savePNG( const std::string & filename, Depth d  ) const {
        write_png_file( filename.c_str(), d);
    }
    void image::loadPNG( const std::string & filename ) {
        read_png_file(filename.c_str());
    }
    
    
    void image::read_png_file( const char *filename) {
        png_byte color_type;
        png_byte bit_depth;
        png_bytep *row_pointers;
        FILE *fp = fopen(filename, "rb");
        
        png_structp image = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(!image) abort();
        
        png_infop info = png_create_info_struct(image);
        if(!info) abort();
        
        if(setjmp(png_jmpbuf(image))) abort();
        
        png_init_io(image, fp);
        
        png_read_info(image, info);
        
        w = png_get_image_width(image, info);
        h = png_get_image_height(image, info);
        color_type = png_get_color_type(image, info);
        bit_depth  = png_get_bit_depth(image, info);
        
        // Read any color_type into 8bit depth, RGBA format.
        // See http://www.libpng.org/pub/image/libpng-manual.txt
        
        //if(bit_depth == 16)
        //    png_set_strip_16(image);
        
        if(color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(image);
        
        // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
        if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            png_set_expand_gray_1_2_4_to_8(image);
        
        if(png_get_valid(image, info, PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(image);
        
        // These color_type don't have an alpha channel then fill it with 0xff.
        if(color_type == PNG_COLOR_TYPE_RGB ||
           color_type == PNG_COLOR_TYPE_GRAY ||
           color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(image, 0xFF, PNG_FILLER_AFTER);
        
        if(color_type == PNG_COLOR_TYPE_GRAY ||
           color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(image);
        
        png_read_update_info(image, info);
        
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * h);
        for(int y = 0; y < h; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(image,info));
        }
        
        png_read_image(image, row_pointers);
        int total = w*h;
        pixels.resize(total);
        int span = bit_depth/2, k;
        uint16_t max16 = 0xFFFF;
        float fmax16 = static_cast<float>(max16);
        
        if( bit_depth == 8 ){
            for(int y = 0; y < h; y++) {
                png_bytep row = row_pointers[y];
                for(int x = 0; x < w; x++) {
                    png_bytep px = &(row[x * span]);
                    k = y*w + x;
                    pixel & p = pixels[k];
                    p.r() = static_cast<float>(px[0])/255.0;
                    p.g() = static_cast<float>(px[1])/255.0;
                    p.b() = static_cast<float>(px[2])/255.0;
                    p.a() = static_cast<float>(px[3])/255.0;
                }
            }
        }else{
            for(int y = 0; y < h; y++) {
                png_bytep row = row_pointers[y];
                for(int x = 0; x < w; x++) {
                    png_bytep px = &(row[x * span]);
                    k = y*w + x;
                    uint16_t color = px[1] | (px[0] << 8);
                    pixel & p = pixels[k];
                    p.r() = static_cast<float>(color)/fmax16;
                    color = px[3] | (px[2] << 8);
                    p.g() = static_cast<float>(color)/fmax16;
                    color = px[5] | (px[4] << 8);
                    p.b() = static_cast<float>(color)/fmax16;
                    color = px[7] | (px[8] << 8);
                    p.a() = static_cast<float>(color)/fmax16;
                }
            }
        }
        
        for(int y = 0; y < h; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        
        fclose(fp);
    }
    
    void split( uint16_t n, uint8_t & n1, uint8_t & n2 );
    
    void image::write_png_file( const char *filename, const Depth & depth ) const {
        png_bytep *row_pointers;
        FILE *fp = fopen(filename, "wb");
        if(!fp) abort();
        
        png_structp image = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!image) abort();
        
        png_infop info = png_create_info_struct(image);
        if (!info) abort();
        
        if (setjmp(png_jmpbuf(image))) abort();
        
        png_init_io(image, fp);
        
        // Output is RGBA format.
        png_set_IHDR(image,
                     info,
                     w, h,
                     depth,
                     PNG_COLOR_TYPE_RGBA,
                     PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);
        png_write_info(image, info);
        
        // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
        // Use png_set_filler().
        //png_set_filler(image, 0, PNG_FILLER_AFTER);
        int span = depth/2;
        uint16_t max16 = 0xFFFF;
        uint8_t max8 = 0xFF;
        float fmax16 = static_cast<float>(max16);
        float fmax8  = static_cast<float>(max8);
        
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * h);
        for(int y = 0; y < h; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(image,info));
            png_bytep row = row_pointers[y];
            for(int x = 0; x < w; x++ ){
                png_bytep px = &(row[x * span]);
                const pixel & p = (*this)(y,x);
                uint8_t n1, n2;
                if( depth == _8bit ){
                    px[0] = static_cast<uint8_t>( p.r()*fmax8);
                    px[1] = static_cast<uint8_t>( p.g()*fmax8);
                    px[2] = static_cast<uint8_t>( p.b()*fmax8);
                    px[3] = static_cast<uint8_t>( p.a()*fmax8);
                }else{
                    split(static_cast<uint16_t>( p.r()*fmax16), n1, n2);
                    px[0] = n1; px[1] = n2;
                    split(static_cast<uint16_t>( p.g()*fmax16), n1, n2);
                    px[2] = n1; px[3] = n2;
                    split(static_cast<uint16_t>( p.b()*fmax16), n1, n2);
                    px[4] = n1; px[5] = n2;
                    split(static_cast<uint16_t>( p.a()*fmax16), n1, n2);
                    px[6] = n1; px[7] = n2;
                }
            }
        }
        
        png_write_image(image, row_pointers);
        png_write_end(image, NULL);
        
        for(int y = 0; y < h; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        
        fclose(fp);
    }
    
    void split( uint16_t n, uint8_t & n1, uint8_t & n2 ){
        n1 = n | 0xFF00;
        n2 = n | 0x00FF;
    }
    
    //std::vector<pixel> pixels;
    //int height, cols;

}
