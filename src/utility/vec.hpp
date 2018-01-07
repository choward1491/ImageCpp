/*
 *  vec.hpp
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef vec_interface_hpp
#define vec_interface_hpp

namespace aSDG {
    namespace math {
        
        /*!
         * \class   vec
         * \file    vec.hpp
         * \brief   Data structure representing a generic mathematical vector
         *
         * This data structure repersents a generic mathematical vector and typical operations done on it
         */
        template<int dim, typename num_t = double>
        class vec {
        public:
            
            // specify typedefs
            typedef num_t num_type;
            
            // specify useful constexpr
            static constexpr int ndims = dim;
            
            // ctor/dtor
            vec();
            ~vec() = default;
            
            // specify accessor methods
            num_t & operator[](int idx);
            const num_t & operator[](int idx) const;
            
            // specify norm-like methods
            num_t dot( const vec & v ) const;
            num_t mag() const;
            num_t magsq() const;
            void normalize();
            vec getNormal() const;
            
            // specify element-wise math operators
            vec operator+( const vec & v ) const;
            vec operator-( const vec & v ) const;
            vec operator*( const vec & v ) const;
            vec operator/( const vec & v ) const;
            vec operator+( const num_t & d ) const;
            vec operator-( const num_t & d ) const;
            vec operator*( const num_t & d ) const;
            vec operator/( const num_t & d ) const;
            
            // overloaded operator =
            void operator=( const num_t & s );
            
            // print method
            void print() const;
            
        protected:
            num_t d[dim];
        };
        
    }// end namespace math
}// end namespace aSDG


#include "vec.hxx"

#endif /* vec_interface_hpp */
