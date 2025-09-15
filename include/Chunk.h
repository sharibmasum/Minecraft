//
// Created by sharib masum on 2025-07-01.
//

#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

#include <vector>
#include "gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

class Chunk {
public:
    Chunk(unsigned int chunkSize);

    void generateChunk(std::vector<unsigned int>& data);
    void render();
    glm::vec3 position;

    void buildMeshWithNeighbors(Chunk* neighbors[6]);

    ~Chunk();
    bool isMeshed = false;

private:
    unsigned int chunkSize;

    std::vector<std::vector<std::vector<int>>> blocks;
    Mesh mesh;

    void addCube(std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices,
                 int x, int y, int z,
                 unsigned int& vertexOffset,
                 Chunk* neighbors[6]);

    void addCubeFace(int faceIndex, int x, int y, int z,
                                std::vector<GLfloat>& vertices,
                                std::vector<unsigned int>& indices,
                                unsigned int& vertexOffset);

};


#endif //MINECRAFT_CHUNK_H
