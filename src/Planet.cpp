//
// Created by sharib masum on 2025-07-02.
//

#include "Planet.h"
#include "Chunk.h"
#include "iostream"

Planet::Planet(glm::vec3 cameraPos) {
    for (int x = 0; x < renderDistance; x++) {
        for (int y = 0; y < renderDistance; y++) { // Only generate Y >= 0
            for (int z = 0 ; z < renderDistance; z++) {
                std::string key = getChunkKey(x, y, z);
                Chunk* chunk = new Chunk(chunksize);
                chunk->position = glm::vec3(x, y, z) * float(chunksize);
                std::vector<unsigned int> data = generateChunkData(x, y, z);

                chunk->generateChunk(data);
                chunks.emplace(key, chunk);
            }
        }
    }

    // Mesh building pass
    for (int x = 0; x < renderDistance; x++) {
        for (int y = 0; y < renderDistance; y++) { // Only generate Y >= 0
            for (int z = 0 ; z < renderDistance; z++) {
                Chunk* chunk = getChunk(x, y, z);
                Chunk* neighbors[6] = {
                        getChunk(x, y, z - 1), // NORTH
                        getChunk(x + 1, y, z), // EAST
                        getChunk(x, y + 1, z), // UP
                        getChunk(x, y, z + 1), // SOUTH
                        getChunk(x - 1, y, z), // WEST
                        getChunk(x, y - 1, z)  // DOWN
                };
                chunk->buildMeshWithNeighbors(neighbors);
            }
        }
    }
}


std::vector<unsigned int> Planet::generateChunkData(int chunkX, int chunkY, int chunkZ) {
    std::vector<unsigned int> chunkData;
    chunkData.reserve(chunksize * chunksize * chunksize);

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.02f);

    const int maxHeight = 16;

    for (int x = 0; x < chunksize; x++) {
        for (int z = 0; z < chunksize; z++) {
            float rawNoise = noise.GetNoise((chunkX * chunksize + x) * 2.5f, (chunkZ * chunksize + z) * 2.5f);
            int height = static_cast<int>(rawNoise * maxHeight + 10);

            for (int y = 0; y < chunksize; y++) {
                chunkData.push_back(y <= height ? 1 : 0);
            }
        }
    }

    return chunkData;
}


std::string Planet::getChunkKey(int x, int y, int z) {
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}



void Planet::update(float camx, float camy, float camz, unsigned int modelLoc) {
    // will be used later for dynamic chunk loading
}


Chunk* Planet::getChunk(int x, int y, int z) {
    std::string key = getChunkKey(x, y, z);
    auto it = chunks.find(key);
    if (it != chunks.end()) return it->second;
    return nullptr;
}

void Planet::render(GLuint modelLoc) {
    for (auto& pair : chunks) {
        Chunk* chunk = pair.second;

        glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk->position);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        chunk->render();
    }
}

Planet::~Planet() {
    for(auto& pair : chunks) {
        delete pair.second;
    }
    chunks.clear();
}