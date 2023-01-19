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
void Transformable::MoveOnScreenPlane(int m_mousePrevX,int  m_mousePrevY,int  posX,int  posY,glm::dmat4x4 * dmat4view)
{
     double xdiff = 1.0d * (posX - m_mousePrevX);
    double ydiff = 1.0d * (posY - m_mousePrevY);

    glm::dvec4 moveInCameraCoord(xdiff,ydiff,0.0,0.0);
    glm::dvec4 moveInWorldCoord = 
    inverse(*dmat4view) * 
    moveInCameraCoord;

    Translate(moveInWorldCoord);
//    rotCenter = rotCenter - xyz(moveInWorldCoord);
//    
//    dvec3 newPosition = position;
//    transformation = scale(dmat4x4(1.0),dvec3(m_scale,m_scale,m_scale));
//    newPosition = xyz(transformation * dvec4(position,0.0));
//    
//    transformation = toMat4(q_rotation);
//    newPosition = xyz(transformation * dvec4(newPosition,0.0));
//    dvec3 newCamUp = xyz(transformation * dvec4(camUp,0.0));
//    dvec3 newTarget = xyz(transformation * dvec4(target,0.0));
//    
//    dmat4view = lookAt(newPosition + rotCenter,newTarget + rotCenter,newCamUp);
//    camDistance = glm::distance(newPosition,newTarget);
}
