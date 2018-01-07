/*
 *  math_operators.hxx
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef math_operators_impl_hxx
#define math_operators_impl_hxx

// useful macros for defining class methods
#define H1 template<int R, int C, typename data>
#define H2 template<int R, int I, int C, typename data>

// template class interface
#include "math_operators.hpp"

// implementation details
namespace aSDG {
    namespace math {
        
        // function definitions
        
        H1 vec<R,data> operator*( const mat<R,C,data> & m, const vec<C,data> & v ){
            vec<R,data> o;
            for(int i = 0; i < R; ++i){
                for( int j = 0; j < C; ++j ){
                    o[i] += m(i,j)*v[j];
                }
            }
            return o;
        }
        
        H1 vec<C,data> operator*( const vec<R,data> & v, const mat<R,C,data> & m ){
            vec<C,data> o;
            for(int i = 0; i < C; ++i){
                for( int j = 0; j < R; ++j ){
                    o[i] += v[j]*m(j,i);
                }
            }
            return o;
        }
        
        H2 mat<R,C,data> operator*( const mat<R,I,data> & m1, const mat<I,C,data> & m2 ){
            mat<R,C,data> o;
            for(int i = 0; i < R; ++i){
                for( int j = 0; j < C; ++j ){
                    for( int k = 0; k < I; ++k){
                        o(i,j) += m1(i,k)*m2(k,j);
                    }
                }
            }
            return o;
        }
        
#define newop(OP) operator OP
#define mat_scalar_op1(OP) template<int R, int C, typename data>\
    mat<R,C,data> newop(OP) ( const mat<R,C,data> & m, const data & s ){\
        mat<R,C,data> o; \
        for( int i = 0; i < mat<R,C,data>::ndata; ++i ){ o[i] = m[i] OP s; } \
        return o; \
    }
#define mat_scalar_op2(OP) template<int R, int C, typename data>\
    mat<R,C,data> newop(OP) ( const data & s, const mat<R,C,data> & m ){\
        mat<R,C,data> o; \
        for( int i = 0; i < mat<R,C,data>::ndata; ++i ){ o[i] = m[i] OP s; } \
        return o; \
    }
        
        mat_scalar_op1(+)
        mat_scalar_op1(-)
        mat_scalar_op1(*)
        mat_scalar_op1(/)
        
        mat_scalar_op2(+)
        mat_scalar_op2(-)
        mat_scalar_op2(*)
        mat_scalar_op2(/)
        
    }// end namespace math
}// end namespace aSDG


// get rid of macro definitions
#undef H1
#undef H2

#endif /* math_operators_impl_hxx */


