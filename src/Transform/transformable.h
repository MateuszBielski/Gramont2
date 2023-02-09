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
    glm::dmat4 transformation;//?
    glm::dquat q_rotation;
    glm::dvec3 position;
    float mat4f[16];
    void UpdateModelMatrix();
public:
    Transformable();
    ~Transformable();
    void Translate(glm::dvec3);
    void Rotate(double, glm::dvec3);
    void MoveOnScreenPlane(int, int, int, int, glm::dmat4x4 *);
    void MouseRotation(glm::dquat);
    const double * getModelMatrixdv();
    glm::dmat4 * getModelGlmMatrixdv(){return &modelMatrix;}
    bool needUpdateModelMat;
    
    glm::dquat getQ_rotationVal() {return q_rotation;}
    glm::dvec3 getGlmVecPosition() {return position;}
protected:


};
using spTransformable = std::shared_ptr<Transformable>;
using upTransformable = std::unique_ptr<Transformable>;
#endif // Transformable_H
