/* %PATH% %FILE% */
#ifndef CameraTrial_H
#define CameraTrial_H
#include "oglstuff.h"
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class CameraTrial : public myOGLCamera
{
private:
    dquat q_rotation;
    dquat q_rotationDiff;
public:
    CameraTrial();
    virtual void UpdateMatrices() override;
    virtual void ViewSizeChanged(int newWidth, int newHeight) override;
    void UpdatePosition(int ,int ,int , int );
    virtual void MouseRotation(int fromX, int fromY, int toX, int toY) override;
    const double * getViewMatrixdv();
    const double * getProjMatrixdv();
    const double * getModeMatrixdv();
    bool needUpdateViewMat;
    bool needUpdateProjMat;
    bool needUpdateModeMat;
protected:
};

using spCameraTrial = shared_ptr<CameraTrial>;
#endif // CameraTrial_H
