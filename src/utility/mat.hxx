/*
 *  mat.hxx
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef mat_impl_hxx
#define mat_impl_hxx

// useful macros for defining class methods
#define MAT mat<R,C,num_t>
#define MATt mat<C,R,num_t>
#define DT typename MAT::num_type
#define HEADER template<int R, int C, typename num_t>

// standard libs
#include <stdio.h>

// template class interface
#include "mat.hpp"


namespace aSDG {
    namespace math {
        
        HEADER
        MAT::mat() {
            num_t zero = num_t();
            for(int i = 0; i < ndata; ++i){ d[i] = zero; }
        }
        
        HEADER
        DT & MAT::operator()(int r, int c) {
            return d[c + r*C];
        }
        
        HEADER
        const DT & MAT::operator()(int r, int c) const {
            return d[c + r*C];
        }
        
        HEADER
        DT & MAT::operator()(int k){
            return d[k];
        }
        
        HEADER
        const DT & MAT::operator()(int k) const{
            return d[k];
        }
        
        HEADER
        DT & MAT::operator[](int k){
            return d[k];
        }
        
        HEADER
        const DT & MAT::operator[](int k) const{
            return d[k];
        }
        
        HEADER
        MAT MAT::operator+(const mat & m) const {
            mat o;
            for(int i = 0; i < ndata; ++i){ o[i] = d[i] + m.d[i]; }
            return o;
        }
        
        HEADER
        MAT MAT::operator-(const mat & m) const {
            mat o;
            for(int i = 0; i < ndata; ++i){ o[i] = d[i] - m.d[i]; }
            return o;
        }
        
        HEADER
        void MAT::operator=( const num_t & s ) {
            for(int i =0; i < ndata; ++i){ d[i] = s; }
        }
        
        HEADER
        MATt MAT::getTranspose() {
            MATt mt;
            for(int i = 0; i < R; ++i){
                for(int j = 0; j < C; ++j){
                    mt(i,j) = this->operator()(j,i);
                }
            }
            return mt;
        }
        
        HEADER
        num_t MAT::determinant() const {
            printf("Use Basic Determinant..\n");
            return 0; // should come up with some algorithm for a generic matrix but until then...
        }
        
        HEADER
        int MAT::rows() const {
            return R;
        }
        
        HEADER
        int MAT::cols() const {
            return C;
        }
        
        HEADER
        void MAT::print() const {
            for(int i = 0; i < R; ++i){
                printf("%lf",static_cast<double>((*this)(i,0)));
                for(int j = 1; j < C; ++j){
                    printf(", %lf",static_cast<double>((*this)(i,j)));
                }
                printf("\n");
            }
        }
        
    }// end namespace math
}// end namespace aSDG



// get rid of macro definitions
#undef HEADER
#undef MAT
#undef MATt
#undef DT

#endif /* mat_impl_hpp */


