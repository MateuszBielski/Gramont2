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

