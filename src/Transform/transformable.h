#ifndef Transformable_H
#define Transformable_H

#include <iostream>
#include <memory>
#include <glm/glm.hpp>

using namespace std;

class Transformable
{
private:
    glm::mat4 modelMatrix;// = glm::mat4(1.0f);
    double mat4d[16];
public:
    Transformable();
    ~Transformable();
    void Translate(glm::vec3);
    void Rotate(float, glm::vec3);
    const float * getModelMatrix();
    const double * getModelMatrixdv();
protected:


};
using spTransformable = shared_ptr<Transformable>;
using upTransformable = unique_ptr<Transformable>;
#endif // Transformable_H
