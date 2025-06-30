//
// Created by sharib masum on 2025-06-21.
//

#include <stdio.h>

#include "gl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef BASICOPENGL_LIGHT_H
#define BASICOPENGL_LIGHT_H


class Light {
public:
    Light();

    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat XDirection, GLfloat YDirection, GLfloat ZDirection, GLfloat dIntensity);

    void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);



    ~Light();

private:
    glm::vec3 colour;
    GLfloat ambientIntensity;

    glm::vec3 direction;
    GLfloat diffuseIntensity;



};


#endif //BASICOPENGL_LIGHT_H
