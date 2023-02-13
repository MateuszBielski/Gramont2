//#include <cmath>
#include "onemodelmanager.h"
#include "textfile.h"
#include "funkcje.h"
#ifdef TESTOWANIE_F
#include "glFunctionsMock.h"
#endif

#include "shadersPath.h"
//
//#include "pyramid.h"
//#include "box.h"
//#include "triangle.h"
//#include "surfaceonequad.h"
//#include "surface.h"
//#include "surface3x1.h"
//#include "surface1x3.h"
//#include "surface1x3colors.h"
//#include "surface1x1normals.h"
#include "convexsurface.h"

using namespace std;
//#define Model make_shared<Pyramid>()
//#define Model make_shared<Box>()
//#define Model make_shared<Triangle>()
#define TEXTURE_IMAGE "Resources/MB640x400.jpg"
//#define Model make_shared<SurfaceOneQuad>(180.0f,180.0f)
//#define Model make_shared<Surface>(2,2,200,200)
//#define Model make_shared<Surface1x1normals>()
#define Model make_shared<ConvexSurface>(80,80,200,200,50)


bool OneModelManager::isStaticManagerInitialized = false;
unique_ptr<OneModelManager> OneModelManager::ptr_OneModelManagerStatic;
#ifdef TESTOWANIE_F
OneModelManager OneModelManager::m_OMM_static_inst(nullptr);
#endif

unsigned short OneModelManager::nuConstructors = 0;
unsigned short OneModelManager::nuDestructors = 0;
unsigned short OneModelManager::nuInstantions = 0;
unsigned short OneModelManager::maxNuInstantions = 0;

OneModelManager::OneModelManager(myOGLErrHandler* extErrHnd)
{
    setErrHandler(extErrHnd);
//    model = new Model;
    model = Model;
    ++nuConstructors;
    ++nuInstantions;
}
OneModelManager::~OneModelManager()
{
//    if(model)delete model;
    --nuInstantions;
    ++nuDestructors;
    if(nuInstantions <1 && nuDestructors > 1 ) {
        cout<<"\nNu: "<<nuInstantions<<"Konstr: "<<nuConstructors<<" Destr:"<<nuDestructors<<endl;
    }
}

void OneModelManager::SetModel(spOneModel m)
{
    model = m;
}

void OneModelManager::InitModelShader()
{
    m_ModelShader.AddAttrib("in_Position");
    m_ModelShader.AddAttrib("in_Colour");
    m_ModelShader.AddAttrib("in_Normal");
    m_ModelShader.AddUnif("mMVP");
    m_ModelShader.AddUnif("mToViewSpace");
    m_ModelShader.AddUnif("lightProps");
    m_ModelShader.AddUnif("lightColour");
    m_ModelShader.AddCode(modelVertexShader, GL_VERTEX_SHADER);
    m_ModelShader.AddCode(illuminationShader, GL_FRAGMENT_SHADER);
    m_ModelShader.AddCode(modelFragmentShader, GL_FRAGMENT_SHADER);
    m_ModelShader.Init();
}

void OneModelManager::InitTextureShader()
{
    m_TextureShader.AddAttrib("in_sPosition");
    m_TextureShader.AddAttrib("in_sNormal");
    m_TextureShader.AddAttrib("in_TextPos");
    m_TextureShader.AddUnif("mMVP");
    m_TextureShader.AddUnif("mToViewSpace");
    m_TextureShader.AddUnif("lightProps");
    m_TextureShader.AddUnif("lightColour");
    m_TextureShader.AddUnif("stringTexture");
    m_TextureShader.AddCode(textureVertexShader, GL_VERTEX_SHADER);
    m_TextureShader.AddCode(illuminationShader, GL_FRAGMENT_SHADER);
    m_TextureShader.AddCode(textureFragmentShader, GL_FRAGMENT_SHADER);
    m_TextureShader.Init();
    MyOnGLError(myoglERR_CLEAR);
}



void OneModelManager::SetShadersAndGeometry()
{
     vector<t_OLoc_str> tnames {
        {&OLoc::mMVP,"mMVP"},
        {&OLoc::mToViewSpace,"mToViewSpace"},
        {&OLoc::lightProps,"lightProps"},
        {&OLoc::lightColour,"lightColour"},
        {&OLoc::stringTexture,"stringTexture"}
    };
    vector<t_OLoc_str> names {
        {&OLoc::mMVP,"mMVP"},
        {&OLoc::mToViewSpace,"mToViewSpace"},
        {&OLoc::lightProps,"lightProps"},
        {&OLoc::lightColour,"lightColour"}
    };
    vector<t_BLoc_str> bnames {
        {&BLoc::position,"in_Position"},
        {&BLoc::colour,"in_Colour"},
        {&BLoc::normal,"in_Normal"},
    };
    vector<t_BLoc_str> btnames {
        {&BLoc::position_tex,"in_sPosition"},
        {&BLoc::normal_tex,"in_sNormal"},
        {&BLoc::textureCoord,"in_TextPos"},
    };
    
    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);
    OdczytajShaderyZplikow();
    InitModelShader();

    setLocations<BufferLoader>(m_BufferLoader,bnames,m_ModelShader,&myOGLShaders::GetAttribLoc);
    m_BufferLoader->CreateBuffersForSingleModelEntry(model->GetModelData());
    m_BufferLoader->LoadBuffersForSingleModelEntry(model->GetModelData());

    setLocations<OglRenderer>(m_OglRenderer, names, m_ModelShader,&myOGLShaders::GetUnifLoc);
    setMatricesForRender(m_OglRenderer);

    auto ptrTex = model->MyTexture();
    if(!ptrTex) return;

    InitTextureShader();

    auto& tex = *ptrTex;
    tex.LoadImageFile(TEXTURE_IMAGE);

    setLocations<BufferLoader>(m_BufferLoader,btnames,m_TextureShader,&myOGLShaders::GetAttribLoc);
    m_BufferLoader->LoadTextureBuffersForSingleModelEntry(tex,model->GetModelData());
   
    setLocations<OglRenderer>(m_TexRenderer,tnames,m_TextureShader,&myOGLShaders::GetUnifLoc);
    setMatricesForRender(m_TexRenderer);
}

void OneModelManager::DrawTexture()// to delete
{
    m_Camera->UpdateMatrices();

    m_TexRenderer->DrawTextureForSingleModelEntry(
        *(model->MyTexture()),
        model->GetModelData(),
        m_TextureShader.getProgramId());
}
void OneModelManager::Draw3d()
{
    auto modelData = model->GetModelData();

    m_Camera->UpdateMatrices();

    //unnecessery
//    m_OglRenderer->DrawSingleModelEntry(
//        modelData,
//        m_ModelShader.getProgramId());

    m_TexRenderer->DrawTextureForSingleModelEntry(
        *(model->MyTexture()),
        modelData,
        m_TextureShader.getProgramId());
}
void OneModelManager::ZapiszShaderyDoPlikow()
{
    textFileWrite("triangVertexShader.c",modelVertexShader);
    textFileWrite("illuminationShader.c",illuminationShader);
    textFileWrite("triangFragmentShader.c",modelFragmentShader);
}
void OneModelManager::OdczytajShaderyZplikow()
{
    modelVertexShader  = textFileRead(d_vertexShaderPath);
    modelFragmentShader = textFileRead(d_fragmentShaderPath);
    illuminationShader = textFileRead(d_illuminationShaderPath);
    textureVertexShader = textFileRead(d_textureVertexShaderPath);
    textureFragmentShader = textFileRead(d_textureFragmentShaderPath);
}



void OneModelManager::Draw2d()
{
//    m_ImmString.Draw(m_ImmString.GetFloatMVP(), NULL, NULL);
}

#ifdef TESTOWANIE_F
OneModelManager& OneModelManager::CreateAndInitStatic()
{
    if (!isStaticManagerInitialized) {
//        wxGLContextAttrs ctxAttrs;
//        OneModelManager::Init();
        GlFunctionsMock functionsMock;
        functionsMock.Define();
//        ptr_OneModelManagerStatic = make_unique<OneModelManager>();
        isStaticManagerInitialized = true;
    }
//    return *ptr_OneModelManagerStatic;
    return m_OMM_static_inst;
}
#endif
void OneModelManager::OnMouseLeftDClick(int posX, int posY)
{
    auto a = posX;
    auto b = wxWidth;
//    Selecting selecting;
////    ViewportParameters viewportParameters;
//    selecting.setViewportParameters(viewportParameters);
//    selecting.setMousePosition(posX,posY);
//    SelectingResult sel = selecting.getSelectingResult();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
}
