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
    accessed.ptr_PickingShader = shader;
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
