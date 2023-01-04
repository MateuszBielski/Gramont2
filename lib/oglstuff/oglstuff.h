/////////////////////////////////////////////////////////////////////////////
// Name:        oglstuff.h
// Purpose:     OpenGL manager for pyramid sample
// Author:      Manuel Martin
// Created:     2015/01/31
// Copyright:   (c) 2015 Manuel Martin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef OGLSTUFF_H
#define OGLSTUFF_H

#include <string>
#include <vector>
#include <memory>
#include "mathstuff.h"
// This file allows access to OpenGL functions used in this sample.
#include <glad/glad.h>

using namespace std;

enum {
    myoglERR_CLEAR = 0,
    myoglERR_JUSTLOG,

    myoglERR_SHADERCREATE,
    myoglERR_SHADERCOMPILE,
    myoglERR_SHADERLINK,
    myoglERR_SHADERLOCATION,

    myoglERR_BUFFER,
    myoglERR_TEXTIMAGE,

    myoglERR_DRAWING_TRI,
    myoglERR_DRAWING_STR
};

// Used to handle GL errors in other part of the app.
typedef void myOGLErrHandler(int err, int glerr, const GLchar* glMsg);

// For shader attributes
struct shaVars {
    GLuint loc; //The attribute "location", some kind of index in the shader
    std::string name; //The name of the attribute
};

typedef std::vector<shaVars> shaVars_v;

// For shader code
struct shaShas {
    GLuint shaId;
    GLenum typeSha; //The type of shader
    const GLchar* scode; //The NULL terminated GLSL code
};

typedef std::vector<shaShas> shaShas_v;

void SetAsGLFloat4x4(double *matD, GLfloat *matF, int msize);
bool MyOnGLError(int err, const GLchar* glMsg = NULL);

//-----------------------------------------------------------------------------
// This object builds a GPU program by joining several shaders.
class myOGLShaders
{
public:
    myOGLShaders();
    ~myOGLShaders();
#ifdef TESTOWANIE_F
    virtual
#endif
    void Init();
    bool Use();
    void StopUse();
    void CleanUp();
    GLuint getProgramId();

    void AddCode(const GLchar* shaString, GLenum shaType);
    void AddAttrib(const std::string& name);
    void AddUnif(const std::string& name);
    GLuint GetAttribLoc(const std::string& name);
    GLuint GetUnifLoc(const std::string& name);
    // Disable generic vertex attribute array
    void DisableGenericVAA();

protected:
    void SetAttribLocations();
    bool AskUnifLocations();
#ifdef TESTOWANIE_F
    virtual
#endif
    bool Compile(GLuint shaId);
    bool LinkProg(GLuint proId);

    shaVars_v m_shaAttrib; // 'attributes' names and locations
    shaVars_v m_shaUnif; // 'uniforms' names and locations
    shaShas_v m_shaCode; // shaders code and their types
    GLuint m_proId; // program Id

    bool m_SHAinitializated;
};
using spMyOGLShaders = shared_ptr<myOGLShaders>;
//-----------------------------------------------------------------------------
// A "point light"
class myLight
{
public:
    myLight();
    ~myLight() {}

    void Set(const myVec3& position, GLfloat intensity,
             GLfloat R, GLfloat G, GLfloat B);
    // Return position and intensity
    const GLfloat* GetFLightPos() const {
        return m_PosAndIntensisty;
    }
    // Return colour
    const GLfloat* GetFLightColour() const {
        return m_Colour;
    }

private:
    // Light position and intensity
    GLfloat m_PosAndIntensisty[4];
    // Light colour
    GLfloat m_Colour[3];
};


//-----------------------------------------------------------------------------
// The "camera", or the point of view
class myOGLCamera
{
public:
    myOGLCamera();
    ~myOGLCamera() {}

    // Initial positions
    void InitPositions();
    // When the size of the window changes
    virtual void ViewSizeChanged(int newWidth, int newHeight);
    // The whole transformation matrix
    const GLfloat* GetFloatMVP();
    // The 'To View Space' transformation matrix
    const GLfloat* GetFloatToVw();
    // The camera position

    virtual void UpdateMatrices();
    bool NeedMVPUpdate() {
        return m_needMVPUpdate;
    }
    myVec3 GetPosition() {
        return m_camPosition;
    }

    // Simulates a virtual trackball and rotates the 'world'
    void MouseRotation(int fromX, int fromY, int toX, int toY);
    double GetTrackballZ(double x, double y, double r);

    // The used matrices
    double m_dMode[16]; // The model matrix, rotation in this sample
    double m_dView[16]; // The view matrix
    double m_dProj[16]; // The projection matrix
    double m_dMVP[16];  // The whole transform matrix
    double m_dToVw[16]; // The 'to View' transform matrix
    // GLFloat versions. GLdouble is available since OGL 4.0, and we use 3.2
    GLfloat m_fMVP[16];
    GLfloat m_fToVw[16];
protected:
    bool m_needMVPUpdate;
private:


    // Coordinates in model space
    myVec3 m_centerOfWorld;
    double m_radiusOfWorld;
    myVec3 m_camPosition;
    myVec3 m_camTarget;
    myVec3 m_camUp;

    // Window size in pixels
    int m_winWidth;
    int m_winHeight;

    // Parameters for the projection
    double m_fov;
    double m_nearD;
    double m_farD;
};

using spMyOGLCamera = shared_ptr<myOGLCamera>;
using upMyOGLCamera = unique_ptr<myOGLCamera>;
#endif //OGLSTUFF_H
