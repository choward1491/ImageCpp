/*
 *  vec3.hpp
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/28/17
 *  Purpose:
 *
 */

#ifndef vec3_interface_hpp
#define vec3_interface_hpp

#include "vec.hpp"

namespace aSDG {
    namespace math {
        
        template<typename num_t>
        class vec<3,num_t> {
        public:
            
            // specify typedefs
            typedef num_t num_type;
            
            // specify useful constexpr
            static constexpr int ndims = 3;
            
            //ctor/dtor
            vec();
            vec( num_t x, num_t y, num_t z);
            ~vec() = default;
            
            // methods for cross products
            vec cross( const vec & v ) const;
            void cross( const vec & v, vec & output) const ;
            
            // specify accessor methods
            num_t & operator[](int idx);
            const num_t & operator[](int idx) const;
            
            // specify norm-like methods
            num_t dot( const vec & v ) const;
            num_t mag() const;
            num_t magsq() const;
            void normalize();
            vec getNormal() const;
            
            // operator = overload
            void operator=( const num_t & s );
            
            // mathematical operators
            vec operator+( const vec & v ) const;
            vec operator-( const vec & v ) const;
            vec operator*( const vec & v ) const;
            vec operator/( const vec & v ) const;
            vec operator+( const num_t & d_ ) const;
            vec operator-( const num_t & d_ ) const;
            vec operator*( const num_t & d_ ) const;
            vec operator/( const num_t & d_ ) const;
            
            // print method
            void print() const;
            
        protected:
            num_t d[ndims];
        };
        
    }// end namespace math
}// end namespace aSDG


#include "vec3.hxx"

#endif /* vec3_interface_hpp */
