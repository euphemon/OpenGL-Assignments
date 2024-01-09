#include "Animation.h"

Animation::Animation()
{
    this->transformationMatrix = glm::mat4(1.0f);
}

Animation::~Animation()
{
}

void Animation::init()
{
    reset();
}

void Animation::reset()
{
    accumulatedAngleX = accumulatedAngleY = 0.0f;
    presentPos = glm::vec3(5.0f, 0.0f, 0.0f);
    startPos = presentPos;

    matrix3LocalRotation = glm::mat3(1.0f); 
    matrix3WorldRotation = glm::mat3(1.0f);

    transformationMatrix = glm::translate(glm::mat4(1.0f), startPos);
}

void Animation::update(float delta_time)
{
}

void Animation::applyRotationOnLocalAxis()
{
    accumulatedAngleX += angleX;
    
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), presentPos);
    glm::mat4 globalRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(accumulatedAngleY), rotationAxisWorld);
    glm::mat4 localRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(accumulatedAngleX), rotationAxisLocal);
    
    matrix3LocalRotation = glm::mat3(localRotationMat);
    
    transformationMatrix = translationMat * globalRotationMat * localRotationMat;
    modelAfterRotation = transformationMatrix;
}

void Animation::applyRotationOnWorldAxis()
{
    accumulatedAngleY += angleY;
    
    glm::mat3 rotationMat = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(accumulatedAngleY), rotationAxisWorld));
    presentPos = rotationMat * startPos;

    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), presentPos);
    glm::mat4 globalRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(accumulatedAngleY), rotationAxisWorld);
    glm::mat4 localRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(accumulatedAngleX), rotationAxisLocal);
    
    matrix3WorldRotation = rotationMat;
    
    transformationMatrix = translationMat * globalRotationMat * localRotationMat;
    modelAfterRotation = transformationMatrix;
}