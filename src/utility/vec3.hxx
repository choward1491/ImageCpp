/*
 *  vec3.hxx
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef vec3_impl_hxx
#define vec3_impl_hxx

// useful macros for defining class methods
#define HEADER template<typename num_t>
#define CLASS vec<3,num_t>

// template class interface
#include "vec3.hpp"

// implementation details
namespace aSDG {
    namespace math {
            
        HEADER CLASS::vec(){
            d[0] = d[1] = d[2] = num_t();
        }
        
        HEADER CLASS::vec( num_t x, num_t y, num_t z){
            d[0] = x;
            d[1] = y;
            d[2] = z;
        }
        
        HEADER CLASS CLASS::cross( const vec & v ) const {
            vec out;
            out.d[0] = d[1]*v.d[2] - d[2]*v.d[1];
            out.d[1] = d[2]*v.d[0] - d[0]*v.d[2];
            out.d[2] = d[0]*v.d[1] - d[1]*v.d[0];
            return out;
        }
        
        HEADER void CLASS::cross( const vec & v, vec & output) const {
            output.d[0] = d[1]*v.d[2] - d[2]*v.d[1];
            output.d[1] = d[2]*v.d[0] - d[0]*v.d[2];
            output.d[2] = d[0]*v.d[1] - d[1]*v.d[0];
            return output;
        }
        // specify accessor methods
        HEADER num_t & CLASS::operator[](int idx){ return d[idx];}
        HEADER const num_t & CLASS::operator[](int idx) const { return d[idx]; }
        
        // specify norm-like methods
        HEADER num_t CLASS::dot( const vec & v ) const {
            return d[0]*v.d[0] + d[1]*v.d[1] + d[2]*v.d[2];
        }
        HEADER num_t CLASS::mag() const {
            return sqrt(magsq());
        }
        HEADER num_t CLASS::magsq() const{
            return d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
        }
        HEADER void CLASS::normalize() {
            num_t mag = mag();
            d[0] /= mag;
            d[1] /= mag;
            d[2] /= mag;
        }
        HEADER CLASS CLASS::getNormal() const {
            num_t m = mag();
            return vec(d[0]/m,d[1]/m,d[2]/m);
        }
        
        HEADER void CLASS::operator=( const num_t & s ) {
            d[0] = d[1] = d[2] = s;
        }
        
        HEADER CLASS CLASS::operator+( const vec & v ) const {
            return vec(d[0] + v.d[0], d[1] + v.d[1], d[2] + v.d[2]);
        }
        HEADER CLASS CLASS::operator-( const vec & v ) const {
            return vec(d[0] - v.d[0], d[1] - v.d[1], d[2] - v.d[2]);
        }
        HEADER CLASS CLASS::operator*( const vec & v ) const {
            return vec(d[0] * v.d[0], d[1] * v.d[1], d[2] * v.d[2]);
        }
        HEADER CLASS CLASS::operator/( const vec & v ) const {
            return vec(d[0] / v.d[0], d[1] / v.d[1], d[2] / v.d[2]);
        }
        HEADER CLASS CLASS::operator+( const num_t & d_ ) const {
            return vec(d[0] + d_, d[1] + d_, d[2] + d_);
        }
        HEADER CLASS CLASS::operator-( const num_t & d_ ) const {
            return vec(d[0] - d_, d[1] - d_, d[2] - d_);
        }
        HEADER CLASS CLASS::operator*( const num_t & d_ ) const {
            return vec(d[0] * d_, d[1] * d_, d[2] * d_);
        }
        HEADER CLASS CLASS::operator/( const num_t & d_ ) const {
            return vec(d[0] / d_, d[1] / d_, d[2] / d_);
        }
        
        // print method
        HEADER void CLASS::print() const{
            printf("[ %lf, %lf, %lf ]\n",(double)d[0],(double)d[1],(double)d[2]);
        }
        
    }// end namespace math
}// end namespace aSDG


// get rid of macro definitions
#undef HEADER
#undef CLASS

#endif /* vec3_impl_hxx */


