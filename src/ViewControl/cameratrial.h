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
    const double * getViewMatrixdv();
    const double * getProjMatrixdv();
    bool needUpdateViewMat;
    bool needUpdateProjMat;
protected:

};

using spCameraTrial = shared_ptr<CameraTrial>;
#endif // CameraTrial_H
