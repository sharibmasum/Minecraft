//
// Created by sharib masum on 2025-06-16.
//

#ifndef BASICOPENGL_CAMERA_H
#define BASICOPENGL_CAMERA_H

#include "gl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed);

    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    glm::mat4 calculateViewMatrix();
    glm::vec3 getCameraPosition();


    ~Camera();

private:
    glm::vec3 position, up, front, right, worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    void update();


};


#endif //BASICOPENGL_CAMERA_H
