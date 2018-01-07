/*
 *  edge.hpp
 *
 *  Project: ImageCpp
 *  Author : Christian J Howard
 *  Date   : 1/6/18
 *  Purpose:
 *
 */

#ifndef edge_hpp
#define edge_hpp

template<typename num_type>
class Edge {
public:
    Edge(){
        id = vertices[0] = vertices[1] = triangles[0] = triangles[1] = 0;
    }
    Edge(unsigned int v1_id, unsigned int v2_id){
        vertices[0] = v1_id;
        vertices[1] = v2_id;
        id = triangles[0] = triangles[1] = 0;
    }
    ~Edge() = default;
    
    void setTriangle(int idx, unsigned int tid){
        triangles[idx%2] = tid;
    }
    
    bool hasVertex(unsigned int vid){
        return vertices[0] == vid || vertices[1] == vid;
    }
    
    unsigned int id;
    unsigned int vertices[2];
    unsigned int triangles[2];
};

#endif /* edge_hpp */
