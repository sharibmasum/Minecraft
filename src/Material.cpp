//
// Created by sharib masum on 2025-06-29.
//

#include "Material.h"

Material::Material() {
specularIntensity= 0;
shininess=0;
}

Material::Material(GLfloat sIntensity, GLfloat shine) {
    specularIntensity= sIntensity;
    shininess=shine;
}

void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}


Material::~Material() {

}