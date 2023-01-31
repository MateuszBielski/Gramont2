/* %PATH% %FILE% */
#ifndef MultiModelManager_H
#define MultiModelManager_H
#include "modelmanager.h"
#include "onemodel.h"
#include "matrixstack.h"

#include "cameratrial.h"

class MultiModelManager : public ModelManager
{
#ifdef TESTOWANIE_F
    friend class MultiModelManagerAccess;
#endif

private:
    spCameraTrial cameraTrial;
    void MakeAndSetCustomModels();
public:
    MultiModelManager(myOGLErrHandler* extErrHnd);
    ~MultiModelManager();
    virtual void SetShadersAndGeometry() override;
    virtual void Draw3d() override;
    virtual void OnMouseRotDragging(int posX, int posY) override;
    virtual void OnMouseLeftDClick(int posX, int posY) override;
    virtual void OnMouseWheel(int rotation) override;
    virtual void OnMouseMiddleClick(int posX, int posY);
    

    void setModels(vector<spOneModel>&& );

protected:
    spMatrixStack m_ptrMatrixStack;
    vector<spOneModel> models;
    spMyOGLShaders ptr_TextureShader;
    char * textureShaderCode = nullptr;

    virtual void setMatricesForRender(upOglRenderer& ) override;
};


#endif // MultiModelManager_H
