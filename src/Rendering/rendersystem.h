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
#ifdef TESTOWANIE_F
    virtual
#endif
    void ReloadVAO(ModelData& d, TextureForModel& tex);
    void CreateGraphicBuffers(ModelData& d, TextureForModel& tex);
protected:
    spOglRenderer m_renderer;
    spBufferLoader m_BufferLoader;
    spMyOGLShaders m_shader;

};

using FunReSys = void (RenderSystem::*)(ModelData& d,TextureForModel& tex);
using spRenderSystem = std::shared_ptr<RenderSystem>;
using upRenderSystem = std::unique_ptr<RenderSystem>;
#endif // RenderSystem_H
