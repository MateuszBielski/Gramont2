#include "cameratrial.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>



CameraTrial::CameraTrial()
{
	q_rotation.w = 1.0d;
}
void CameraTrial::UpdateMatrices()
{
    if ( m_needMVPUpdate ) {
        MyMatMul4x4(m_dView, m_dMode, m_dToVw);
        MyMatMul4x4(m_dProj, m_dToVw, m_dMVP);
//        MyMatMul4x4(m_dProj, m_dView, m_dToVw);
//        MyMatMul4x4(m_dToVw, m_dMode, m_dMVP);
        // Store the 'float' matrices

        SetAsGLFloat4x4(m_dToVw, m_fToVw, 16);
//        SetAsGLFloat4x4(m_dMVP, m_fToVw, 16);
        SetAsGLFloat4x4(m_dMVP, m_fMVP, 16);
        /***forDebug matrixStack*/
        float sum= 0.0f;
        for(short i = 0 ; i < 16 ; i++) {
            sum += m_fMVP[i];
        }
        if(sum > 0.0f) {
            int o = 9;
        }
        /**/
//        SetAsGLFloat4x4(m_dToVw, m_fMVP, 16);
        m_needMVPUpdate = false;
    }
//    int i;
//    cout.precision(3);
//    cout<<fixed;
//    cout<<"\n double matrix_model[] = {"<<fixed;
//    for(i = 0; i < 16 ; i++)cout<<m_dMode[i]<<", ";
//    cout<<"};\n double matrix_projection[] = {";
//    for(i = 0; i < 16 ; i++)cout<<m_dProj[i]<<", ";
//    cout<<"};\n double matrix_view[] = {";
//    for(i = 0; i < 16 ; i++)cout<<m_dView[i]<<", ";
//    cout<<"\n VW";
//    for(i = 0; i < 16 ; i++)cout<<m_fToVw[i]<<", ";
}
const double* CameraTrial::getViewMatrixdv()
{
    return m_dView;
}

const double* CameraTrial::getProjMatrixdv()
{
    return m_dProj;
}
const double* CameraTrial::getModeMatrixdv()
{
    return m_dMode;
}
void CameraTrial::ViewSizeChanged(int newWidth, int newHeight)
{
    myOGLCamera::ViewSizeChanged(newWidth,newHeight);
    needUpdateProjMat = true;
}

void CameraTrial::UpdatePosition(int m_mousePrevX,int  m_mousePrevY,int  posX, int posY)
{
    double xdiff = 1.0d * (posX - m_mousePrevX);
    double ydiff = 1.0d * (posY - m_mousePrevY);

//   m_camPosition

    glm::dvec4 moveInCameraCoord(xdiff,ydiff,0.0,0.0);
    glm::dvec4 moveInWorldCoord = glm::inverse(glm::make_mat4x4(m_dView)) * moveInCameraCoord;

    m_camPosition.x -= moveInWorldCoord.x;
    m_camPosition.y -= moveInWorldCoord.y;
    m_camPosition.z -= moveInWorldCoord.z;

    m_camTarget.x -= moveInWorldCoord.x;
    m_camTarget.y -= moveInWorldCoord.y;
    m_camTarget.z -= moveInWorldCoord.z;
    MyLookAt(m_camPosition, m_camUp, m_camTarget, m_dView);
}
void CameraTrial::MouseRotation(int fromX, int fromY, int toX, int toY)
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

    dvec3 v1 = normalize(dvec3{xw1, yw1, z1});
    dvec3 v2 = normalize(dvec3{xw2, yw2, z2});
    
    dvec3 axis = cross(v1,v2);
    
    dvec4 axis4(axis,0.0);
    axis4 = inverse(make_mat4x4(m_dView)) * axis4;
    axis.x = axis4.x;
    axis.y = axis4.y;
    axis.z = axis4.z;

    axis = normalize(axis);
    double angle = glm::angle(v1,v2);
    q_rotationDiff = angleAxis(angle, axis);
    q_rotation = q_rotationDiff * q_rotation;
    
    dvec3 camTarget(m_camTarget.x, m_camTarget.y, m_camTarget.z);

    dmat4x4 mnew(1.0d);
    mnew = glm::translate(mnew, camTarget);
    mnew = mnew * toMat4(q_rotation);
    mnew = glm::translate(mnew, -camTarget);

    double * mnewv = glm::value_ptr(mnew);
    for (size_t i = 0; i<16; ++i)
        m_dMode[i] = mnewv[i];

    // Inform we need to calculate MVP matrix
    m_needMVPUpdate = true;
}
