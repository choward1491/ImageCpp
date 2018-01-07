/*
 *  mat.hpp
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef mat_interface_hpp
#define mat_interface_hpp

namespace aSDG {
    namespace math {
        
        /*!
         * \class   mat
         * \file    mat.hpp
         * \brief   Data structure representing a generic mathematical matrix
         *
         * This data structure repersents a generic mathematical matrix and typical operations done on it
         */
        template<int R, int C, typename num_t = double>
        class mat {
        public:
            
            // specify useful typedefs
            typedef num_t num_type;
            
            // specify useful constexpr
            static constexpr int ndata = R*C;
            static constexpr int nrows = R;
            static constexpr int ncols = C;
            
            // ctor/dtor
            mat();
            ~mat() = default;
            
            // useful access operators
            num_t & operator()(int r, int c);
            const num_t & operator()(int r, int c) const;
            num_t & operator()(int k);
            const num_t & operator()(int k) const;
            num_t & operator[](int k);
            const num_t & operator[](int k) const;
            
            // useful math operators
            mat operator+(const mat & m) const;
            mat operator-(const mat & m) const;
            void operator=( const num_t & s );
            mat<C,R,num_t> getTranspose();
            
            // potentially useful methods
            num_t determinant() const;
            
            // useful explicit methods for getting dimensions of matrix
            int rows() const;
            int cols() const;
            
            // method to print matrix to screen
            void print() const;
            
        private:
            num_t d[ndata];
        };
        
        // declare some specializations
        template<> float mat<2,2,float>::determinant() const;
        template<> double mat<2,2,double>::determinant() const;
        template<> float mat<3,3,float>::determinant() const;
        template<> double mat<3,3,double>::determinant() const;
        template<> float mat<4,4,float>::determinant() const;
        template<> double mat<4,4,double>::determinant() const;
        
    }// end namespace math
}// end namespace aSDG


#include "mat.hxx"

#endif /* mat_interface_hpp */
