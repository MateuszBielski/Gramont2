#ifndef ConvexSurface_H
#define ConvexSurface_H
#include "surface.h"

using namespace std;

class ConvexSurface : public Surface
{
private:
   float CalculateZtoCosShapeForXY(float x,float y);
public:
    ConvexSurface(GLuint segmentX, GLuint segmentY, float sizeX, float sizeY, float sizeZ);
    void CalculateAndSet_ZcoordsToCosShape();
protected:
    const float m_sizeZ;
};
#endif // ConvexSurface_H
