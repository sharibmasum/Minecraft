//
// Created by sharib masum on 2025-07-01.
//

#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

#include <vector>
#include "Mesh.h"

class Chunk {
public:
    Chunk(unsigned int chunkSize);

    void generateChunk();
    void render();

    ~Chunk();


private:
    unsigned int chunkSize;

    std::vector<std::vector<std::vector<bool>>> blocks;
    Mesh mesh;

    void buildMesh();
    void addCube(std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices, int x, int y, int z, unsigned int& vertexOffset);

};


#endif //MINECRAFT_CHUNK_H
