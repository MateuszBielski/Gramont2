/* %PATH% %FILE% */
#ifndef MultiModelManager_H
#define MultiModelManager_H
#include "modelmanager.h"
#include "onemodel.h"
#include "matrixstack.h"
#include "cameratrial.h"
#include "selecting.h"

class MultiModelManager : public ModelManager
{
#ifdef TESTOWANIE_F
    friend class MultiModelManagerAccess;
#endif

private:
    spCameraTrial cameraTrial;
    spSelecting m_selecting;
    spTransformable selectedTransformable;
    void MakeAndSetCustomModels();
    bool ConfigureTextureShader();
public:
    MultiModelManager(myOGLErrHandler* extErrHnd);
    ~MultiModelManager();
    virtual void SetShadersAndGeometry() override;
    virtual void SetViewport(int x, int y, int width, int height) override;
    virtual void Draw3d() override;
    virtual void SwitchViewControl() override;
    virtual void OnMouseRotDragging(int posX, int posY) override;
    virtual void OnMouseLeftDClick(int posX, int posY) override;
    virtual void OnMouseWheel(int rotation) override;
    virtual void OnMouseMiddleClick(int posX, int posY);
    

    void setModels(vector<spOneModel>&& );
    void setSelectingResult(SelectingResult&& );
protected:
    spMatrixStack m_ptrMatrixStack;
    vector<spOneModel> models;
    spMyOGLShaders ptr_TextureShader;
    char * textureShaderCode = nullptr;
};


#endif // MultiModelManager_H
