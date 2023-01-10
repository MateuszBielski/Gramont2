/* %PATH% %FILE% */
#ifndef MultiModelManager_H
#define MultiModelManager_H
#include "myoglmanager.h"
#include "onemodel.h"
#include "matrixstack.h"

#include "cameratrial.h"

using namespace std;

class MultiModelManager : public myOGLManager
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
    virtual void OnMouseLeftDClick(int posX, int posY) override;
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
