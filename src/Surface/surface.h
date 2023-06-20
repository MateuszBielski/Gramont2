#ifndef Surface_H
#define Surface_H
#include "onemodel.h"
#include <glm/glm.hpp>
#include <string.h> //memcpy
#include <vector>

using std::vector;
using glm::vec2, glm::vec3, glm::mat2x3;

class Surface : public OneModel
{
private:
public:
    Surface(GLuint segmentX,GLuint segmentY,float sizeX,float sizeY);
    ~Surface();
    const vector<GLuint> IndicesAdjacentToPoint(GLuint);
    GLuint PointOppositeTo(GLuint);
    vec3 ResultantNormalOnePoint(const GLuint point,const vector<GLuint> adjacent, const float * verts);
    vec3 ResultantNormalOnePoint(const GLuint point,const vector<GLuint> adjacent);//verts inside
    mat2x3 ResultantTangentAndBitangentOnePoint(const GLuint point, const vector<GLuint> adjacent, const float * texCoord);
//    bool CalculateTangentAndBitangentForAllPoints();
    //idea for future becouse modelData should to have coordinates  <- false
    bool CalculateTangentAndBitangentForAllPointsBasedOn(TextureForModel& );
    void CalculateResultantNormalForAllPoints();
    void SetZcoordinateForOnePoint(GLuint, float);
protected:
    const GLuint m_segmentX,m_segmentY;
    const float m_sizeX,m_sizeY;
};

#endif // Surface_H
