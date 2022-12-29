#include "selecting.h"
#include "textfile.h"

Selecting::Selecting()
{
}

Selecting::~Selecting()
{
	if(m_pickingShader)delete m_pickingShader;
}
void Selecting::SetVertexShaderPath(string p)
{
    m_vertexShaderPath = p;
}

void Selecting::SetFragmentShaderPath(string p)
{
    m_fragmentShaderPath = p;
}
bool Selecting::Init()
{
    LoadShaders();
    if(!m_pickingShader)m_pickingShader = new myOGLShaders;
    if(shadersLoaded)m_pickingShader->Init();
    return false;
}

SelectingResult Selecting::getResult()
{
    return SelectingResult(readyForRendering);
}
void Selecting::LoadShaders()
{
    m_vertexShader = textFileRead(m_vertexShaderPath.c_str());
    m_fragmentShader = textFileRead(m_fragmentShaderPath.c_str());
    shadersLoaded = (m_vertexShader != nullptr) &&  (m_fragmentShader != nullptr);
}
