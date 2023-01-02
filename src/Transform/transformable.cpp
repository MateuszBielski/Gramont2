#include "transformable.h"
#include <glm/gtc/type_ptr.hpp>


Transformable::Transformable()
{
    modelMatrix = glm::dmat4(1.0d);
}
Transformable::~Transformable()
{
}
const double* Transformable::getModelMatrixdv()
{
    return glm::value_ptr(modelMatrix);
}
const float* Transformable::getModelMatrixfv()
{
    UpdateFloatMatrix();
    return mat4f;
}
void Transformable::Translate(glm::dvec3 translateVector)
{
    modelMatrix = glm::translate(modelMatrix,translateVector);
    UpdateFloatMatrix();
}
void Transformable::Rotate(double angle, glm::dvec3 axis)
{
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
    UpdateFloatMatrix();
}
void Transformable::UpdateFloatMatrix()
{
    double * mat4d = glm::value_ptr(modelMatrix);
    for(short i = 0; i < 16 ; i++) {
        mat4f[i] = mat4d[i];
    }
}
