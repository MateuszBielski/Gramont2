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
    void setTextureShaderForTest(spMyOGLShaders sh){manager.ptr_TextureShader = sh;}
    void setMatrixStack(spMatrixStack ms){manager.m_ptrMatrixStack = ms;}
    void setTexRenderer(upOglRenderer );
    template<typename T>
    unique_ptr<T> getTexRenderer();
private:
    MultiModelManager& manager;
};

#endif // MULTIMODELMANAGERACCESS_H
