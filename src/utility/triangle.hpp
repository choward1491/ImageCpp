//
//  triangle.hpp
//  ImageCpp
//
//  Created by Christian J Howard on 8/21/17.
//  Copyright © 2017 Christian Howard. All rights reserved.
//

#ifndef triangle_h
#define triangle_h

#include <cmath>
#include <iostream>

/*=======================================
 This is a triangle class for use in the
 3D convex hull algorithm. The output will
 be all the triangles that create the
 convex hull, so this is fairly important
 ========================================*/
template< typename T>
class Triangle{
    
public:
    
    // Numeric id for triangle
    int id;
    
    // indices of points that are the
    // vertices of this triangle
    unsigned int vertices[3];
    
    // the normal vector of the triangle
    T normal[3];
    
    // the ids the neighboring triangles
    unsigned int neighbors[3];
    
    
    
    
    /*=======================================
     Null constructor for triangle
     ========================================*/
    Triangle(){
        for (int i = 0; i < 3; i++ ) {
            vertices[i]  = -1;
            normal[i]    = T(0);
            neighbors[i] = -1;
        }
        id = -1;
    }
    
    
    bool hasVertexID(unsigned int id){
        return vertices[0] == id || vertices[1] == id || vertices[2] == id;
    }
    
    
    /*=======================================
     method to generate the normal for the triangle
     ========================================*/
    void makeNormal( const T *x, const T *y, const T *z, int num_pts ){
        
        int i1 = vertices[0];
        int i2 = vertices[1];
        int i3 = vertices[2];
        
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
    
    
    
    bool pointWithinTriangle( T xt, T yt, const T *x, const T *y, int num_pts, double eps = 0.0 ) const{
        int i1 = vertices[0];
        int i2 = vertices[1];
        int i3 = vertices[2];
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
        
        return s >= -eps && t >= -eps && (s + t <= (1.0 + eps));
        
    }
    
    
    /*=======================================
     method to check if this triangle shares
     the edge generated by the input points
     pt1 and pt2
     ========================================*/
    bool shareEdge( int pt1, int pt2 ) const {
        int count = 0;
        for (int i = 0; i < 3; i++) {
            count += (pt1 == vertices[i]) + (pt2 == vertices[i]);
        }
        
        return count == 2;
    }
    
    
    
    T area( const T * x, const T * y, const T * z ){
        int i1 = vertices[0];
        int i2 = vertices[1];
        int i3 = vertices[2];
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
        int i1 = vertices[0];
        int i2 = vertices[1];
        int i3 = vertices[2];
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
        int i1 = vertices[0];
        int i2 = vertices[1];
        int i3 = vertices[2];
        T pt1[] = {x[i1], y[i1], z[i1]};
        T pt2[] = {x[i2], y[i2], z[i2]};
        T pt3[] = {x[i3], y[i3], z[i3]};
        
        std::cout << "Triangle: " << id << "\n";
        std::cout << "( " << pt1[0] <<", "<< pt1[1] << ", " << pt1[2] << " )\n";
        std::cout << "( " << pt2[0] <<", "<< pt2[1] << ", " << pt2[2] << " )\n";
        std::cout << "( " << pt3[0] <<", "<< pt3[1] << ", " << pt3[2] << " )\n\n";
    }
    void print( const T * x, const T * y){
        int i1 = vertices[0];
        int i2 = vertices[1];
        int i3 = vertices[2];
        T pt1[] = {x[i1], y[i1]};
        T pt2[] = {x[i2], y[i2]};
        T pt3[] = {x[i3], y[i3]};
        
        std::cout << "Triangle: " << id << "\n";
        std::cout << "( " << pt1[0] <<", "<< pt1[1] << " )\n";
        std::cout << "( " << pt2[0] <<", "<< pt2[1] << " )\n";
        std::cout << "( " << pt3[0] <<", "<< pt3[1] << " )\n\n";
    }
    
};

#endif /* triangle_h */