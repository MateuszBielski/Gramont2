#ifndef RenderSystem_H
#define RenderSystem_H

#include <iostream>
#include <memory>
#include "onemodel.h"
//#include "textureformodel.h"
#include "bufferloader.h"
#include "matrixstack.h"
//#include "oglstuff.h"
#include "cameratrial.h"

class RenderSystem
{
private:

public:
    RenderSystem();
    virtual bool ConfigureShadersAndLocations() = 0;
    virtual void Draw(spOneModel ) = 0;
    spMyOGLShaders getShader();
    spBufferLoader getBufferLoader();
    void setBufferLoader(spBufferLoader );
    void setShader(spMyOGLShaders );
    
    spMatrixStack matrixStack;
    spCameraTrial camera;
    myLight * light = nullptr;
#ifdef TESTOWANIE_F
    virtual
#endif
    void ReloadVAO(spOneModel );
    void CreateVAO(spOneModel);
    virtual void LoadVAO(spOneModel ) {};
    virtual void CreateGraphicBuffers(spOneModel ) {};
    void CreateGraphicBuffers(TextureInMemory& texm);
    virtual void CheckModelWasConnected(spOneModel ) {}
protected:
    spBufferLoader m_BufferLoader;
    spMyOGLShaders m_shader;
    
    unsigned int * activeVaoPtr = nullptr;
    unsigned int getProgramId() {return m_shader->getProgramId();}
    void InitShadersAndReadLocations(string& ,const vector<string>& ,unsigned int * loc);
    void DrawIndicesAndFinish(ModelData& d);

};

//using FunReSys_MdTfm = void (RenderSystem::*)(ModelData& d,TextureForModel& tex);
using FunReSys_Tim = void (RenderSystem::*)(TextureInMemory& tex);

using FunReSys_spOm = void (RenderSystem::*)(spOneModel model);

using spRenderSystem = std::shared_ptr<RenderSystem>;
using upRenderSystem = std::unique_ptr<RenderSystem>;
#endif // RenderSystem_H
