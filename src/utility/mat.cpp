/*
 *  mat.cpp
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 1/3/18
 *  Purpose:
 *
 */

// template class interface
#include "mat.hpp"


namespace aSDG {
    namespace math {
        
        template<> double mat<2,2,double>::determinant() const {
            return d[0]*d[3] - d[1]*d[2];
        }
        
        template<> float mat<2,2,float>::determinant() const {
            return d[0]*d[3] - d[1]*d[2];
        }
        
        template<> double mat<3,3,double>::determinant() const {
            /* Latex expression for what's implemented. Found using sympy for matrix A
             with indices listed in row order, not column order
             
             \begin{align}
             a_{0} a_{4} a_{8} - a_{0} a_{5} a_{7} - a_{1} a_{3} a_{8} + a_{1} a_{5} a_{6} + a_{2} a_{3} a_{7} - a_{2} a_{4} a_{6}
             \end{align}
             */
            return d[0]*(d[4]*d[8] - d[5]*d[7])
            + d[1]*(d[5]*d[6] - d[3]*d[8])
            + d[2]*(d[3]*d[7] - d[4]*d[6]);
        }
        
        template<> float mat<3,3,float>::determinant() const {
            /* Latex expression for what's implemented. Found using sympy for matrix A
             with indices listed in row order, not column order
             
             \begin{align}
             a_{0} a_{4} a_{8} - a_{0} a_{5} a_{7} - a_{1} a_{3} a_{8} + a_{1} a_{5} a_{6} + a_{2} a_{3} a_{7} - a_{2} a_{4} a_{6}
             \end{align}
             */
            return d[0]*(d[4]*d[8] - d[5]*d[7])
            + d[1]*(d[5]*d[6] - d[3]*d[8])
            + d[2]*(d[3]*d[7] - d[4]*d[6]);
        }
        
        template<> double mat<4,4,double>::determinant() const {
            /* Latex expression for what's implemented. Found using sympy for matrix A
             with indices listed in row order, not column order
             
             \begin{align}
             &a_{10} \left( a_{12} \left(a_{1} a_{7} - a_{3} a_{5}\right) + a_{13} \left(- a_{0} a_{7} + a_{3} a_{4}\right) + a_{15} \left(a_{0} a_{5} - a_{1} a_{4}\right)\right) \\
             &+ a_{11} \left( a_{12} \left(- a_{1} a_{6} + a_{2} a_{5}\right) + a_{13} \left(a_{0} a_{6} - a_{2} a_{4}\right) + a_{14} \left(- a_{0} a_{5} + a_{1} a_{4}\right) \right)\\
             &+ a_{9}  \left( a_{12}\left(- a_{2} a_{7} + a_{3} a_{6}\right) + a_{14} \left(a_{0} a_{7} - a_{3} a_{4}\right) + a_{15} \left(- a_{0} a_{6} + a_{2} a_{4}\right) \right)\\
             &+ a_{8} \left( a_{13} \left(a_{2} a_{7} - a_{3} a_{6}\right) + a_{14}  \left(- a_{1} a_{7} + a_{3} a_{5}\right) + a_{15} \left(a_{1} a_{6} - a_{2} a_{5}\right) \right)
             \end{align}
             */
            return d[8]*(d[13]*(d[2]*d[7] - d[3]*d[6]) + d[14]*(d[3]*d[5] - d[1]*d[7]) + d[15]*(d[1]*d[6] - d[2]*d[5]))
            + d[9]*(d[12]*(d[3]*d[6] - d[2]*d[7]) + d[14]*(d[0]*d[7] - d[3]*d[4]) + d[15]*(d[2]*d[4] - d[0]*d[6]))
            + d[10]*(d[12]*(d[1]*d[7] - d[3]*d[5]) + d[13]*(d[3]*d[4] - d[0]*d[7]) + d[15]*(d[0]*d[5]-d[1]*d[4]))
            + d[11]*(d[12]*(d[2]*d[5] - d[1]*d[6]) + d[13]*(d[0]*d[6]-d[2]*d[4]) + d[14]*(d[1]*d[4]-d[0]*d[5]));
        }
        
        template<> float mat<4,4,float>::determinant() const {
            /* Latex expression for what's implemented. Found using sympy for matrix A
             with indices listed in row order, not column order
             
             \begin{align}
             &a_{10} \left( a_{12} \left(a_{1} a_{7} - a_{3} a_{5}\right) + a_{13} \left(- a_{0} a_{7} + a_{3} a_{4}\right) + a_{15} \left(a_{0} a_{5} - a_{1} a_{4}\right)\right) \\
             &+ a_{11} \left( a_{12} \left(- a_{1} a_{6} + a_{2} a_{5}\right) + a_{13} \left(a_{0} a_{6} - a_{2} a_{4}\right) + a_{14} \left(- a_{0} a_{5} + a_{1} a_{4}\right) \right)\\
             &+ a_{9}  \left( a_{12}\left(- a_{2} a_{7} + a_{3} a_{6}\right) + a_{14} \left(a_{0} a_{7} - a_{3} a_{4}\right) + a_{15} \left(- a_{0} a_{6} + a_{2} a_{4}\right) \right)\\
             &+ a_{8} \left( a_{13} \left(a_{2} a_{7} - a_{3} a_{6}\right) + a_{14}  \left(- a_{1} a_{7} + a_{3} a_{5}\right) + a_{15} \left(a_{1} a_{6} - a_{2} a_{5}\right) \right)
             \end{align}
             */
            return d[8]*(d[13]*(d[2]*d[7] - d[3]*d[6]) + d[14]*(d[3]*d[5] - d[1]*d[7]) + d[15]*(d[1]*d[6] - d[2]*d[5]))
            + d[9]*(d[12]*(d[3]*d[6] - d[2]*d[7]) + d[14]*(d[0]*d[7] - d[3]*d[4]) + d[15]*(d[2]*d[4] - d[0]*d[6]))
            + d[10]*(d[12]*(d[1]*d[7] - d[3]*d[5]) + d[13]*(d[3]*d[4] - d[0]*d[7]) + d[15]*(d[0]*d[5]-d[1]*d[4]))
            + d[11]*(d[12]*(d[2]*d[5] - d[1]*d[6]) + d[13]*(d[0]*d[6]-d[2]*d[4]) + d[14]*(d[1]*d[4]-d[0]*d[5]));
        }
        
    }// end namespace math
}// end namespace aSDG
