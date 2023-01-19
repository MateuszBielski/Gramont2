#include <iostream>
#include "myoglmanager.h"
#ifdef TESTOWANIE_F
#include "glFunctionsMock.h"
#endif
using namespace std;

myOGLErrHandler* externalMyOGLErrHandler = NULL;
// Allow GL errors to be handled in other part of the app.
bool MyOnGLError(int err, const GLchar* glMsg)
{
    GLenum GLErrorVal = glGetError();

    if ( err == myoglERR_CLEAR ) {
        // Clear previous errors
        while ( GLErrorVal != GL_NO_ERROR )
            GLErrorVal = glGetError();
        return true;
    }

    if ( (GLErrorVal == GL_NO_ERROR) && (glMsg == NULL) )
        return true;

    if ( externalMyOGLErrHandler ) {
        // Use the external error message handler. We pass our err-enum value.
        externalMyOGLErrHandler(err, GLErrorVal, glMsg);
    }

    return err == myoglERR_JUSTLOG ? true : false;
}

myOGLManager::myOGLManager(myOGLErrHandler* extErrHnd)
{
//    externalMyOGLErrHandler = extErrHnd;
#ifdef TESTOWANIE_F
    GlFunctionsMock functionsMock;
    functionsMock.Define();
#endif
    MyOnGLError(myoglERR_CLEAR); //clear error stack
    m_BufferLoader = make_unique<BufferLoader>();
    m_TexRenderer = make_unique<OglRenderer>();
    m_OglRenderer = make_unique<OglRenderer>();
    m_Camera = make_unique<myOGLCamera>();
}

myOGLManager::~myOGLManager()
{
    MyOnGLError(myoglERR_CLEAR); //clear error stack

    // Force GPU finishing before the context is deleted
    glFinish();
}

/* Static */
bool myOGLManager::Init()
{
//     Retrieve the pointers to OGL functions we use in this sample
//    return MyInitGLPointers();
    if (gladLoadGL() == -1) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;//using glad instead
}

const GLubyte* myOGLManager::GetGLVersion()
{
    return glGetString(GL_VERSION);
}

const GLubyte* myOGLManager::GetGLVendor()
{
    return glGetString(GL_VENDOR);
}

const GLubyte* myOGLManager::GetGLRenderer()
{
    return glGetString(GL_RENDERER);
}
template<typename T>
void myOGLManager::setLocations(unique_ptr<T>& rend, vec_locations_T<T> vec, myOGLShaders& shader,  sha_FunGetStr FunGetByString)
{
    for(auto& name : vec) {
        rend->m_loc.*(get<0>(name)) = (shader.*FunGetByString)(get<1>(name));//ok
    }
}
void myOGLManager::setMatricesForRender(upOglRenderer& rend)
{
    rend->m_matrices.matMVP = m_Camera->GetFloatMVP();
    rend->m_matrices.matToVw = m_Camera->GetFloatToVw();
    rend->m_matrices.light_position = m_Light.GetFLightPos();
    rend->m_matrices.light_colour = m_Light.GetFLightColour();
}
void myOGLManager::SetShadersAndGeometry()
{
//    ZapiszShaderyDoPlikow();
    OdczytajShaderyZplikow();
}

void myOGLManager::SetViewport(int x, int y, int width, int height)
{
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    glViewport(x, y, (GLsizei)width, (GLsizei)height);

    // The camera handles perspective projection
    m_Camera->ViewSizeChanged(width, height);
}

void myOGLManager::Render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor((GLfloat)0.15, (GLfloat)0.15, 0.0, (GLfloat)1.0); // Dark, but not black.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Draw3d();

    // This string is at the very front, whatever z-coords are given
    glDisable(GL_DEPTH_TEST);
    Draw2d();
}

void myOGLManager::OnMouseButDown(int posX, int posY)
{
    // Just save mouse position
    m_mousePrevX = posX;
    m_mousePrevY = posY;
}
void myOGLManager::SwitchViewControl()
{
	doesCameraViewControl = !doesCameraViewControl;
}

void myOGLManager::OnMouseRotDragging(int posX, int posY)
{
    m_Camera->MouseRotation(m_mousePrevX, m_mousePrevY, posX, posY);
    m_mousePrevX = posX;
    m_mousePrevY = posY;
}
//for linker
template void myOGLManager::setLocations<OglRenderer>(unique_ptr<OglRenderer>&, vec_locations_T<OglRenderer>, myOGLShaders&, sha_FunGetStr);
template void myOGLManager::setLocations<BufferLoader>(unique_ptr<BufferLoader>&, vec_locations_T<BufferLoader>, myOGLShaders&, sha_FunGetStr);
