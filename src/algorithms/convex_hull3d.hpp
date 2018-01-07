//
//  convex_hull3d.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/19/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#ifndef convex_hull3d_h
#define convex_hull3d_h

#include <vector>
#include <random>
#include "triangle.hpp"
#include <queue>
#include <unordered_map>
#include <cmath>


// define overloaded templated class
namespace std {
    
    template<typename T>
    struct less< std::pair<T,unsigned int> > {
        
        bool operator()(const std::pair<T,unsigned int> &lhs,
                        const std::pair<T,unsigned int> &rhs) const
        {
            return lhs.first < rhs.first;
        }
        
    };
    
}

namespace _3D {
    
    // template function for assigning 3d arrays
    template< typename T>
    void assign( T p[], T x, T y, T z){
        p[0] = x;
        p[1] = y;
        p[2] = z;
    }
    
    // compute signed distance between plane and some coordinate in 3D
    template< typename T >
    T dist_plane2pt( T ppt[], T normal[], T p[] ){
        T del[3] = {p[0] - ppt[0], p[1] - ppt[1], p[2] - ppt[2]};
        T dp  = del[0]*normal[0] + del[1]*normal[1] + del[2]*normal[2];
        return dp;
    }
    
    template< typename T >
    T dist_pt2pt( T pt1[], T pt2[] ){
        T dx  = pt1[0] - pt2[0];
        T dy  = pt1[1] - pt2[1];
        T dz  = pt1[2] - pt2[2];
        T mag = sqrt( dx*dx + dy*dy + dz*dz );
        
        return mag;
    }
    
    template< typename T >
    T dist_line2pt( T p1[], T p2[], T p[]){
        
        // Find relative vector along line
        T del[] = {p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]};
        
        // Find magnitude of relative vector along line
        T mag   = sqrt(del[0]*del[0] + del[1]*del[1] + del[2]*del[2]);
        
        // Generate unit vector along line
        T u[] = {del[0]/mag, del[1]/mag, del[2]/mag};
        
        // Find relative vector from starting point of line to point p
        assign<T>(del , p[0]-p1[0], p[1]-p1[1], p[2]-p1[2] );
        
        // Find dot product between del and u
        T dp = u[0]*del[0] + u[1]*del[1] + u[2]*del[2];
        
        // Find vector component of del perpendicular to u
        T d[3];
        d[0] = del[0] - dp * u[0];
        d[1] = del[1] - dp * u[1];
        d[2] = del[2] - dp * u[2];
        
        // Find magnitude of perpendicular component
        T dist = sqrt( d[0]*d[0] + d[1]*d[1] + d[2]*d[2] );
        
        return dist;
    }
    
    template< typename num_type >
    void setNeighbor( unsigned int parent_tri_id,
                      std::queue<unsigned int> & neighbor_ids,
                     std::unordered_map<unsigned int, std::pair<Triangle<num_type>,std::priority_queue<std::pair<num_type,unsigned int>, std::vector<std::pair<num_type,unsigned int>>, std::less<std::pair<num_type,unsigned int>> >>> & data,
                      const Triangle<num_type>** tri_list,
                      Triangle<num_type> & tri )
    {
        const Triangle<num_type>* ptr = nullptr;
        
        while(neighbor_ids.size() != 0) {
            int tmp = neighbor_ids.front(); neighbor_ids.pop();
            for(int k = 0; k < 3; ++k){
                if( tmp == tri_list[k]->id ){ ptr = tri_list[k]; break; }
            }
            
            bool sharesEdge = false;
            for(int i = 0; i < 3; ++i){
                sharesEdge = sharesEdge || ptr->shareEdge( tri.vertices[i], tri.vertices[(i+1)%3]) ;
                if( sharesEdge ){ break; }
            }
            
            if ( sharesEdge ) {
                tri.neighbors[2] = ptr->id;
                auto & n_tri = data[ptr->id].first;
                for(int k = 0; k < 3; ++k){
                    if( n_tri.neighbors[k] == parent_tri_id ){
                        n_tri.neighbors[k] = tri.id;
                        break;
                    }
                }
                break;
            }else{
                neighbor_ids.push(tmp);
            }// end if
        }// end for i
    }
    
    
    // method to select initial tetrahedron. this will be updated to be something more clever
    // eventually. currently doing random selection
    template<typename num_type>
    void initial_tetrahedron_selector( const ::std::vector<num_type> & x,
                                      const ::std::vector<num_type> & y,
                                      const ::std::vector<num_type> & z,
                                      unsigned int *out_vertices )
    {
        
        // init some variables
        std::default_random_engine generator(17);
        std::uniform_int_distribution<unsigned int> distribution(0,UINT32_MAX);
        unsigned int num_data = x.size();
        num_type value = num_type(0), tmp = num_type(0);
        unsigned int best_idx = 0;
        num_type p1[3] = {0}, p2[3] = {0}, p3[3] = {0};
        
        
        // select point with most negative x value
        out_vertices[0] = 0;
        value = x[0];
        for(int i = 1; i < num_data; ++i){
            if( x[i] < value ){
                out_vertices[0] = i;
                value = x[i];
            }
        }
        
        // select point furthest from current index
        value = 0; out_vertices[1] = out_vertices[0];
        p1[0] = x[out_vertices[0]];
        p1[1] = y[out_vertices[0]];
        p1[2] = z[out_vertices[0]];
        for(int i = 0; i < num_data; ++i){
            if( i != out_vertices[0] ){
                p2[0] = x[i];
                p2[1] = y[i];
                p2[2] = z[i];
                tmp = dist_pt2pt(p1, p2);
                if( tmp > value ){
                    value = tmp;
                    out_vertices[1] = i;
                }
            }
        }
        
        // select point furthest from current line
        value = 0; out_vertices[2] = out_vertices[0];
        p2[0] = x[out_vertices[1]];
        p2[1] = y[out_vertices[1]];
        p2[2] = z[out_vertices[1]];
        for(int i = 0; i < num_data; ++i){
            if( i != out_vertices[0] && i != out_vertices[1] ){
                p3[0] = x[i];
                p3[1] = y[i];
                p3[2] = z[i];
                tmp = dist_line2pt(p1, p2, p3);
                if( tmp > value ){
                    value = tmp;
                    out_vertices[2] = i;
                }
            }
        }
        
        // select point furthest from current triangle
        value = 0; out_vertices[3] = out_vertices[0];
        Triangle<num_type> tri;
        tri.vertices[0] = out_vertices[0];
        tri.vertices[1] = out_vertices[1];
        tri.vertices[2] = out_vertices[2];
        tri.makeNormal( &x[0], &y[0], &z[0], num_data );
        for(int i = 0; i < num_data; ++i){
            if( i != out_vertices[0] && i != out_vertices[1] && i != out_vertices[2] ){
                p3[0] = x[i];
                p3[1] = y[i];
                p3[2] = z[i];
                tmp = std::abs(dist_plane2pt(p1, tri.normal, p3));
                if( tmp > value ){
                    value = tmp;
                    out_vertices[3] = i;
                }
            }
        }
    }
    
    
    bool isTetrahedronVertices( const unsigned int *tetra0, unsigned int idx );
    
    
    // convex hull
    template<typename num_type>
    void convhull( const std::vector<num_type> & x,
                   const std::vector<num_type> & y,
                   const std::vector<num_type> & z,
                   std::vector<Triangle<num_type>> & triangles )
    {
        
        // specify some typedefs for ease
        typedef Triangle<num_type> triangle;
        typedef std::pair<num_type,unsigned int> ch_pair;
        typedef std::priority_queue<ch_pair, std::vector<ch_pair>, std::less<ch_pair> > max_heap;
        
        // init some variables
        unsigned int id_counter = 0;
        unsigned int num_data = x.size();
        num_type dist = num_type(0);
        num_type plane_pt[3] = {0}, cloud_pt[3] = {0};
        std::unordered_map<unsigned int, std::pair<triangle,max_heap>> data;
        std::queue<unsigned int>    tri_q;
        
        
        // construct initial tetrahedron and data in associated queues for breadth first search
        {
            // init some variables
            unsigned int init_tetra[4] = {0};
            triangle tri[4];
            max_heap tri_data[4];
            
            // select initial tetrahedron
            initial_tetrahedron_selector( x, y, z, init_tetra );
            
            // populate triangle faces for tetrahedron
            for(int i = 0; i < 4; ++i){ tri[i].id = id_counter++; }
            assign<unsigned int>(tri[0].vertices,init_tetra[0],init_tetra[1],init_tetra[3]);
            assign<unsigned int>(tri[1].vertices,init_tetra[1],init_tetra[2],init_tetra[3]);
            assign<unsigned int>(tri[2].vertices,init_tetra[2],init_tetra[0],init_tetra[3]);
            assign<unsigned int>(tri[3].vertices,init_tetra[2],init_tetra[1],init_tetra[0]);
            assign<unsigned int>(tri[0].neighbors,1, 2, 3);
            assign<unsigned int>(tri[1].neighbors,0, 2, 3);
            assign<unsigned int>(tri[2].neighbors,0, 1, 3);
            assign<unsigned int>(tri[3].neighbors,0, 1, 2);
            
            // construct triangle faces' normal vector
            for(int i = 0; i < 4; ++i){ tri[i].makeNormal( &x[0], &y[0], &z[0], num_data ); }
            
            // populate distance sorted max_heaps for each triangle face
            for(unsigned int k = 0; k < num_data; ++k){
                if( !isTetrahedronVertices( init_tetra, k ) ){
                    cloud_pt[0] = x[k]; cloud_pt[1] = y[k]; cloud_pt[2] = z[k];
                    for(unsigned int id = 0; id < 4; ++id){
                        plane_pt[0] = x[tri[id].vertices[0]];
                        plane_pt[1] = y[tri[id].vertices[0]];
                        plane_pt[2] = z[tri[id].vertices[0]];
                        dist = dist_plane2pt( plane_pt, tri[id].normal, cloud_pt );
                        if( dist > 0.0 ){
                            tri_data[id].push(std::pair<num_type,unsigned int>(dist,k));
                        }// end if
                    }// end for id
                }else{
                    printf("yadda\n");
                }
            }// end for k
            
            // populate data hash table with triangle and point cloud data
            for(int i = 0; i < 4; ++i){
                data[tri[i].id] = std::pair<triangle,max_heap>(tri[i],tri_data[i]);
                if( tri_data[i].size() != 0 ){ tri_q.push(tri[i].id); }
            }
            
        }
        
        // loop through non-terminal faces until no faces have any more data to check
        
        while( !tri_q.empty() ){
            
            // pop out triangle ID associated with triangle at front of queue
            unsigned int tri_id = tri_q.front(); tri_q.pop();
            
            // get reference to underlying data at this location
            auto & tri_res  = data[tri_id];
            auto & tri      = tri_res.first;
            auto & heap     = tri_res.second;
            
            // pop vertex index used to create a new tetrahedron based on furthest
            // distance from triangle plane
            num_type distance= heap.top().first;
            unsigned int idx = heap.top().second; heap.pop();
            
            // setup new triangles to be made
            triangle new_tri[3];
            for(int i = 0; i < 3; ++i){ new_tri[i].id = id_counter++; }
            
            // set neighbor IDs
            std::queue<unsigned int> neighbor_ids;
            for(int i = 0; i < 3; ++i){ neighbor_ids.push( tri.neighbors[i] ); }
            
            // define neighbor triangle list
            const triangle* tri_list[3] = { &data[tri.neighbors[0]].first,
                                      &data[tri.neighbors[1]].first,
                                      &data[tri.neighbors[2]].first };
            
            // triangle 0
            new_tri[0].vertices[0] = tri.vertices[0];
            new_tri[0].vertices[1] = tri.vertices[1];
            new_tri[0].vertices[2] = idx;
            new_tri[0].neighbors[0] = new_tri[1].id;
            new_tri[0].neighbors[1] = new_tri[2].id;
            
            // triangle 1
            new_tri[1].vertices[0] = tri.vertices[1];
            new_tri[1].vertices[1] = tri.vertices[2];
            new_tri[1].vertices[2] = idx;
            new_tri[1].neighbors[0] = new_tri[0].id;
            new_tri[1].neighbors[1] = new_tri[2].id;
            
            // triangle 2
            new_tri[2].vertices[0] = tri.vertices[2];
            new_tri[2].vertices[1] = tri.vertices[0];
            new_tri[2].vertices[2] = idx;
            new_tri[2].neighbors[0] = new_tri[0].id;
            new_tri[2].neighbors[1] = new_tri[1].id;
            
            // set neighbors based on original triangle that generated the new triangles
            // also define normal for new triangles
            for(int i = 0; i < 3; ++i){
                setNeighbor( tri.id, neighbor_ids, data, tri_list, new_tri[i] );
                new_tri[i].makeNormal( &x[0], &y[0], &z[0], num_data );
                new_tri[i].print(&x[0], &y[0], &z[0]);
                printf("Area = %0.10e\n",new_tri[i].area(&x[0], &y[0], &z[0]));
            }
            
            // obtain data for each new triangle face and push this onto the queue
            max_heap tri_data[3];
            while( !heap.empty() ){
                
                // get index for next data point
                unsigned int idx = heap.top().second; heap.pop();
                cloud_pt[0] = x[idx]; cloud_pt[1] = y[idx]; cloud_pt[2] = z[idx];
                
                // loop through triangles to see if data point should be considered
                for(int id = 0; id < 3; ++id){
                    plane_pt[0] = x[new_tri[id].vertices[0]];
                    plane_pt[1] = y[new_tri[id].vertices[0]];
                    plane_pt[2] = z[new_tri[id].vertices[0]];
                    dist = dist_plane2pt( plane_pt, new_tri[id].normal, cloud_pt );
                    if( dist > 0.0 ){
                        tri_data[id].push(std::pair<num_type,unsigned int>(dist,idx));
                    }// end if
                }// end for i
            }// end while heap not empty
            
            // populate data hash table with triangle and point cloud data
            for(int i = 0; i < 3; ++i){
                data[new_tri[i].id] = std::pair<triangle,max_heap>(new_tri[i],tri_data[i]);
                if( tri_data[i].size() != 0 ){ tri_q.push(new_tri[i].id); }
            }
            
            // erase data for old triangle
            data.erase(tri.id);
            
        }
        
        // assign triangle set to output vector
        triangles.clear();
        for( auto it = data.begin(); it != data.end(); ++it ){
            triangles.push_back( it->second.first );
        }
    }
    
}


#endif /* convex_hull3d_h */
