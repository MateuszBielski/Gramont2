#include "selecting.h"
#include "textfile.h"
#include "shadersPath.h"

using namespace std;

Selecting::Selecting()
{
    m_pickingRenderer = make_shared<PickingRenderer>();
    ptr_PickingShader = make_shared<myOGLShaders>();
}

Selecting::~Selecting()
{
}
void Selecting::SetVertexShaderPath(string p)
{
//    m_vertexShaderPath = p;
}

void Selecting::SetFragmentShaderPath(string p)
{
//    m_fragmentShaderPath = p;
}
bool Selecting::Init()
{
//    LoadShaders();
//    if(!m_pickingShader)m_pickingShader = new myOGLShaders;
//    if(shadersLoaded)m_pickingShader->Init();
    ptr_PickingShader->AddCode(textFileRead(d_vertexPickingShaderPath),GL_VERTEX_SHADER);
    ptr_PickingShader->AddCode(textFileRead(d_fragmentPickingShaderPath),GL_FRAGMENT_SHADER);
    ptr_PickingShader->Init();
    return false;
}

SelectingResult Selecting::getResult()
{
    return SelectingResult(readyForRendering);
}
void Selecting::LoadShaders()
{
//    m_vertexShader = textFileRead(d_vertexPickingShaderPath);
//    m_fragmentShader = textFileRead(d_fragmentPickingShaderPath);
//    shadersLoaded = (m_vertexShader != nullptr) &&  (m_fragmentShader != nullptr);
    
}
spOglRenderer Selecting::getRenderer()
{
    return m_pickingRenderer;
}
void Selecting::setReadPosition(int posX, int posY)
{
    clickedPosX = posX;
    clickedPosY = posY;
}
