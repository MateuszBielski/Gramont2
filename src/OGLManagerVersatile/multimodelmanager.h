/* %PATH% %FILE% */
#ifndef MultiModelManager_H
#define MultiModelManager_H
#include "modelmanager.h"
#include "onemodel.h"
#include "matrixstack.h"
#include "cameratrial.h"
#include "selecting.h"
#include "rendersystemmanager.h"

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
    void RenderSystemSetIfWant();
public:
    MultiModelManager(myOGLErrHandler* extErrHnd);
    ~MultiModelManager();
    void SetShadersAndGeometry() override;
    void SetViewport(int x, int y, int width, int height) override;
    void Draw3d() override;
    void SwitchViewControl() override;
    void OnMouseRotDragging(int posX, int posY) override;
    void OnMouseLeftDClick(int posX, int posY) override;
    void OnMouseWheel(int rotation) override;
    void OnMouseMiddleClick(int posX, int posY);
    

    void setModels(vector<spOneModel>&& );
    void setSelectingResult(SelectingResult&& );
protected:
    void CallForMyRenderable(FunReSys_spOm ,spRenderSystem) override;
    void CallForMyTextures(FunReSys_Tim FunToCall, spRenderSystem rs) override;
    void ConfigureWithMyViewControl(spRenderSystem) override;
    
    spRenderSystemManager m_rs_manager;//czy jest jakaś korzyść z użycia wskaźnika zamiast obiektu? Nie korzyść tylko konieczność do stosowania w metodzie CallForMyRenderable(FunReSys_spOm ,spRenderSystem)
    spMatrixStack m_ptrMatrixStack;
    vector<spOneModel> models;
    vector<spTextureInMemory> texms;
    char * textureShaderCode = nullptr;
    
};

#endif // MultiModelManager_H
