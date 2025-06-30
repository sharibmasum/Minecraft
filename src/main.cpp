#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <vector>

#include "gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

std::vector<Mesh *> meshList;
std::vector<glm::vec3> cubePositions;
std::vector<Shader> shaderList;

Window mainWindow;
Camera camera;

Mesh* pyramid = nullptr;
Mesh* lightBlock = nullptr;

float cubeRotationAngle = 0.0f;
std::vector<glm::vec3> cubeRotationAxes;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brickTexture;
Texture dirtTexture;
Texture lightTexture;

Material shinyMaterial;
Material dullMaterial;


Light mainLight;
glm::vec3 lightPosition (-2.0f, 2.0f, -1.0f);
int numOfCubes = 30;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0, uniformCameraPosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

static const char *vShader = "Shaders/vertexShader.glsl";
static const char *fShader = "Shaders/fragmentShader.glsl";


glm::vec3 getRandomNearbyPosition(float maxDistance) {
    // Ensure rand is seeded once
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    auto randomFloat = [](float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    };

    float x = randomFloat(-maxDistance, maxDistance);
    float y = randomFloat(-maxDistance, maxDistance);
    float z = randomFloat(-maxDistance, maxDistance);

    return glm::vec3(x, y, z);
}

void calcAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticiesCount,
                        unsigned int vLength, unsigned int normalOffset) {
    for (size_t i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 p0(vertices[in0], vertices[in0 + 1], vertices[in0 + 2]);
        glm::vec3 p1(vertices[in1], vertices[in1 + 1], vertices[in1 + 2]);
        glm::vec3 p2(vertices[in2], vertices[in2 + 1], vertices[in2 + 2]);

        glm::vec3 v1 = p1 - p0;
        glm::vec3 v2 = p2 - p0;

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;

    }


    for (size_t i = 0; i < verticiesCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;

    }
}

void createObjects() {
    GLfloat vertices[] = {
            // mesh coords                      texture coords                 normals
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 0.0625, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    GLfloat rectVerticies[] = {
            // mesh coords                      texture coords                 normals
            // front
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0
            1.0f, -1.0f, -1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 1
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 2
            1.0f, 1.0f, -1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f, // 3

            //back
            1.0f, -1.0f, 1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 4
            1.0f, 1.0f, 1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f, // 5
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 6
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 7

            // right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8
            1.0f, -1.0f, 1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 9
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 10
            1.0f, 1.0f, 1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f, // 11

            // left
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12
            -1.0f, -1.0f, 1.0f, 0.333333f, 0.0f, 0.0f, 0.0f, 0.0f, // 13
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 14
            -1.0f, 1.0f, 1.0f, 0.333333f, 1.0f, 0.0f, 0.0f, 0.0f,  // 15

            // bottom
            -1.0f, -1.0f, -1.0f, 0.33333f, 0.0f, 0.0f, 0.0f, 0.0f, // 16
            1.0f, -1.0f, -1.0f, 0.666666f, 0.0f, 0.0f, 0.0f, 0.0f, // 17
            1.0f, -1.0f, 1.0f, 0.666666f, 1.0f, 0.0f, 0.0f, 0.0f, // 18
            -1.0f, -1.0f, 1.0f, 0.333333, 1.0f, 0.0f, 0.0f, 0.0f, // 19

            // top
            -1.0f, 1.0f, -1.0f, 0.666666f, 0.0f, 0.0f, 0.0f, 0.0f, // 20
                1.0f, 1.0f, -1.0f, 1.0, 0.0f, 0.0f, 0.0f, 0.0f, // 21
            -1.0f, 1.0f, 1.0f, 0.666666f, 1.0f, 0.0f, 0.0f, 0.0f, // 22
            1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f, 0.0f, 0.0f // 23
    };

    GLfloat lightBlockVerticies[] = {
            // mesh coords                      texture coords                 normals
            // front
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 2
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 3

            //back
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 4
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 5
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 6
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 7

            // right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 9
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 10
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 11

            // left
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12
            -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 13
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 14
            -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // 15

            // bottom
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 16
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 17
            1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 18
            -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 19

            // top
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 20
            1.0f, 1.0f, -1.0f, 1.0, 0.0f, 0.0f, 0.0f, 0.0f, // 21
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 22
            1.0f, 1.0f, 1.0f, 1.0, 1.0f, 0.0f, 0.0f, 0.0f // 23
    };


    unsigned int rectIndices[] = {
            //face 1 front
            0, 2, 1,
            2, 1, 3,

            //face 2 right
            8, 9, 10,
            10, 11, 9,

            //face 3 up
            20, 21, 22,
            22, 23, 21,

            //face 4 back
            6, 4, 7,
            5, 6, 4,

            //face 5 left
            12, 13, 14,
            14, 15, 13,

            //face 6 bottom
            16, 17, 18,
            18, 16, 19
    };


    unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };


    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    pyramid = new Mesh();
    pyramid->createMesh(vertices, indices, 32, 12);


    lightBlock = new Mesh();
    lightBlock->createMesh(lightBlockVerticies,rectIndices, 192, 36);
    calcAverageNormals(rectIndices, 36, rectVerticies, 192, 8, 5);


    for (int i = 0; i < numOfCubes; ++i) {
        Mesh* obj = new Mesh();
        obj->createMesh(rectVerticies, rectIndices, 192, 36);
        meshList.push_back(obj);

        glm::vec3 randomPos = getRandomNearbyPosition(3.0f);
        cubePositions.push_back(randomPos); // store initial position

        glm::vec3 randomAxis = glm::normalize(getRandomNearbyPosition(1.0f));
        cubeRotationAxes.push_back(randomAxis);
    }
}

void createShader() {
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader); //i dont know waht the arrow is for.
    shaderList.push_back(*shader1);
}

int main() {

    mainWindow = Window();
    mainWindow.initalize();
    glEnable(GL_DEPTH_TEST);


    createObjects();
    createShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.05f);

    brickTexture = Texture("Textures/brick.png");
    brickTexture.loadTexture();
    dirtTexture = Texture("Textures/DirtBlock.png");
    dirtTexture.loadTexture();
    lightTexture = Texture("Textures/redstone-lamp.png");
    lightTexture.loadTexture();


    shinyMaterial = Material(1.0f, 32.0f);
    dullMaterial = Material(0.4f, 4.0f);

    mainLight = Light(1.0f, 1.0f, 1.0f, 0.5f, lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);


    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;


        cubeRotationAngle += 50.0f * deltaTime;  // degrees per second
        if (cubeRotationAngle > 360.0f) cubeRotationAngle -= 360.0f;

        glfwPollEvents();

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() /
                                                       (GLfloat) mainWindow.getBufferHeight(), 0.1f, 100.f);

        glClearColor(94.0f / 250.0f, 198.0f / 250.0f, 242.0f / 250.f,
                     1.0f); // clear the window and set the color to red.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();

        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformAmbientIntensity = shaderList[0].getAmbientIntensityLocation();
        uniformAmbientColour = shaderList[0].getAmbientColourLocation();
        uniformDirection = shaderList[0].getDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].getDiffuseIntensityLocation();
        uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
        uniformShininess = shaderList[0].getShininessLocation();
        uniformCameraPosition = shaderList[0].getCameraPosition();


        mainLight.useLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

        glUniform3f(uniformCameraPosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z); // attaches to the camera pos in frag shader

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        brickTexture.useTexture();

        pyramid->renderMesh();


        model = glm::mat4 (1.0f);

        model = glm::translate(model, glm::vec3(lightPosition));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        lightTexture.useTexture();

        lightBlock->renderMesh();

        for (int a = 0; a < numOfCubes; a++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[a]); // use stored position
            model = glm::rotate(model, glm::radians(cubeRotationAngle), cubeRotationAxes[a]);

            model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            dirtTexture.useTexture();
            dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
            meshList[a]->renderMesh();
        }


        glUseProgram(0);
        mainWindow.swapBuffers();

    }

    return 0;
}
