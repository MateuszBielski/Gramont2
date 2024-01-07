#include "selecting.h"
#include "textfile.h"
#include "shadersPath.h"
//#include "funkcje.h" //round_to

using namespace std;

Selecting::Selecting()
{
    m_pickingBuffLoader = make_shared<PickingBuffLoader>();
    m_BufferLoader = m_pickingBuffLoader;
    m_pickingShader = m_shader;
}

Selecting::~Selecting()
{
}
bool Selecting::ConfigureShadersAndLocations()
{
    vertCode = textFileRead(d_vertexPickingShaderPath);
    fragCode = textFileRead(d_fragmentPickingShaderPath);

    if(!ConfigurePickingShader()) return false;

    string nameOfFunction = typeid(*this).name();
    nameOfFunction +="::";
    nameOfFunction += __FUNCTION__;
    m_pickingShader->Init(nameOfFunction);

    CreateAndLoadFrameBuffer();
    loc_mMVP = m_pickingShader->GetUnifLoc("mMVP");
    loc_objectIndex = m_pickingShader->GetUnifLoc("modelUniqueId");
    m_pickingBuffLoader->setLocationsFrom(m_pickingShader);
    inited = true;
    return true;
}
bool Selecting::ConfigurePickingShader()
{
    if(!vertCode || !fragCode)return false;

    m_pickingShader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_pickingShader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    m_pickingShader->AddAttrib("position");
    m_pickingShader->AddUnif("mMVP");
    m_pickingShader->AddUnif("modelUniqueId");
    return true;
}

void Selecting::LoadVAO(spOneModel model)
{
    m_BufferLoader->CreateVao(model->getVAOforSelect());
    m_BufferLoader->LoadBuffersForModelGeometry(model->GetModelData(),model->getVAOforSelect());
    std::string str_log = "Texture buffers loaded into GPU for model Id = "+to_string(model->getUniqueId()) + " in Selecting::LoadVAO";
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
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
    glClearColor((GLfloat)0.08, (GLfloat)0.07, 0.06, (GLfloat)1.0);
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
                     to_string(Pixel.notUsed_1) + " " +
                     to_string(Pixel.notUsed_2);
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());

    return Pixel;
}
void Selecting::UpdateSelectedModelId(PixelInfo& pxi)
{
    if(pxi.ObjectID == -2.0) return;
    if(pxi.ObjectID > -2.0 && pxi.ObjectID <= 1.0) {
        selectedModelId = -1;
        return;
    }
    selectedModelId = static_cast<int>(round(pxi.ObjectID)) - BACKGROUND_COMPENSATION;
}
void Selecting::ReadInClickedPosition()
{
    PixelInfo Pixel = ReadPixel(clickedPosX,clickedPosY);
    UpdateSelectedModelId(Pixel);
    string str_log = to_string(Pixel.ObjectID) + " " +
                     to_string(Pixel.notUsed_1) + " " +
                     to_string(Pixel.notUsed_2);
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
}
SelectingResult Selecting::getResult()
{
    if(selectedModelId > -1 && selectedModelId < registeredForSelection.size()) {

        spSelectable sel = registeredForSelection.at(selectedModelId);
        return SelectingResult(sel);
    } else {
        return SelectingResult();
    }
}
void Selecting::SetVertexShaderPath(string p)
{
//    m_vertexShaderPath = p;
}

void Selecting::SetFragmentShaderPath(string p)
{
//    m_fragmentShaderPath = p;
}
void Selecting::Draw(spOneModel model)
{
	ModelData& d = model->GetModelData();
    
    glUseProgram(getProgramId());
    glBindVertexArray(model->getVAOforSelect());

//    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_viewParamsfv.matMVP);
    glUniformMatrix4fv(loc_mMVP, 1, GL_FALSE, matrixStack->getModelViewProjectionMatrixfv());
//    glUniform1i(m_objectIndexLocation, model->getUniqueId() + BACKGROUND_COMPENSATION);
    glUniform1i(loc_objectIndex, model->getUniqueId() + BACKGROUND_COMPENSATION);

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    
    glBindVertexArray(0);
    glUseProgram(0);
}

