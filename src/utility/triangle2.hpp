/*
 *  triangle2.hpp
 *
 *  Project: ImageCpp
 *  Author : Christian J Howard
 *  Date   : 1/6/18
 *  Purpose:
 *
 */

#ifndef triangle2_hpp
#define triangle2_hpp

#include "edge.hpp"
#include "triangle.hpp"
#include "math_bundle.hpp"

namespace a2 {
    template< typename T>
    class Triangle{
        
    public:
        
        // Numeric id for triangle
        int id;
        
        // indices of points that are the
        // vertices of this triangle
        unsigned int edges[3];
        bool dir[3];
        std::map<unsigned int, Edge<T>>* emap;
        
        // the normal vector of the triangle
        T normal[3];
        
        
        
        /*=======================================
         Null constructor for triangle
         ========================================*/
        Triangle(){
            for (int i = 0; i < 3; i++ ) {
                normal[i]       = T(0);
                edges[i]        = 0;
                dir[i]          = false;
            }
            id = -1;
        }
        
        void setEdgeMap( std::map<unsigned int, Edge<T>> & em){
            emap = &em;
        }
        
        bool isWithinCircumcircle( T x, T y, const T *xv, const T *yv ){
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            
            aSDG::math::mat<3,3> A;
            T d1[] = { xv[i1] - x, yv[i1] - y };
            T d2[] = { xv[i2] - x, yv[i2] - y };
            T d3[] = { xv[i3] - x, yv[i3] - y };
            A(0,0) = d1[0]; A(0,1) = d1[1]; A(0,2) = d1[0]*d1[0] + d1[1]*d1[1];
            A(1,0) = d2[0]; A(1,1) = d2[1]; A(1,2) = d2[0]*d2[0] + d2[1]*d2[1];
            A(2,0) = d3[0]; A(2,1) = d3[1]; A(2,2) = d3[0]*d3[0] + d3[1]*d3[1];
            
            return A.determinant() > 0.0;
        }
        
        ::Triangle<T> getRawTriangle(){
            ::Triangle<T> tri;
            tri.id = id;
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            for(int i = 0; i < 3; ++i){
                tri.normal[i] = normal[i];
                if( edges_[i]->triangles[0] == id ){
                    tri.neighbors[i] = edges_[i]->triangles[1];
                }else{
                    tri.neighbors[i] = edges_[i]->triangles[0];
                }
            }
            tri.vertices[0] = edges_[0]->vertices[dir[0]];
            tri.vertices[1] = edges_[0]->vertices[(dir[0]+1)%2];
            tri.vertices[2] = edges_[1]->vertices[(dir[1]+1)%2];
            
            return tri;
        }
        
        bool hasVertexID(unsigned int id) const{
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            return edges_[0]->vertices[dir[0]] == id || edges_[0]->vertices[(dir[0]+1)%2] == id || edges_[1]->vertices[(dir[1]+1)%2] == id;
        }
        
        
        /*=======================================
         method to generate the normal for the triangle
         ========================================*/
        void makeNormal( const T *x, const T *y, const T *z, int num_pts ){
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            
            T u[] = { x[i2] - x[i1], y[i2] - y[i1], z[i2] - z[i1] };
            T v[] = { x[i3] - x[i1], y[i3] - y[i1], z[i3] - z[i1] };
            
            T N[3];
            N[0] = u[1]*v[2] - u[2]*v[1];
            N[1] = u[2]*v[0] - u[0]*v[2];
            N[2] = u[0]*v[1] - u[1]*v[0];
            T mag = sqrt( N[0]*N[0] + N[1]*N[1] + N[2]*N[2] );
            
            normal[0] = N[0] / mag;
            normal[1] = N[1] / mag;
            normal[2] = N[2] / mag;
        }
        
        
        
        bool pointWithinTriangle( T xt, T yt, const T *x, const T *y, int num_pts ) const{
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            T pt1[] = {x[i1], y[i1]};
            T pt2[] = {x[i2], y[i2]};
            T pt3[] = {x[i3], y[i3]};
            /*
            std::cout << "[ " << xt << ", " << yt << " ]\n";
            std::cout << "( " << pt1[0] <<", "<< pt1[1] << " )\n";
            std::cout << "( " << pt2[0] <<", "<< pt2[1] << " )\n";
            std::cout << "( " << pt3[0] <<", "<< pt3[1] << " )\n\n";
             */
            
            T dx = xt - x[i1], dy = yt - y[i1];
            T dx1= x[i2] - x[i1], dy1 = y[i2] - y[i1];
            T dx2= x[i3] - x[i1], dy2 = y[i3] - y[i1];
            T detA = dx1*dy2 - dx2*dy1;
            T s = (dy2*dx - dx2*dy)/detA;
            T t = (-dy1*dx + dx1*dy)/detA;
            
            return s >= 0.0 && t >= 0.0 && (s + t <= 1.0);
            
        }
        
        
        /*=======================================
         method to check if this triangle shares
         the edge generated by the input points
         pt1 and pt2
         ========================================*/
        bool shareEdge( int pt1, int pt2 ) const {
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int count = 0;
            for (int i = 0; i < 3; i++) {
                count += (edges_[i]->hasVertex(pt1) && edges_[i]->hasVertex(pt2));
            }
            
            return count >= 1;
        }
        
        bool shareEdge( unsigned int edge_id ) const {
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int count = 0;
            for (int i = 0; i < 3; i++) {
                count += edge_id == edges_[i]->id;
            }
            
            return count >= 1;
        }
        
        
        
        T area( const T * x, const T * y, const T * z ){
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            T u[] = {x[i1], y[i1], z[i1]};
            T v[] = {x[i2], y[i2], z[i2]};
            T w[] = {x[i3], y[i3], z[i3]};
            T x1[3] = {0}, x2[3] = {0}, cross[3] = {0};
            
            for(int i = 0; i < 3; ++i){
                x1[i] = v[i]-u[i];
                x2[i] = w[i]-u[i];
            }
            
            cross[0] = x1[2]*x2[1] - x1[1]*x2[2];
            cross[1] = x1[2]*x2[0] - x1[0]*x2[2];
            cross[2] = x1[0]*x2[1] - x1[1]*x2[0];
            
            return 0.5*std::sqrt(cross[0]*cross[0] + cross[1]*cross[1] + cross[2]*cross[2]);
        }
        
        T area( const T * x, const T * y ){
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            T u[] = {x[i1], y[i1], 1.0};
            T v[] = {x[i2], y[i2], 1.0};
            T w[] = {x[i3], y[i3], 1.0};
            T x1[3] = {0}, x2[3] = {0}, cross[3] = {0};
            
            for(int i = 0; i < 3; ++i){
                x1[i] = v[i]-u[i];
                x2[i] = w[i]-u[i];
            }
            
            cross[0] = x1[2]*x2[1] - x1[1]*x2[2];
            cross[1] = x1[2]*x2[0] - x1[0]*x2[2];
            cross[2] = x1[0]*x2[1] - x1[1]*x2[0];
            
            return 0.5*std::sqrt(cross[0]*cross[0] + cross[1]*cross[1] + cross[2]*cross[2]);
        }
        
        
        /*=======================================
         method to print the triangle inputs
         ========================================*/
        void print( const T * x, const T * y, const T * z ){
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            T pt1[] = {x[i1], y[i1], z[i1]};
            T pt2[] = {x[i2], y[i2], z[i2]};
            T pt3[] = {x[i3], y[i3], z[i3]};
            
            std::cout << "Triangle: " << id << "\n";
            std::cout << "( " << pt1[0] <<", "<< pt1[1] << ", " << pt1[2] << " )\n";
            std::cout << "( " << pt2[0] <<", "<< pt2[1] << ", " << pt2[2] << " )\n";
            std::cout << "( " << pt3[0] <<", "<< pt3[1] << ", " << pt3[2] << " )\n\n";
        }
        void print( const T * x, const T * y){
            const Edge<T>* edges_[3] = {&(*emap)[edges[0]],&(*emap)[edges[1]],&(*emap)[edges[2]]};
            int i1 = edges_[0]->vertices[dir[0]];
            int i2 = edges_[0]->vertices[(dir[0]+1)%2];
            int i3 = edges_[1]->vertices[(dir[1]+1)%2];
            T pt1[] = {x[i1], y[i1]};
            T pt2[] = {x[i2], y[i2]};
            T pt3[] = {x[i3], y[i3]};
            
            std::cout << "Triangle: " << id << "\n";
            std::cout << "( " << pt1[0] <<", "<< pt1[1] << " )\n";
            std::cout << "( " << pt2[0] <<", "<< pt2[1] << " )\n";
            std::cout << "( " << pt3[0] <<", "<< pt3[1] << " )\n\n";
        }
        
    };
}

#endif /* triangle2_hpp */
