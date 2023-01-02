#ifndef Transformable_H
#define Transformable_H

#include <iostream>
#include <memory>
#include <glm/glm.hpp>

using namespace std;

class Transformable
{
private:
    glm::dmat4 modelMatrix;
    float mat4f[16];
    void UpdateFloatMatrix();
public:
    Transformable();
    ~Transformable();
    void Translate(glm::dvec3);
    void Rotate(double, glm::dvec3);
    const float * getModelMatrixfv();
    const double * getModelMatrixdv();
    bool needUpdateModelMat;
protected:


};
using spTransformable = shared_ptr<Transformable>;
using upTransformable = unique_ptr<Transformable>;
#endif // Transformable_H
