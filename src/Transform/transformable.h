#ifndef Transformable_H
#define Transformable_H

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// using namespace std;

class Transformable
{
private:
    glm::dmat4 modelMatrix;
    glm::dquat q_rotation;
    glm::dmat4 transformation;//?
    glm::dvec3 position;
    float mat4f[16];
    void UpdateFloatMatrix();
public:
    Transformable();
    ~Transformable();
    void Translate(glm::dvec3);
    void Rotate(double, glm::dvec3);
    void MoveOnScreenPlane(int, int, int, int, glm::dmat4x4 *);
    void MouseRotation(glm::dquat);
    const float * getModelMatrixfv();
    const double * getModelMatrixdv();
    bool needUpdateModelMat;
protected:


};
using spTransformable = std::shared_ptr<Transformable>;
using upTransformable = std::unique_ptr<Transformable>;
#endif // Transformable_H
