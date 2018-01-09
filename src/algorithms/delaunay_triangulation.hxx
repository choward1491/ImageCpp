/*
 *  delaunay_triangulation.hxx
 *
 *  Project: ImageCpp
 *  Author : Christian J Howard
 *  Date   : 12/28/17
 *  Purpose:
 *
 */

#ifndef delaunay_triangulation_impl_hxx
#define delaunay_triangulation_impl_hxx

// useful macros for defining class methods
#define HEADER template<typename T>
#define CLASS delaunay_triangulation<T>

// template class interface
#include "delaunay_triangulation.hpp"

// implementation details
namespace delaunay {
    
    template<typename num_type>
    std::pair<num_type,num_type> getMinMax( const ::std::vector<num_type> & v)
    {
        std::pair<num_type,num_type> out(std::numeric_limits<num_type>::max(),
                                         std::numeric_limits<num_type>::lowest());
        
        for(auto & value : v){
            if( value < out.first ) { out.first     = value; }
            if( value > out.second ){ out.second    = value;}
        }
        
        return out;
    }
    
    template< typename T >
    T dist_pt2pt( T x1, T y1, T x2, T y2 ){
        T dx  = x1 - x2;
        T dy  = y1 - y2;
        T mag = dx*dx + dy*dy;
        return mag;
    }
    
    template<typename num_type>
    int spatial_hash(num_type x, num_type y, std::pair<int,int> dims){
        
    }
    
    template<typename num_type>
    void triangulation(const ::std::vector<num_type> & x,
                       const ::std::vector<num_type> & y,
                       std::vector<Triangle<num_type>> & triangles,
                       std::pair<int,int> boxdims)
    {
        typedef Triangle<num_type> ftri_t;
        typedef a2::Triangle<num_type> tri_t;
        typedef Edge<num_type> e_t;
        
        // get bounds for the x and y vertices
        auto xbnds = getMinMax(x), ybnds = getMinMax(y);
        
        // init boxes for spatial hashing
        std::vector<std::set<unsigned int>> hashtable(boxdims.first*boxdims.second);
        
        // clear triangle vector
        triangles.clear();
        std::vector<tri_t> internal_tris;
        std::vector<bool> internal_use;
        std::map<unsigned int,e_t> emap;
        
        // will speed up the below algorithms later
        // make copies of coordinates
        size_t size0 = x.size();
        std::vector<num_type> xc;
        std::vector<num_type> yc;
        
        // compute initial bounding triangle vertices
        const num_type radius = 5.0;
        num_type dx = (xbnds.second-xbnds.first), dy = (ybnds.second-ybnds.first);
        num_type v1[2] = {xbnds.first + dx*0.5*(1.0 + radius*sqrt(3.0)), ybnds.first + dy*0.5*(1.0 - radius) };
        num_type v2[2] = {xbnds.first + dx*(0.5), ybnds.first + dy*(radius + 0.5) };
        num_type v3[2] = {xbnds.first + dx*0.5*(1.0 - radius*sqrt(3.0)), ybnds.first + dy*0.5*(1.0 - radius) };
        xc.push_back(v1[0]); yc.push_back(v1[1]);
        xc.push_back(v2[0]); yc.push_back(v2[1]);
        xc.push_back(v3[0]); yc.push_back(v3[1]);
        
        for(int i = 0; i < size0; ++i){
            xc.push_back(x[i]);
            yc.push_back(y[i]);
        }
        
        // construct useful references to underlying data in xc,yc
        const num_type* xv= &xc[0];
        const num_type* yv= &yc[0];
        
        // construct initial triangle
        unsigned int eid_tracker = 1,tid_tracker = 1;
        std::vector<std::set<int>> vert_tri_map(xc.size());
        
        { // create a new triangle
            
            // construct edges
            e_t edges[3];
            edges[0].vertices[0] = 0;
            edges[0].vertices[1] = 1;
            edges[1].vertices[0] = 1;
            edges[1].vertices[1] = 2;
            edges[2].vertices[0] = 2;
            edges[2].vertices[1] = 0;
            edges[0].id = eid_tracker++;
            edges[1].id = eid_tracker++;
            edges[2].id = eid_tracker++;
            
            // construct triangle
            tri_t t0;
            t0.setEdgeMap(emap);
            t0.edges[0]  = edges[0].id;
            t0.edges[1]  = edges[1].id;
            t0.edges[2]  = edges[2].id;
            t0.id        = tid_tracker++;
            internal_tris.push_back(t0);
            internal_use.push_back(true);
            
            // set triangle connectivity with edges
            edges[0].setTriangle(0, t0.id);
            edges[1].setTriangle(0, t0.id);
            edges[2].setTriangle(0, t0.id);
            
            // add to edge map
            for(int k = 0; k < 3; ++k){
                emap[edges[k].id] = edges[k];
            }
        }
        
        for(int i = 3; i < xc.size(); ++i){
            int tid = 0;
            
            for(int k = 0; k < internal_tris.size(); ++k){
                if( internal_use[k] && internal_tris[k].pointWithinTriangle( xc[i], yc[i], xv, yv, xc.size() ) ){ tid = k; break; }
            }
             
            
            // break the triangle into pieces and remove the triangle that is intersected
            // create the new edges first
            e_t* eref[3] = {&emap[internal_tris[tid].edges[0]],&emap[internal_tris[tid].edges[1]],&emap[internal_tris[tid].edges[2]]};
            bool dir[3] = {internal_tris[tid].dir[0],internal_tris[tid].dir[1],internal_tris[tid].dir[2]};
            
            e_t edges[3];
            edges[0].vertices[0] = eref[0]->vertices[(dir[0]+1)%2];
            edges[0].vertices[1] = i;
            edges[1].vertices[0] = eref[1]->vertices[(dir[1]+1)%2];
            edges[1].vertices[1] = i;
            edges[2].vertices[0] = eref[2]->vertices[(dir[2]+1)%2];
            edges[2].vertices[1] = i;
            edges[0].id = eid_tracker++;
            edges[1].id = eid_tracker++;
            edges[2].id = eid_tracker++;
            
            // construct triangles next
            tri_t tris[3];
            tris[0].edges[0]    = internal_tris[tid].edges[0];
            tris[0].dir[0]      = dir[0];
            tris[0].edges[1]    = edges[0].id;
            tris[0].dir[1]      = false;
            tris[0].edges[2]    = edges[2].id;
            tris[0].dir[2]      = true;
            tris[0].id          = tid_tracker++;
            
            
            tris[1].edges[0]    = internal_tris[tid].edges[1];
            tris[1].dir[0]      = dir[1];
            tris[1].edges[1]    = edges[1].id;
            tris[1].dir[1]      = false;
            tris[1].edges[2]    = edges[0].id;
            tris[1].dir[2]      = true;
            tris[1].id          = tid_tracker++;
            
            tris[2].edges[0]    = internal_tris[tid].edges[2];
            tris[2].dir[0]      = dir[2];
            tris[2].edges[1]    = edges[2].id;
            tris[2].dir[1]      = false;
            tris[2].edges[2]    = edges[1].id;
            tris[2].dir[2]      = true;
            tris[2].id          = tid_tracker++;
            
            // pair triangles to edges
            if( eref[0]->triangles[0] == (tid+1) )  { eref[0]->setTriangle(0, tris[0].id); }
            else                                { eref[0]->setTriangle(1, tris[0].id); }
            if( eref[1]->triangles[0] == (tid+1) )  { eref[1]->setTriangle(0, tris[1].id); }
            else                                { eref[1]->setTriangle(1, tris[1].id); }
            if( eref[2]->triangles[0] == (tid+1) )  { eref[2]->setTriangle(0, tris[2].id); }
            else                                { eref[2]->setTriangle(1, tris[2].id); }
            
            edges[0].setTriangle(0, tris[0].id); edges[0].setTriangle(1, tris[1].id);
            edges[1].setTriangle(0, tris[1].id); edges[1].setTriangle(1, tris[2].id);
            edges[2].setTriangle(0, tris[2].id); edges[2].setTriangle(1, tris[0].id);
            
            // add triangles to list and edges to map
            //printf("p = (%lf, %lf)\n",xv[i],yv[i]);
            internal_use[tid] = false;
            for(int k = 0; k < 3; ++k ){
                tris[k].setEdgeMap(emap);
                internal_tris.push_back(tris[k]);
                internal_use.push_back(true);
                emap[edges[k].id] = edges[k];
            }
            
            
            // check that things are legal
            legalizeEdge<num_type>(i, internal_tris[tid].edges[0],
                         xc,yc,emap,internal_tris );
            legalizeEdge<num_type>(i, internal_tris[tid].edges[1],
                         xc,yc,emap,internal_tris );
            legalizeEdge<num_type>(i, internal_tris[tid].edges[2],
                         xc,yc,emap,internal_tris );
            
        }// end for loop
        
        // remove triangles that are not usable
        for(int k = 0; k < internal_tris.size(); ++k){
            tri_t& tri = internal_tris[k];
            if( internal_use[k] ){
                if ( tri.hasVertexID(0) || tri.hasVertexID(1) || tri.hasVertexID(2) || tri.area(&xc[0], &yc[0]) < 1e-6 ){
                    // do nothing
                }else{
                    ftri_t tri2 = tri.getRawTriangle();
                    tri2.vertices[0] -= 3;
                    tri2.vertices[1] -= 3;
                    tri2.vertices[2] -= 3;
                    triangles.push_back(tri2);
                }
            }
        }// end while loop
        
    }
    
    template<typename num_type>
    void legalizeEdge(unsigned int vid, unsigned int edge_id,
                      const ::std::vector<num_type> & xv, const ::std::vector<num_type> & yv,
                      std::map<unsigned int, Edge<num_type>> & edge_map,
                      std::vector<a2::Triangle<num_type>> & int_triangles )
    {
        
        // initialization of stuff
        Edge<num_type> & edge = edge_map[edge_id];
        a2::Triangle<num_type>* tref[2] = {nullptr};
        
        // if edge is not shared, we should not perform this
        if( edge.triangles[0] == 0 || edge.triangles[1] == 0 ){ return; }
        
        // get the triangle that will be used for circumcircle test
        unsigned int i0 = edge.triangles[0]-1, i1 = edge.triangles[1]-1;
        tref[0] = &int_triangles[i0];
        tref[1] = &int_triangles[i1];
        
        unsigned int tid = 0;
        if( int_triangles[i0].hasVertexID(vid) ) { tid = 1; }
        
        // check if edge is valid
        if( tref[tid]->isWithinCircumcircle(xv[vid], yv[vid], &xv[0], &yv[0]) ){ // if this passes, it means edge is illegal
            
            // flip the edge
            unsigned int idx[2] = {0};
            for(int i = 0; i < 3; ++i){
                if( tref[0]->edges[i] == edge_id ){ idx[0] = i;}
                if( tref[1]->edges[i] == edge_id ){ idx[1] = i;}
            }
            const unsigned int tr1 = tid, tr2 = (tid+1)%2;
            const unsigned int t1n1 = (idx[tr1]+1)%3, t2n1 = (idx[tr2]+1)%3;
            const unsigned int t1n2 = (idx[tr1]+2)%3, t2n2 = (idx[tr2]+2)%3;
            a2::Triangle<num_type> ntri[2] = {*tref[0],*tref[1]};
            ntri[tr1].edges[0] = tref[tr1]->edges[t1n2];
            ntri[tr1].dir[0]   = tref[tr1]->dir[t1n2];
            ntri[tr1].edges[1] = tref[tr2]->edges[t2n1];
            ntri[tr1].dir[1]   = tref[tr2]->dir[t2n1];
            ntri[tr1].edges[2] = edge_id;
            ntri[tr1].dir[2]   = false;
            if( edge_map[ntri[tr1].edges[1]].triangles[0] == tref[tr2]->id )  { edge_map[ntri[tr1].edges[1]].triangles[0] = ntri[tr1].id; }
            else                                                              { edge_map[ntri[tr1].edges[1]].triangles[1] = ntri[tr1].id; }
            
            ntri[tr2].edges[0] = tref[tr2]->edges[t2n2];
            ntri[tr2].dir[0]   = tref[tr2]->dir[t2n2];
            ntri[tr2].edges[1] = tref[tr1]->edges[t1n1];
            ntri[tr2].dir[1]   = tref[tr1]->dir[t1n1];
            ntri[tr2].edges[2] = edge_id;
            ntri[tr2].dir[2]   = true;
            if( edge_map[ntri[tr2].edges[1]].triangles[0] == tref[tr1]->id )  { edge_map[ntri[tr2].edges[1]].triangles[0] = ntri[tr2].id; }
            else                                                              { edge_map[ntri[tr2].edges[1]].triangles[1] = ntri[tr2].id; }
            
            edge.vertices[0] = vid;
            edge.vertices[1] = edge_map[ntri[tr1].edges[0]].vertices[ntri[tr1].dir[0]];
            
            *tref[0] = ntri[0];
            *tref[1] = ntri[1];
            
            // perform edge legalization recursively
            legalizeEdge(vid, ntri[tr1].edges[0],
                         xv,yv,edge_map,int_triangles );
            legalizeEdge(vid, ntri[tr2].edges[1],
                         xv,yv,edge_map,int_triangles );
        }
        
    }// end legalizeEdge
    
}


// get rid of macro definitions
#undef HEADER
#undef CLASS

#endif /* delaunay_triangulation_impl_hxx */


