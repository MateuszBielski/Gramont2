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
void SelectingTestAccess::setShadersMock(myOGLShaders* shad_mock)
{
    accessed.m_pickingShader = shad_mock;
}
void SelectingTestAccess::setShadersLoaded(bool f)
{
    accessed.shadersLoaded = f;
}
