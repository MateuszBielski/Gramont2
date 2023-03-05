#include "selectingtestaccess.h"

using namespace std;

SelectingTestAccess::SelectingTestAccess(Selecting& ac):accessed(ac)
{

}

void SelectingTestAccess::SetReadyForRendering(bool fl)
{
    accessed.readyForRendering = fl;
}
bool SelectingTestAccess::ShadersLoaded()
{

    return accessed.shadersLoaded;
}
void SelectingTestAccess::setShader(spMyOGLShaders shader)
{
    accessed.m_pickingShader = shader;
}
void SelectingTestAccess::setShadersLoaded(bool f)
{
    accessed.shadersLoaded = f;
}

int SelectingTestAccess::getClickedPosX()
{
    return accessed.clickedPosX;
}

int SelectingTestAccess::getClickedPosY()
{
    return accessed.clickedPosY;
}
unsigned int SelectingTestAccess::getWindowHeight()
{
    return accessed.WindowHeight;
}
unsigned int SelectingTestAccess::getWindowWidth()
{
    return accessed.WindowWidth;
}
bool SelectingTestAccess::Inited()
{
    return accessed.inited;
}
void SelectingTestAccess::setNeedUpdateFrameBuffer(bool f)
{
	accessed.needUpdateFrameBuffer = f;
}
bool SelectingTestAccess::needUpdateFrameBuffer()
{
    return accessed.needUpdateFrameBuffer;
}
bool SelectingTestAccess::FrameBufferUpdated()
{
	return accessed.frameBufferUpdated;
}
void SelectingTestAccess::setFramebufferUpdated(bool f)
{
	accessed.frameBufferUpdated = f;
}
void SelectingTestAccess::UpdateFrameBuffer()
{
    return accessed.UpdateFrameBuffer();
}
void SelectingTestAccess::setFBO(unsigned int n)
{
	accessed.m_fbo = n;
}

