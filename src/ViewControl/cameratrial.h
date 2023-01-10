/* %PATH% %FILE% */
#ifndef CameraTrial_H
#define CameraTrial_H
#include "oglstuff.h"
#include <iostream>
#include <memory>

using namespace std;

class CameraTrial : public myOGLCamera
{
private:
public:
    virtual void UpdateMatrices() override;
    virtual void ViewSizeChanged(int newWidth, int newHeight) override;
    void UpdatePosition(int ,int ,int , int );
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
