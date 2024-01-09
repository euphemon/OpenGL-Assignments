#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Animation
{
public:
    Animation();
    ~Animation();

    int angleX;
    int angleY;
    float accumulatedAngleX = 0;
    float accumulatedAngleY = 0;
    glm::vec3 rotationAxisLocal = glm::vec3(1, 0, 0);
    glm::vec3 rotationAxisWorld = glm::vec3(0, 1, 0);
    glm::vec3 startPos = glm::vec3(5.0, 0.0, 0.0);
    glm::vec3 presentPos = startPos;
    glm::mat3 matrix3Current = glm::mat3();
    glm::mat3 matrix3LocalRotation = glm::mat3();
    glm::mat3 matrix3WorldRotation = glm::mat3();
    glm::mat4 modelAfterRotation;

    void init();
    void update(float delta_time);
    void applyRotationOnLocalAxis();
    void applyRotationOnWorldAxis();
    void reset();
    glm::mat4 get_model_mat() { return transformationMatrix; };

private:
    glm::mat4 transformationMatrix;
};
