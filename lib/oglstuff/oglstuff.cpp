/////////////////////////////////////////////////////////////////////////////
// Name:        oglstuff.cpp
// Purpose:     OpenGL manager for pyramid sample
// Author:      Manuel Martin
// Created:     2015/01/31
// Copyright:   (c) 2015 Manuel Martin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include "oglstuff.h"


// We do calculations with 'doubles'. We pass 'GLFloats' to the shaders
// because OGL added 'doubles' since OGL 4.0, and this sample is for 3.2
// Due to asynchronous nature of OGL, we can't not trust in the passed matrix
// to be stored by GPU before the passing-function returns. So we don't use
// temporary storage, but dedicated matrices
void SetAsGLFloat4x4(double *matD, GLfloat *matF, int msize)
{
    for (int i = 0; i < msize; i++) {
        matF[i] = (GLfloat) matD[i];
    }
}


myOGLShaders::myOGLShaders()
{
    m_proId = 0;
    m_SHAinitializated = false;
}

myOGLShaders::~myOGLShaders()
{
    if ( m_proId )
        CleanUp();
}

void myOGLShaders::CleanUp()
{
    StopUse();

    glDeleteProgram(m_proId);

    glFlush();
}

void myOGLShaders::AddCode(const GLchar* shaString, GLenum shaType)
{
    // The code is a null-terminated string
    if(shaString == nullptr)return;
    shaShas sv = {0, shaType, shaString};
    m_shaCode.push_back(sv);
}

void myOGLShaders::AddAttrib(const std::string& name)
{
    shaVars sv = {0, name}; //We will set the location later
    m_shaAttrib.push_back(sv);
    // We don't check the max number of attribute locations (usually 16)
}

void myOGLShaders::AddUnif(const std::string& name)
{
    shaVars sv = {0, name};
    m_shaUnif.push_back(sv);
}

// Inform GL of the locations in program for the vars for buffers used to feed
// the shader. We use glBindAttribLocation (before linking the gl program) with
// the location we want.
// Since GL 3.3 we could avoid this using in the shader "layout(location=x)...".
// The same names as in the shader must be previously set with AddAttrib()
void myOGLShaders::SetAttribLocations()
{
    GLuint loc = 0;
    for(shaVars_v::iterator it = m_shaAttrib.begin(); it != m_shaAttrib.end(); ++it) {
        it->loc = loc++;
        glBindAttribLocation(m_proId, it->loc, it->name.c_str());
    }
}

GLuint myOGLShaders::GetAttribLoc(const std::string& name)
{
    for (shaVars_v::iterator it = m_shaAttrib.begin(); it != m_shaAttrib.end(); ++it) {
        if ( it->name == name && it->loc != (GLuint)-1 )
            return it->loc;
    }

    return (GLuint) -1;
}

// Store the locations in program for uniforms vars
bool myOGLShaders::AskUnifLocations()
{
    bool isNoError = true;
    for (shaVars_v::iterator it = m_shaUnif.begin(); it != m_shaUnif.end(); ++it) {
        GLint glret = glGetUniformLocation(m_proId, it->name.c_str());
        if ( glret == -1 ) {
            string err = "uniform " + it->name + " not found";
            MyOnGLError(myoglERR_SHADERLOCATION, err.c_str());
            isNoError = false;
            continue;
        }
        it->loc = glret;
    }

    return isNoError;
}

GLuint myOGLShaders::GetUnifLoc(const std::string& name)
{
    for (shaVars_v::iterator it = m_shaUnif.begin(); it != m_shaUnif.end(); ++it) {
        if ( it->name == name && it->loc != (GLuint)-1 )
            return it->loc;
    }

    return (GLuint) -1;
}
void myOGLShaders::Init(string funName)
{
    nameOfOuterFunctionCalling = funName;
    Init();
}
// Create a GPU program from the given shaders
void myOGLShaders::Init()
{
    MyOnGLError(myoglERR_CLEAR); //clear error stack

    bool resC = false;
    bool resL = false;

    // GLSL code load and compilation
    for (shaShas_v::iterator it = m_shaCode.begin(); it != m_shaCode.end(); ++it) {
        it->shaId = glCreateShader(it->typeSha);
        glShaderSource(it->shaId, 1, &(it->scode), NULL);
        MyOnGLError(myoglERR_SHADERCREATE);

        resC = Compile(it->shaId);
        if ( !resC )
            break;
    }

    if ( resC ) {
        // The program in the GPU
        m_proId = glCreateProgram();
        for (shaShas_v::iterator it = m_shaCode.begin(); it != m_shaCode.end(); ++it) {
            glAttachShader(m_proId, it->shaId);
        }

        SetAttribLocations(); //Before linking

        resL = LinkProg(m_proId);
    }

    // We don't need them any more
    for (shaShas_v::iterator it = m_shaCode.begin(); it != m_shaCode.end(); ++it) {
        if ( resC && it->shaId ) {
            glDetachShader(m_proId, it->shaId);
        }
        glDeleteShader(it->shaId);
    }

    if ( !resC || !resL )
        return;

    // Log that shaders are OK
    std::string message = "Shaders, called from "+nameOfOuterFunctionCalling+" successfully compiled and linked.";
    MyOnGLError(myoglERR_JUSTLOG,message.c_str() );

    // After linking, we can get locations for uniforms
    m_SHAinitializated = AskUnifLocations();
    if ( !m_SHAinitializated )
        MyOnGLError(myoglERR_SHADERLOCATION, " Unused or unrecognized uniform.");
}

// Useful while developing: show shader compilation errors
bool myOGLShaders::Compile(GLuint shaId)
{
    glCompileShader(shaId);

    GLint Param = 0;
    glGetShaderiv(shaId, GL_COMPILE_STATUS, &Param);

    if ( Param == GL_FALSE ) {
        glGetShaderiv(shaId, GL_INFO_LOG_LENGTH, &Param);

        if ( Param > 0 ) {
            GLchar* InfoLog = new GLchar[Param];
            int nChars = 0;
            glGetShaderInfoLog(shaId, Param, &nChars, InfoLog);
            MyOnGLError(myoglERR_SHADERCOMPILE, InfoLog);
            delete [] InfoLog;
        }
        return false;
    }
    return true;
}

// Useful while developing: show shader program linkage errors
bool myOGLShaders::LinkProg(GLuint proId)
{
    glLinkProgram(proId);

    GLint Param = 0;
    glGetProgramiv(proId, GL_LINK_STATUS, &Param);

    if ( Param == GL_FALSE ) {
        glGetProgramiv(proId, GL_INFO_LOG_LENGTH, &Param);

        if ( Param > 0 ) {
            GLchar* InfoLog = new GLchar[Param];
            int nChars = 0;
            glGetProgramInfoLog(proId, Param, &nChars, InfoLog);
            MyOnGLError(myoglERR_SHADERLINK, InfoLog);
            delete [] InfoLog;
        }
        return false;
    }
    return true;
}

bool myOGLShaders::Use()
{
    if ( !m_SHAinitializated )
        return false;

    glUseProgram(m_proId);
    return true;
}

void myOGLShaders::StopUse()
{
    glUseProgram(0);
}

// Disable generic attributes from VAO.
// This should be needed only for some old card, which uses generic into VAO
void myOGLShaders::DisableGenericVAA()
{
    for(shaVars_v::iterator it = m_shaAttrib.begin(); it != m_shaAttrib.end(); ++it) {
        glDisableVertexAttribArray(it->loc);
    }
}
GLuint myOGLShaders::getProgramId()
{
	return m_proId;
}



// ----------------------------------------------------------------------------
// A point light
// ----------------------------------------------------------------------------
myLight::myLight()
{
    m_PosAndIntensisty[0] = 0;
    m_PosAndIntensisty[1] = 0;
    m_PosAndIntensisty[2] = 0;
    m_PosAndIntensisty[3] = 0;
    m_Colour[0] = 0;
    m_Colour[1] = 0;
    m_Colour[2] = 0;
}
void myLight::Set(const myVec3& position, GLfloat intensity,
                  GLfloat R, GLfloat G, GLfloat B)
{
    m_PosAndIntensisty[0] = (GLfloat) position.x;
    m_PosAndIntensisty[1] = (GLfloat) position.y;
    m_PosAndIntensisty[2] = (GLfloat) position.z;
    m_PosAndIntensisty[3] = (GLfloat) intensity;
    m_Colour[0] = R;
    m_Colour[1] = G;
    m_Colour[2] = B;
}



// ----------------------------------------------------------------------------
// myOGLCamera
// ----------------------------------------------------------------------------
myOGLCamera::myOGLCamera()
{
    m_needMVPUpdate = true; //Matrix must be updated
    InitPositions();
}

void myOGLCamera::InitPositions()
{
    // We have a tetrahedron centered at origin and edge length = 200
    m_centerOfWorld.x = m_centerOfWorld.y = m_centerOfWorld.z = 0.0;

    // The radius of the bounding sphere
    m_radiusOfWorld = 122.4745;

    // From degrees to radians
    double degToRad = (double) 4.0 * atan(1.0) / 180.0;

    // Angle of the field of view
    m_fov = 40.0 * degToRad; //radians

    // Position the camera far enough so we can see the whole world.
    // The camera is between X and Z axis, below the pyramid
    double tmpv = m_radiusOfWorld / sin(m_fov/2.0);
    tmpv *= 1.05; // 5% margin
    m_camPosition.x = m_centerOfWorld.x + tmpv * cos(75.0 * degToRad);
    m_camPosition.z = m_centerOfWorld.z + tmpv * sin(75.0 * degToRad);
    m_camPosition.y = m_centerOfWorld.y - m_radiusOfWorld;

    // This camera looks always at center
    m_camTarget = m_centerOfWorld;

    // A vector perpendicular to Position-Target heading Y+
    myVec3 vper = MyNormalize(m_camTarget - m_camPosition);
    m_camUp = myVec3(0.0, 1.0, 0.0);
    m_camUp = MyCross(m_camUp, vper);
    m_camUp = MyNormalize( MyCross(vper, m_camUp) );

    tmpv = MyDistance(m_camPosition, m_centerOfWorld);
    // Calculate distances, not coordinates, with some margins
    // Near clip-plane distance to the camera
    m_nearD = tmpv - 1.10 * m_radiusOfWorld - 5.0;
    // Far clip-plane distance to the camera
    m_farD = tmpv + 1.10 * m_radiusOfWorld + 5.0;

    // The "View" matrix. We will not change it any more in this sample
    MyLookAt(m_camPosition, m_camUp, m_camTarget, m_dView);

    // The initial "Model" matrix is the Identity matrix
    MyRotate(myVec3(0.0, 0.0, 1.0), 0.0, m_dMode);

    // Nothing else. "View" matrix is calculated at ViewSizeChanged()
}

void myOGLCamera::ViewSizeChanged(int newWidth, int newHeight)
{
    // These values are also used for MouseRotation()
    m_winWidth = newWidth;
    m_winHeight = newHeight;

    // Calculate the projection matrix
    aspect = (double) newWidth / newHeight;
    MyPerspective(m_fov, aspect, m_nearD, m_farD, m_dProj);

    // Inform we need to calculate MVP matrix
    m_needMVPUpdate = true;
}


const GLfloat* myOGLCamera::GetFloatMVP()
{
    UpdateMatrices();

    return m_fMVP;
}

const GLfloat* myOGLCamera::GetFloatToVw()
{
    UpdateMatrices();

    return m_fToVw;
}

void myOGLCamera::UpdateMatrices()
{
    if ( m_needMVPUpdate ) {
        MyMatMul4x4(m_dView, m_dMode, m_dToVw);
        MyMatMul4x4(m_dProj, m_dToVw, m_dMVP);
        // Store the 'float' matrices
        SetAsGLFloat4x4(m_dToVw, m_fToVw, 16);
        SetAsGLFloat4x4(m_dMVP, m_fMVP, 16);
        m_needMVPUpdate = false;
    }
}

void myOGLCamera::MouseRotation(int fromX, int fromY, int toX, int toY)
{
    if ( fromX == toX && fromY == toY )
        return; //no rotation

    // 1. Obtain axis of rotation and angle simulating a virtual trackball "r"

    // 1.1. Calculate normalized coordinates (2x2x2 box).
    // The trackball is a part of sphere of radius "r" (the rest is hyperbolic)
    // Use r= 0.8 for better maximum rotation (more-less 150 degrees)
    double xw1 = (2.0 * fromX - m_winWidth) / m_winWidth;
    double yw1 = (2.0 * fromY - m_winHeight) / m_winHeight;
    double xw2 = (2.0 * toX - m_winWidth) / m_winWidth;
    double yw2 = (2.0 * toY - m_winHeight) / m_winHeight;
    double z1 = GetTrackballZ(xw1, yw1, 0.8);
    double z2 = GetTrackballZ(xw2, yw2, 0.8);

    // 1.2. With normalized vectors, compute axis from 'cross' and angle from 'dot'
    myVec3 v1(xw1, yw1, z1);
    myVec3 v2(xw2, yw2, z2);
    v1 = MyNormalize(v1);
    v2 = MyNormalize(v2);
    myVec3 axis(MyCross(v1, v2));

    // 'axis' is in camera coordinates. Transform it to world coordinates.
    double mtmp[16];
    MyMatInverse(m_dView, mtmp);
    myVec4 res = MyMatMul4x1(mtmp, myVec4(axis));
    axis.x = res.x;
    axis.y = res.y;
    axis.z = res.z;
    axis = MyNormalize(axis);

    double angle = AngleBetween(v1, v2);

    // 2. Compute the model transformation (rotate the model) matrix
    MyRotate(axis, angle, mtmp);
    // Update "Model" matrix
    double mnew[16];
    MyMatMul4x4(mtmp, m_dMode, mnew);
    for (size_t i = 0; i<16; ++i)
        m_dMode[i] = mnew[i];

    // Inform we need to calculate MVP matrix
    m_needMVPUpdate = true;
}

// Return the orthogonal projection of (x,y) into a sphere centered on the screen
// and radius 'r'. This makes some (x,y) to be outside of circle r='r'. We avoid
// this issue by using a hyperbolic sheet for (x,y) outside of r = 0.707 * 'r'.
double myOGLCamera::GetTrackballZ(double x, double y, double r)
{
    double d = x*x + y*y;
    double r2 = r*r;
    return  (d < r2/2.0) ? sqrt(r2 - d) : r2/2.0/sqrt(d);
}
