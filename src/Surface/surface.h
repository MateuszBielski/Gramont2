#ifndef Surface_H
#define Surface_H
#include "onemodel.h"
#include <glm/glm.hpp>
#include <string.h> //memcpy
#include <vector>

// using namespace std;

class Surface : public OneModel
{
private:
public:
    Surface(GLuint segmentX,GLuint segmentY,float sizeX,float sizeY);
    ~Surface();
    const vector<GLuint> IndicesAdjacentToPoint(GLuint);
    glm::vec3 ResultantNormalOnePoint(const GLuint point,const vector<GLuint> adjacent, const float * verts);//to move verts inside
    glm::vec3 ResultantNormalOnePoint(const GLuint point,const vector<GLuint> adjacent);//to move verts inside
    void CalculateResultantNormalForAllPoints();
    void SetZcoordinateForOnePoint(GLuint, float);
protected:
    const GLuint m_segmentX,m_segmentY;
    const float m_sizeX,m_sizeY;
};

#endif // Surface_H
