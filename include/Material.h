//
// Created by sharib masum on 2025-06-29.
//

#ifndef MINECRAFT_MATERIAL_H
#define MINECRAFT_MATERIAL_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "gl.h"

class Material {
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);

    ~Material();

    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);


private:
    GLfloat specularIntensity;
    GLfloat shininess;

};


#endif //MINECRAFT_MATERIAL_H
