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
    spOglRenderer getRenderer();
    spMyOGLShaders getShader();
protected:
    spOglRenderer m_renderer;
    spMyOGLShaders m_shader;

};
using spRenderSystem = std::shared_ptr<RenderSystem>;
using upRenderSystem = std::unique_ptr<RenderSystem>;
#endif // RenderSystem_H
