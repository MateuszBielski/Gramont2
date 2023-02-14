#ifndef myOGLManager_H
#define myOGLManager_H
#include "oglstuff.h"

// Used to handle GL errors in other part of the app.
typedef void myOGLErrHandler(int err, int glerr, const GLchar* glMsg);

using sha_FunGetStr = GLuint (myOGLShaders::*)(const std::string& name);


class myOGLManager
{
public:
    myOGLManager();
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
    virtual void OnMouseRotDragging(int posX, int posY);
    virtual void OnMouseLeftDClick(int posX, int posY) {};
    virtual void OnMouseMiddleClick(int posX, int posY) {};
    virtual void OnMouseWheel(int rotation) {};
    void SwitchViewControl();

    virtual void ZapiszShaderyDoPlikow() {};
    virtual void OdczytajShaderyZplikow() {};


protected:
    void setErrHandler(myOGLErrHandler* extErrHnd);
    // Members
    myLight        m_Light;
    spMyOGLCamera    m_Camera;
    bool doesCameraViewControl = true;


    // For mouse event
    int m_mousePrevX = 0;
    int m_mousePrevY = 0;
};

#endif // myOGLManager_H
