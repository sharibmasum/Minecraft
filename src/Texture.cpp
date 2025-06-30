//
// Created by sharib masum on 2025-06-18.
//

#include "Texture.h"

Texture::Texture() {
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";

}

Texture::Texture(char* fileLoc) {
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = fileLoc;


}

void Texture::loadTexture() {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, STBI_rgb_alpha);

    if (!texData) {
        std::cout<< "Failed to find: " << fileLocation << std::endl;
        return;

    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s means wrapping along x axis (left and right)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // NEAREST for pixelated look
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // NEAREST for pixelated look

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);

}

void Texture::clearTexture() {
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";

}

void Texture::useTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture() {
    clearTexture();
}