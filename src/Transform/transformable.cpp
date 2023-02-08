#include "transformable.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vec_swizzle.hpp>

using namespace glm;

Transformable::Transformable():q_rotation(1.0,0.0,0.0,0.0)
{
    modelMatrix = glm::dmat4(1.0f);
//    q_rotation.w = 1.0f;
}
Transformable::~Transformable()
{
}
const double* Transformable::getModelMatrixdv()
{
    return glm::value_ptr(modelMatrix);
}
//const float* Transformable::getModelMatrixfv()
//{
//    UpdateFloatMatrix();
//    return mat4f;
//}
void Transformable::Translate(glm::dvec3 translateVector)
{
    position += translateVector;
    UpdateModelMatrix();
}
void Transformable::Rotate(double angle, glm::dvec3 axis)
{
    q_rotation = angleAxis(glm::radians(angle), axis) * q_rotation;
    UpdateModelMatrix();
}
void Transformable::UpdateModelMatrix()
{
    modelMatrix = dmat4x4(1.0);
    modelMatrix= translate(modelMatrix,position);
    modelMatrix= modelMatrix * glm::toMat4(q_rotation);
}
void Transformable::MoveOnScreenPlane(int m_mousePrevX,int  m_mousePrevY,int  posX,int  posY,glm::dmat4x4 * dmat4view)
{
    double xdiff = 1.0f * (posX - m_mousePrevX);
    double ydiff = 1.0f * (posY - m_mousePrevY);

    glm::dvec4 moveInCameraCoord(xdiff,ydiff,0.0,0.0);
    glm::dvec4 moveInWorldCoord =
        inverse(*dmat4view) *
        moveInCameraCoord;
        
    position += xyz(moveInWorldCoord);
    
    UpdateModelMatrix();
}
void Transformable::MouseRotation(dquat q_diff)
{
    q_rotation = q_diff * q_rotation;

    UpdateModelMatrix();
}
