//
// Created by sharib masum on 2025-06-11.
//

#include "../include/Shader.h"

Shader::Shader() {
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;

}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode) {
    compileShader(vertexCode, fragmentCode);

}

std::string Shader::readFile(const char *fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesnt exist", fileLocation);
        return "";

    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");

    }
    fileStream.close();
    return content;
}

void Shader::createFromFiles(const char *vertexLocation, const char *fragLocation) {
    std::string vertexString = readFile(vertexLocation);
    std::string fragmentString = readFile(fragLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragCode = fragmentString.c_str();

    compileShader(vertexCode, fragCode);
}

GLuint Shader::getViewLocation() {
    return uniformView;
}

GLuint Shader::getSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation() {
    return uniformShininess;
}


void Shader::compileShader(const char *vertexCode, const char *fragmentCode) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        printf("error shader");
        return;
    }
    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking shader program: %s\n", eLog);

        return;
    }



    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformAmbientColour = glGetUniformLocation(shaderID, "directLight.colour");
    uniformAmbientIntensity = glGetUniformLocation(shaderID, "directLight.ambientIntensity");
    uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directLight.diffuseIntensity");
    uniformDirection = glGetUniformLocation(shaderID, "directLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");
}

void Shader::useShader() {
    glUseProgram(shaderID);
}


GLuint Shader::getAmbientColourLocation() {
    return uniformAmbientColour;
}

GLuint Shader::getAmbientIntensityLocation() {
    return uniformAmbientIntensity;
}

GLuint Shader::getDiffuseIntensityLocation() {
    return uniformDiffuseIntensity;

}

GLuint Shader::getDirectionLocation() {
    return uniformDirection;
}

void Shader::clearShader() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
}

void Shader::addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error shader program \n", eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

GLuint Shader::getCameraPosition() {
    return uniformCameraPosition;

}
GLuint Shader::getProjectionLocation() {
    return uniformProjection;
}

GLuint Shader::getModelLocation() {
    return uniformModel;
}

Shader::~Shader(){
    clearShader();

}