//
// Created by sharib masum on 2025-07-01.
//
#include <iostream>

#include "Chunk.h"


const glm::ivec3 directions[6] = {
        {0,  0,  -1},
        {1,  0,  0},
        {0,  1,  0},
        {0,  0,  1},
        {-1, 0,  0},
        {0,  -1, 0}
};


struct Face {
    glm::vec3 positions[4];
    glm::vec2 uvs[4];
    glm::vec3 normal;
};

const float s = 0.5f; // scale for posiitoning

static const Face cubeFaces[6] = {
        // FRONT (-Z)
        {
                { { -s, -s, -s }, {  s, -s, -s }, { -s,  s, -s }, {  s,  s, -s } },
                { { 0, 0 }, { 0.33333, 0 }, { 0, 1 }, { 0.33333, 1 } },
                { 0, 0, -1 }
        },
        // RIGHT (+X)
        {
                { { s, -s, -s }, { s, -s,  s }, { s,  s, -s }, { s,  s,  s } },
                { { 0, 0 }, { 0.33333, 0 }, { 0, 1 }, { 0.33333, 1 } },
                { 1, 0, 0 }
        },
        // TOP (+Y)
        {
                { { -s, s, -s }, {  s, s, -s }, { -s, s,  s }, {  s, s,  s } },
                { { 0.66666, 0 }, { 1, 0 }, { 0.66666, 1 }, { 1, 1 } },
                { 0, 1, 0 }
        },
        // BACK (+Z)
        {
                { { s, -s, s }, { -s, -s, s }, { s,  s, s }, { -s,  s, s } },
                { { 0, 0 }, { 0.33333, 0 }, { 0, 1 }, { 0.33333, 1 } },
                { 0, 0, 1 }
        },
        // LEFT (-X)
        {
                { { -s, -s,  s }, { -s, -s, -s }, { -s,  s,  s }, { -s,  s, -s } },
                { { 0, 0 }, { 0.3333, 0 }, { 0, 1 }, { 0.3333, 1 } },
                { -1, 0, 0 }
        },
        // BOTTOM (-Y)
        {
                { { -s, -s,  s }, {  s, -s,  s }, { -s, -s, -s }, {  s, -s, -s } },
                { { 0.333333, 0 }, { 0.6666666, 0 }, { 0.333333, 1 }, { 0.666666, 1 } },
                { 0, -1, 0 }
        }
};

Chunk::Chunk(unsigned int chunkSize) {
    this->chunkSize = chunkSize;

    blocks.resize(chunkSize,
                  std::vector<std::vector<int>>(chunkSize,
                                                std::vector<int>(chunkSize, 0)));  // 0 = air
}


void Chunk::generateChunk(std::vector<unsigned int>& data) { // determines where the chunk is generated, and where itll render a grass block
    auto start = std::chrono::high_resolution_clock::now();

    int index = 0;
    for (unsigned int x = 0; x < chunkSize; ++x) {
        for (unsigned int y = 0; y < chunkSize; ++y) {
            for (unsigned int z = 0; z < chunkSize; ++z) {
                blocks[x][y][z] = data[index++];
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
   // std::cout << "Chunk creation: " << duration << "[ms]" << std::endl;
}

void Chunk::buildMeshWithNeighbors(Chunk* neighbors[6]) {
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    unsigned int vertexOffset = 0;

    for (unsigned int x = 0; x < chunkSize; x++) {
        for (unsigned int y = 0; y < chunkSize; y++) {
            for (unsigned int z = 0; z < chunkSize; z++) {
                if (blocks[x][y][z]) {
                    addCube(vertices, indices, x, y, z, vertexOffset, neighbors);
                }
            }
        }
    }

    mesh.createMesh(vertices.data(), indices.data(), vertices.size(), indices.size());
}

void Chunk::addCubeFace(int faceIndex, int x, int y, int z,
                        std::vector<GLfloat>& vertices,
                        std::vector<unsigned int>& indices,
                        unsigned int& vertexOffset)
{
    const Face& face = cubeFaces[faceIndex];

    for (int i = 0; i < 4; i++) {
        glm::vec3 pos = face.positions[i] + glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f);
        glm::vec2 uv  = face.uvs[i];
        glm::vec3 n   = face.normal;

        vertices.insert(vertices.end(), {
                pos.x, pos.y, pos.z,
                uv.x, uv.y,
                n.x, n.y, n.z
        });
    }

    indices.insert(indices.end(), {
            vertexOffset + 0,
            vertexOffset + 2,
            vertexOffset + 3,
            vertexOffset + 3,
            vertexOffset + 1,
            vertexOffset + 0
    });

    vertexOffset += 4;
}

void Chunk::addCube(std::vector<GLfloat>& vertices,
                    std::vector<unsigned int>& indices,
                    int x, int y, int z,
                    unsigned int& vertexOffset,
                    Chunk* neighbors[6])
{
    for (int face = 0; face < 6; face++) {
        int nx = x + directions[face].x;
        int ny = y + directions[face].y;
        int nz = z + directions[face].z;

        bool neighborFilled = false;

        // Local block neighbor
        if (nx >= 0 && nx < chunkSize &&
            ny >= 0 && ny < chunkSize &&
            nz >= 0 && nz < chunkSize) {
            neighborFilled = blocks[nx][ny][nz];
        } else {
            // Out-of-bounds, check neighbor chunk
            Chunk* neighbor = neighbors[face];
            if (neighbor) {
                int tx = nx;
                int ty = ny;
                int tz = nz;

                if (tx < 0) tx = chunkSize - 1;
                if (tx >= chunkSize) tx = 0;

                if (ty < 0) ty = chunkSize - 1;
                if (ty >= chunkSize) ty = 0;

                if (tz < 0) tz = chunkSize - 1;
                if (tz >= chunkSize) tz = 0;

                if (tx >= 0 && tx < chunkSize &&
                    ty >= 0 && ty < chunkSize &&
                    tz >= 0 && tz < chunkSize) {
                    neighborFilled = neighbor->blocks[tx][ty][tz];
                }
            }
        }

        if (!neighborFilled) {
            addCubeFace(face, x, y, z, vertices, indices, vertexOffset);
        }
    }
}

void Chunk::render() {
    mesh.renderMesh();
}

Chunk::~Chunk() {
    mesh.clearMesh();
}
