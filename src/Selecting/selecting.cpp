#include "selecting.h"
#include "textfile.h"
#include "shadersPath.h"

using namespace std;

Selecting::Selecting()
{
    m_pickingRenderer = make_shared<PickingRenderer>();
    m_pickingShader = make_shared<myOGLShaders>();
    m_pickingBuffLoader = make_shared<PickingBuffLoader>();
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
    vertCode = textFileRead(d_vertexPickingShaderPath);
    fragCode = textFileRead(d_fragmentPickingShaderPath);

    if(!ConfigurePickingShader()) return false;

    string nameOfFunction = typeid(*this).name();
    nameOfFunction +="::";
    nameOfFunction += __FUNCTION__;
    m_pickingShader->Init(nameOfFunction);

    CreateAndLoadFrameBuffer();
    m_pickingRenderer->setLocationsFrom(m_pickingShader);
    m_pickingBuffLoader->setLocationsFrom(m_pickingShader);
    inited = true;
    return true;
}

SelectingResult Selecting::getResult()
{
    return SelectingResult(readyForRendering);
}
spBufferLoader Selecting::getBufferLoader()
{
    return m_pickingBuffLoader;
}
spOglRenderer Selecting::getRenderer()
{
    return m_pickingRenderer;
}
spMyOGLShaders Selecting::getShader()
{
    return m_pickingShader;
}
void Selecting::setWindowSize(unsigned int w, unsigned int h)
{
    if(WindowWidth == w
       && WindowHeight == h
       && frameBufferUpdated) {
        needUpdateFrameBuffer = false;
        return;
    }
    WindowWidth = w;
    WindowHeight = h;
    needUpdateFrameBuffer = true;
}
void Selecting::setReadPosition(int posX, int posY)
{
    clickedPosX = posX;
    clickedPosY = posY;
}
void Selecting::EnableWritingToFrameBuffer()
{
    UpdateFrameBuffer();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Selecting::DisableWritingToFrameBuffer()
{
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
void Selecting::CreateAndLoadFrameBuffer()
{
    //całość można przenieść do buffLoadera;
    std::string str_log;
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create the texture object for the primitive information buffer
    glGenTextures(1, &m_pickingTexture);
    glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight,
                 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_pickingTexture, 0);

    // Create the texture object for the depth buffer
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           m_depthTexture, 0);

    // Disable reading to avoid problems with older GPUs
    glReadBuffer(GL_NONE);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Verify that the FBO is correct
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        str_log = "Selecting::LoadFrameBuffer error ";
        str_log += Status;
        return;
    }

    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    return GLCheckError();
    str_log = "FrameBufferLoaded";
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
}
void Selecting::UpdateFrameBuffer()
{
    if(!needUpdateFrameBuffer || !m_fbo) {
        frameBufferUpdated = false;
        return;
    }
    glDeleteTextures(1, &m_pickingTexture);
    glDeleteTextures(1, &m_depthTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_pickingTexture);
    glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight,
                    0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_pickingTexture, 0);
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
                    0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           m_depthTexture, 0);
    // Disable reading to avoid problems with older GPUs
    glReadBuffer(GL_NONE);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Verify that the FBO is correct
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        string str_log = "Selecting::UpdateFrameBuffer error ";
        str_log += Status;
        MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
        return;
    }

    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    needUpdateFrameBuffer = false;
    frameBufferUpdated = true;
}
void Selecting::RegisterSelectable(vector<spSelectable>&& selectables)
{
    registeredForSelection = selectables;
    int count = registeredForSelection.size();
    int counter = 0;
    for(counter; counter < count; counter++) {
        selectables[counter]->setUniqueId(counter);
    }
}

Selecting::PixelInfo Selecting::ReadPixel(unsigned int x, unsigned int y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    PixelInfo Pixel;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &Pixel);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    string str_log = to_string(Pixel.ObjectID) + " " +
                     to_string(Pixel.DrawID) + " " +
                     to_string(Pixel.PrimID);
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());

    return Pixel;
}
bool Selecting::ConfigurePickingShader()
{
    if(!vertCode || !fragCode)return false;
    /*
    m_pickingShader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_pickingShader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    m_pickingShader->AddAttrib("Position");
    m_pickingShader->AddUnif("gDrawIndex");
    m_pickingShader->AddUnif("gObjectIndex");
    m_pickingShader->AddUnif("gWVP");
    */

    m_pickingShader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_pickingShader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    m_pickingShader->AddAttrib("in_sPosition");
    m_pickingShader->AddUnif("mMVP");
    m_pickingShader->AddUnif("modelUniqueId");
    return true;
}
