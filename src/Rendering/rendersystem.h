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
    virtual bool ConfigureShadersAndLocations() = 0;
    spOglRenderer getRenderer();
    spMyOGLShaders getShader();
    spBufferLoader getBufferLoader();
protected:
    spOglRenderer m_renderer;
    spBufferLoader m_BufferLoader;
    spMyOGLShaders m_shader;

};
using spRenderSystem = std::shared_ptr<RenderSystem>;
using upRenderSystem = std::unique_ptr<RenderSystem>;
#endif // RenderSystem_H
