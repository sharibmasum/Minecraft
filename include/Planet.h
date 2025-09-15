//
// Created by sharib masum on 2025-07-02.
//

#ifndef MINECRAFT_PLANET_H
#define MINECRAFT_PLANET_H

#include <vector>
#include <unordered_map>
#include "Planet.h"
#include "Chunk.h"
#include "FastNoiseLite.h"

class Planet {
public:
    Planet();
    Planet(glm::vec3 cameraPos);

    std::vector<unsigned int> getChunkData(int chunkX, int chunkY, int chunkZ);


    void update(float camx, float camy, float camz, unsigned int modelLoc);
    void render(GLuint );

    ~Planet();

private:
    std::vector <unsigned int> generateChunkData(int chunkX, int chunkY, int chunkZ);
    std::string getChunkKey(int x, int y, int z);
    Chunk* getChunk(int x, int y, int z);



    std::unordered_map<std::string, Chunk*> chunks;
    int renderDistance = 1;
    unsigned int chunksize = 16;

};

#endif //MINECRAFT_PLANET_H
