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
    const char * vertCode = textFileRead(d_vertexPickingShaderPath);
    const char * fragCode = textFileRead(d_fragmentPickingShaderPath);
    if(!vertCode || !fragCode)return false;

    ptr_PickingShader->AddCode(vertCode,GL_VERTEX_SHADER);
    ptr_PickingShader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    ptr_PickingShader->AddAttrib("Position");
    ptr_PickingShader->AddUnif("gDrawIndex");
    ptr_PickingShader->AddUnif("gObjectIndex");
    ptr_PickingShader->AddUnif("gWVP");
    string nameOfFunction = typeid(*this).name();
    nameOfFunction +="::";
    nameOfFunction += __FUNCTION__;
    ptr_PickingShader->Init(nameOfFunction);
    return true;
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
