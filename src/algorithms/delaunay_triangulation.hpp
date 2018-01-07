/*
 *  delaunay_triangulation.hpp
 *
 *  Project: ImageCpp
 *  Author : Christian J Howard
 *  Date   : 12/28/17
 *  Purpose:
 *
 */

#ifndef delaunay_triangulation_interface_hpp
#define delaunay_triangulation_interface_hpp

#include <queue>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <random>
#include <limits>
#include <set>
#include <map>
#include "triangle.hpp"
#include "triangle2.hpp"

namespace delaunay {
    
    template<typename num_type>
    void triangulation(const ::std::vector<num_type> & x,
                       const ::std::vector<num_type> & y,
                       std::vector<Triangle<num_type>> & triangles,
                       std::pair<int,int> boxdims = std::pair<int,int>(10,10) );
    
    template<typename num_type>
    void legalizeEdge(unsigned int vid, unsigned int edge_id,
                      const ::std::vector<num_type> & xv, const ::std::vector<num_type> & yv,
                      std::map<unsigned int, Edge<num_type>> & edge_map,
                      std::vector<a2::Triangle<num_type>> & int_triangles );
};


#include "delaunay_triangulation.hxx"

#endif /* delaunay_triangulation_interface_hpp */
