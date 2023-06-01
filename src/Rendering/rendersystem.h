#ifndef RenderSystem_H
#define RenderSystem_H

#include <iostream>
#include <memory>
#include "oglrenderer.h"

class RenderSystem
{
private:

public:
    RenderSystem();
//RenderSystem(spMyOGLShaders,spBufferLoader,spOglRenderer);
    virtual bool ConfigureShadersAndLocations() = 0;
    spOglRenderer getRenderer();
    spMyOGLShaders getShader();
    spBufferLoader getBufferLoader();
    void setBufferLoader(spBufferLoader );
    void setShader(spMyOGLShaders );
#ifdef TESTOWANIE_F
    virtual
#endif
    void ReloadVAO(spOneModel );
    void CreateGraphicBuffers(ModelData& d, TextureForModel& tex);
    void CreateGraphicBuffers(spOneModel );
    void CreateGraphicBuffers(TextureInMemory& texm);
protected:
    spOglRenderer m_renderer;
    spBufferLoader m_BufferLoader;
    spMyOGLShaders m_shader;
    unsigned int * activeVaoPtr = nullptr;

};

using FunReSys_MdTfm = void (RenderSystem::*)(ModelData& d,TextureForModel& tex);
using FunReSys_Tim = void (RenderSystem::*)(TextureInMemory& tex);

using FunReSys_spOm = void (RenderSystem::*)(spOneModel model);

using spRenderSystem = std::shared_ptr<RenderSystem>;
using upRenderSystem = std::unique_ptr<RenderSystem>;
#endif // RenderSystem_H
