#ifndef myOGLManager_H
#define myOGLManager_H
#include "oglstuff.h"
#include "bufferloader.h"
#include "oglrenderer.h"

// Used to handle GL errors in other part of the app.
typedef void myOGLErrHandler(int err, int glerr, const GLchar* glMsg);

using sha_FunGetStr = GLuint (myOGLShaders::*)(const std::string& name);

using OLoc = OglRenderer::Locations;
using t_OLoc_str = tuple<int OLoc::*,string>;
using BLoc = BufferLoader::Locations;
using t_BLoc_str = tuple<int BLoc::*,string>;

template<typename T>
using vec_locations_T = vector<tuple<int T::Locations::* , string>>;

class myOGLManager
{
public:
    myOGLManager(myOGLErrHandler* extErrHnd = NULL);
    ~myOGLManager();

    // Constants, prototypes and pointers to OGL functions
    static bool Init();
    // Strings describing the current GL connection
    const GLubyte* GetGLVersion();
    const GLubyte* GetGLVendor();
    const GLubyte* GetGLRenderer();

    void Render(); // Total rendering


    // Load data into the GPU
    virtual void SetShadersAndGeometry();
    // For window size change
    virtual void SetViewport(int x, int y, int width, int height);
    virtual void Draw3d() {};
    virtual void Draw2d() {};

    // Action events in OpenGL win coordinates (bottom is y=0)
    void OnMouseButDown(int posX, int posY);
    void OnMouseRotDragging(int posX, int posY);
    virtual void OnMouseLeftDClick(int posX, int posY) {};
    virtual void OnMouseMiddleClick(int posX, int posY) {};
    virtual void OnMouseWheel(int rotation) {};

    virtual void ZapiszShaderyDoPlikow() {};
    virtual void OdczytajShaderyZplikow() {};
#ifdef TESTOWANIE_F
    upBufferLoader getBufferLoaderForTest(){return move(m_BufferLoader);};
    upOglRenderer getTexRendererForTest(){return move(m_TexRenderer);};
    myOGLCamera& getCameraRefForTest(){return *m_Camera;}
#endif

protected:
    // Members
    myLight        m_Light;
    spMyOGLCamera    m_Camera;
    upBufferLoader m_BufferLoader;//should be settable
    upOglRenderer m_TexRenderer;//j/w
    upOglRenderer m_OglRenderer;

    virtual void setMatricesForRender(upOglRenderer& );
    template<typename T>
    void setLocations(unique_ptr<T>&,  vec_locations_T<T>, myOGLShaders&, sha_FunGetStr );


    // For mouse event
    int m_mousePrevX = 0;
    int m_mousePrevY = 0;
};

#endif // myOGLManager_H
