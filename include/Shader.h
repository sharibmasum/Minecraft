//
// Created by sharib masum on 2025-06-11.
//

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "gl.h"

#ifndef UDEMYOPENGL_SHADER_H
#define UDEMYOPENGL_SHADER_H


class Shader {
public:
    Shader();

    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFiles(const char* vertexLocation, const char* fragLocation);

    std::string readFile(const char* fileLocation);

    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColourLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getDirectionLocation();
    GLuint getSpecularIntensityLocation();
    GLuint getShininessLocation();
    GLuint getCameraPosition();

    void useShader();
    void clearShader();


    ~Shader();

private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformCameraPosition,
    uniformAmbientIntensity,uniformAmbientColour,
    uniformDiffuseIntensity, uniformDirection,
    uniformSpecularIntensity, uniformShininess
    ;

    void compileShader(const char* vertexCode, const char* fragmentCode);
    void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);



};


#endif //UDEMYOPENGL_SHADER_H
