//
// Created by sharib masum on 2025-06-11.
//

#ifndef UDEMYOPENGL_MESH_H
#define UDEMYOPENGL_MESH_H

#include "gl.h"

class Mesh {
public:
    Mesh();

    void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void renderMesh();
    void clearMesh();


    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;


};


#endif //UDEMYOPENGL_MESH_H
