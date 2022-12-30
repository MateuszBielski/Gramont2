#include "transformable.h"
#include <glm/gtc/type_ptr.hpp>


Transformable::Transformable()
{
    modelMatrix = glm::mat4(1.0f);
}
Transformable::~Transformable()
{
}
const float* Transformable::getModelMatrix()
{
    return glm::value_ptr(modelMatrix);
}
const double* Transformable::getModelMatrixdv()
{
    float * mat4f = glm::value_ptr(modelMatrix);
    for(short i = 0; i < 16 ; i++)
    {
        mat4d[i] = mat4f[i];
    }
    return mat4d;
}
void Transformable::Translate(glm::vec3 translateVector)
{
    modelMatrix = glm::translate(modelMatrix,translateVector);
}
void Transformable::Rotate(float angle, glm::vec3 axis)
{
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
}
