#include "convexsurface.h"

#include "iostream"
#include <cmath>

ConvexSurface::ConvexSurface(GLuint segmentX,GLuint segmentY,float sizeX,float sizeY, float sizeZ):
Surface(segmentX,segmentY,sizeX,sizeY),m_sizeZ(sizeZ)
{
    CalculateAndSet_ZcoordsToCosShape();
    CalculateResultantNormalForAllPoints();
}
float ConvexSurface::CalculateZtoCosShapeForXY(float x, float y)
{
    float x0 = 0.0f, y0 = 0.0f;
    float x_lim = m_sizeX / 2.0f;
    float y_lim = m_sizeY / 2.0f;
    if(x < x0)x_lim = -x_lim;
    if(y < y0)y_lim = -y_lim;
    
    float xd = (y == 0.0f) ? 0.0f : x * y_lim / y;
    float yd = y_lim;
    if(xd * xd > x_lim * x_lim)
    {
        yd = (x == 0.0f) ? 0.0f : y * x_lim / x;
        xd = x_lim;
    }
    float d = sqrt(x * x + y * y) / sqrt(xd * xd + yd * yd);
    d *=(M_PI);
//    cout<<x<<", "<<y<<", "<<xd<<", "<<yd<<", "<<d<<endl;
    return m_sizeZ*cos(d) / 2;
}
void ConvexSurface::CalculateAndSet_ZcoordsToCosShape()
{
    if(!data.verts)return;
    GLfloat * temp = new GLfloat[data.nuPoints * 3];
    memcpy(temp,data.verts,sizeof(float) * 3 * data.nuPoints);
    float x,y;
    for(int v = 0; v < data.nuPoints ; v++)
    {
        x = temp[v * 3 ];
        y = temp[v * 3 + 1];
        temp[v * 3 + 2] = CalculateZtoCosShapeForXY(x,y);
//        temp[v * 3 + 2] = 0.0f;
    }
    delete [] data.verts;
    data.verts = temp;
}
