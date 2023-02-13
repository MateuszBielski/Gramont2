/* %PATH% %FILE% */
#ifndef CameraTrial_H
#define CameraTrial_H
#include "oglstuff.h"
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

using namespace glm;
struct ScreenMove
{
    int fromX = 0;
    int fromY = 0;
    int toX = 0;
    int toY = 0;
};

using dmat_stack = std::stack<glm::dmat4x4 *>;

class CameraTrial : public myOGLCamera
{
private:
    dquat q_rotation;
    dmat4 transformation;
    dmat4 dmat4view;
    dmat4 dmat4proj;
    dvec3 position;
    dvec3 target;
    dvec3 camUp;
    dvec3 rotCenter;
    double m_scale;
    double camDistance;
public:
    CameraTrial();
    virtual void ViewSizeChanged(int newWidth, int newHeight) override;
    void UpdateViewMatrix();
    void MoveOnScreenPlane(int ,int ,int , int );
    virtual void MouseRotation(int fromX, int fromY, int toX, int toY) override;
    dquat RotationFromScreenMove(ScreenMove& ,bool reverseAngle = false);
    void MoveBackForWard(int );
    virtual void UpdateMatrices() override;//?
    const double * getViewMatrixdv();//?
    const double * getProjMatrixdv();//?
    const double * getModeMatrixdv();//?
    dmat4x4 * getViewGlmMatrixdv();
    dmat4x4 * getProjGlmMatrixdv();
    bool needUpdateViewMat;
    bool needUpdateProjMat;
    bool needUpdateModeMat;
    double getDistance() {return camDistance;}
    glm::dquat getQ_rotationVal() {return q_rotation;}
protected:
};

using spCameraTrial = std::shared_ptr<CameraTrial>;
#endif // CameraTrial_H
