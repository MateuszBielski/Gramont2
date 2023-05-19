#ifndef MULTIMODELMANAGERACCESS_H
#define MULTIMODELMANAGERACCESS_H
#include "multimodelmanager.h"

class MultiModelManagerAccess
{
public:
    MultiModelManagerAccess(MultiModelManager& m);
    virtual ~MultiModelManagerAccess();
    char * getTextureShaderCode(){return manager.textureShaderCode;}
    spCameraTrial getPtrCameraForTest(){return manager.cameraTrial;}
    spMatrixStack getMatrixStack(){return manager.m_ptrMatrixStack;}
    void setTextureShaderForTest(spMyOGLShaders sh){manager.ptr_TextureShader = sh;}
    void setMatrixStack(spMatrixStack ms){manager.m_ptrMatrixStack = ms;}
    void setTexRenderer(spOglRenderer );
    void setMatricesForTexRender();
    bool CameraDoesViewControl();
    void CameraDoesViewControl(bool f);
    spSelecting getModelSelecting();
    spTransformable getSelectedTransformable();
    void setSelectedTransformable(spTransformable );
private:
    MultiModelManager& manager;
};

#endif // MULTIMODELMANAGERACCESS_H
