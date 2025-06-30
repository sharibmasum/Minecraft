//
// Created by sharib masum on 2025-06-18.
//

#ifndef BASICOPENGL_TEXTURE_H
#define BASICOPENGL_TEXTURE_H

#include <iostream>
#include "gl.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {
public:
    Texture();
    Texture(char* fileLoc);

    void loadTexture();
    void useTexture();
    void clearTexture();


    ~Texture();
private:
    GLuint textureID;

    int width, height, bitDepth;

    char* fileLocation;

};


#endif //BASICOPENGL_TEXTURE_H
