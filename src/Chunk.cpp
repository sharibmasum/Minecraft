//
// Created by sharib masum on 2025-07-01.
//
#include <iostream>

#include "Chunk.h"

static const GLfloat rectVerticies [] = {
            // mesh coords                      texture coords                 normals
            // front
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0
                    1.0f, -1.0f, -1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 1
                    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 2
                    1.0f, 1.0f, -1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f, // 3

                    //back
                    1.0f, -1.0f, 1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 4
                    1.0f, 1.0f, 1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f, // 5
                    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 6
                    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 7

                    // right
                    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8
                    1.0f, -1.0f, 1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 9
                    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 10
                    1.0f, 1.0f, 1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f, // 11

                    // left
                    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12
                    -1.0f, -1.0f, 1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 13
                    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 14
                    -1.0f, 1.0f, 1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f,  // 15

                    // bottom
                    -1.0f, -1.0f, -1.0f, 0.33333f, 0.0f, 0.0f, 0.0f, 0.0f, // 16
                    1.0f, -1.0f, -1.0f, 0.666666f, 0.0f, 0.0f, 0.0f, 0.0f, // 17
                    1.0f, -1.0f, 1.0f, 0.666666f, 1.0f, 0.0f, 0.0f, 0.0f, // 18
                    -1.0f, -1.0f, 1.0f, 0.333333, 1.0f, 0.0f, 0.0f, 0.0f, // 19

                    // top
                    -1.0f, 1.0f, -1.0f, 0.666666f, 0.0f, 0.0f, 0.0f, 0.0f, // 20
                    1.0f, 1.0f, -1.0f, 1.0, 0.0f, 0.0f, 0.0f, 0.0f, // 21
                    -1.0f, 1.0f, 1.0f, 0.666666f, 1.0f, 0.0f, 0.0f, 0.0f, // 22
                    1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f, 0.0f, 0.0f // 23
};
static const unsigned int rectIndices[] = {
        // face 1 - front
        0, 2, 1,
        1, 2, 3,

        // face 2 - right
        8, 10, 9,
        10, 11, 9,

        // face 3 - top
        20, 22, 21,
        22, 23, 21,

        // face 4 - back
        4, 5, 6,
        6, 7, 4,

        // face 5 - left
        12, 13, 14,
        14, 13, 15,

        // face 6 - bottom
        16, 17, 18,
        18, 19, 16
};


Chunk::Chunk(unsigned int chunkSize) {
    this->chunkSize = chunkSize;
    blocks.resize(chunkSize, std::vector<std::vector<bool>>(chunkSize, std::vector<bool>(chunkSize, false)));

}

void Chunk::generateChunk() { // determines where the chunk is generated, and where itll render a grass block
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned int x = 0; x < chunkSize; ++x) {
        for (unsigned int y = 0; y < chunkSize; ++y) {
            for (unsigned int z = 0; z < chunkSize; ++z) {
                if (y < chunkSize / 2) {
                    blocks[x][y][z] = true; // grass
                } else {
                    blocks[x][y][z] = false; // air
                }
            }
        }
    }

    buildMesh();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Chunk creation: " << duration << "[ms]" << std::endl;
}

void Chunk::buildMesh() {
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    unsigned int vertexOffset = 0;

    for (unsigned int x = 0; x < chunkSize; x++) {
        for (unsigned int y = 0; y < chunkSize; y++) {
            for (unsigned int z = 0; z < chunkSize; z++) {
                if (blocks[x][y][z]) {
                    addCube(vertices, indices, x, y, z, vertexOffset);
                }
            }
        }
    }

    mesh.createMesh(vertices.data(), indices.data(), vertices.size(), indices.size() );
}



void Chunk::addCube(std::vector<GLfloat> &vertices,
                    std::vector<unsigned int> &indices,
                     int x, int y, int z,
                    unsigned int &vertexOffset)
{
    // Copy all 24 vertices
    for (int i = 0; i < 24; i++) {
        GLfloat px = rectVerticies[i * 8 + 0] * 0.5f + x + 0.5f;
        GLfloat py = rectVerticies[i * 8 + 1] * 0.5f + y + 0.5f;
        GLfloat pz = rectVerticies[i * 8 + 2] * 0.5f + z + 0.5f;

        GLfloat u  = rectVerticies[i * 8 + 3];
        GLfloat v  = rectVerticies[i * 8 + 4];

        GLfloat nx = rectVerticies[i * 8 + 5];
        GLfloat ny = rectVerticies[i * 8 + 6];
        GLfloat nz = rectVerticies[i * 8 + 7];

        vertices.insert(vertices.end(), { px, py, pz, u, v, nx, ny, nz });
    }

    // Add indices with offset
    for (int i = 0; i < 36; i++) {
        indices.push_back(rectIndices[i] + vertexOffset);
    }

    vertexOffset += 24;
}

void Chunk::render() {
    mesh.renderMesh();
}

Chunk::~Chunk(){
    mesh.clearMesh();
}
