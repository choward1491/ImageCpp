/*
 *  vec.hxx
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef vec_impl_hxx
#define vec_impl_hxx

// useful macros for defining class methods
#define CLASS vec<dim, num_t>
#define HEADER template<int dim, typename num_t>

// include standard libs
#include <cmath>

// template class interface
#include "vec.hpp"

namespace aSDG {
    namespace math {

        HEADER
        num_t & CLASS::operator[](int idx) {
            return d[idx];
        }
        HEADER
        const num_t & CLASS::operator[](int idx) const {
            return d[idx];
        }
        
        HEADER
        CLASS::vec(){
            num_t zero = num_t();
            for(int i = 0; i < dim; ++i){ d[i] = zero; }
        }
        
        // specify norm-like methods
        HEADER
        num_t CLASS::dot( const CLASS & v ) const {
            num_t out = num_t();
            for(int i = 0; i < dim; ++i ){ out += d[i]*v.d[i]; }
            return out;
        }
        HEADER
        num_t CLASS::mag() const {
            return sqrt(magsq());
        }
        HEADER
        num_t CLASS::magsq() const {
            num_t out = num_t();
            for(int i = 0; i < dim; ++i ){ out += d[i]*d[i]; }
            return out;
        }
        HEADER
        void CLASS::normalize(){
            num_t m = mag();
            for(int i = 0; i < dim; ++i ){ d[i] /= m; }
        }
        HEADER
        CLASS CLASS::getNormal() const {
            CLASS out;
            num_t m = mag();
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]/m; }
            return out;
        }
        
        HEADER
        void CLASS::operator=( const num_t & s ) {
            for(int i =0 ;i < dim; ++i){ d[i] = s; }
        }
        
        // specify element-wise math operators
        HEADER
        CLASS CLASS::operator+( const CLASS & v ) const{
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i] + v.d[i]; }
            return out;
        }
        HEADER
        CLASS CLASS::operator-( const CLASS & v ) const {
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]-v.d[i]; }
            return out;
        }
        HEADER
        CLASS CLASS::operator*( const CLASS & v ) const {
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]*v.d[i]; }
            return out;
        }
        HEADER
        CLASS CLASS::operator/( const CLASS & v ) const{
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]/v.d[i]; }
            return out;
        }
        HEADER
        CLASS CLASS::operator+( const num_t & d_ ) const {
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]+d_; }
            return out;
        }
        HEADER
        CLASS CLASS::operator-( const num_t & d_ ) const{
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]-d_; }
            return out;
        }
        HEADER
        CLASS CLASS::operator*( const num_t & d_ ) const {
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]*d_; }
            return out;
        }
        HEADER
        CLASS CLASS::operator/( const num_t & d_ ) const {
            CLASS out;
            for(int i = 0; i < dim; ++i ){ out.d[i] = d[i]/d_; }
            return out;
        }
        
        HEADER
        void CLASS::print() const {
            printf("[ %lf",d[0]);
            for(int i = 1; i < dim; ++i){
                printf(", %lf",d[i]);
            }
            printf(" ]\n");
        }
        
    }// end namespace math
}// end namespace aSDG

// get rid of macro definitions
#undef HEADER
#undef CLASS

#endif /* CLASS_impl_impl_hpp */


