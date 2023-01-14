#include "cameratrial.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/vec_swizzle.hpp>



CameraTrial::CameraTrial()
{
    q_rotation.w = 1.0d;
    position = dvec3(m_camPosition.x,m_camPosition.y,m_camPosition.z);
    target = dvec3(m_camTarget.x,m_camTarget.y,m_camTarget.z);
    camUp = dvec3(m_camUp.x,m_camUp.y,m_camUp.z);
    UpdateViewMatrix();
}

void CameraTrial::UpdateMatrices()
{



    if ( m_needMVPUpdate ) {
        MyMatMul4x4(m_dView, m_dMode, m_dToVw);
        MyMatMul4x4(m_dProj, m_dToVw, m_dMVP);

        SetAsGLFloat4x4(m_dToVw, m_fToVw, 16);
        SetAsGLFloat4x4(m_dMVP, m_fMVP, 16);

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
//    return m_dView;
    return value_ptr(dmat4view);
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

    glm::dvec4 moveInCameraCoord(xdiff,ydiff,0.0,0.0);
    glm::dvec4 moveInWorldCoord = inverse(transformation) * 
    inverse(make_mat4x4(m_dView)) * moveInCameraCoord;

    target = target - glm::xyz(moveInWorldCoord);
    position = position - glm::xyz(moveInWorldCoord);
    
    UpdateViewMatrix();
}
dquat CameraTrial::RotationFromScreenMove(ScreenMove& move, dmat_stack mstack)
{

    double xw1 = (2.0 * move.fromX - m_winWidth) / m_winWidth;
    double yw1 = (2.0 * move.fromY - m_winHeight) / m_winHeight;
    double xw2 = (2.0 * move.toX - m_winWidth) / m_winWidth;
    double yw2 = (2.0 * move.toY - m_winHeight) / m_winHeight;
    double z1 = GetTrackballZ(xw1, yw1, 0.8);
    double z2 = GetTrackballZ(xw2, yw2, 0.8);

    dvec3 v1 = normalize(dvec3 {xw1, yw1, z1});
    dvec3 v2 = normalize(dvec3 {xw2, yw2, z2});

    dvec3 axis = cross(v1,v2);

    dvec4 axis4(axis,0.0);

    while(!mstack.empty()) {
        axis4 = inverse(*mstack.top()) * axis4;
        mstack.pop();
    }

    axis = normalize(xyz(axis4));
    double angle = glm::angle(v1,v2);
    return angleAxis(angle, axis);
}
void CameraTrial::UpdateViewMatrix()
{
    transformation = dmat4x4(1.0d);
    transformation = translate(transformation, target);
    transformation = transformation * toMat4(q_rotation);
    transformation = translate(transformation, -target);

    dmat4view = lookAt(position,target,camUp);
    dmat4view = dmat4view * transformation;
}
void CameraTrial::MouseRotation(int fromX, int fromY, int toX, int toY)
{
    if ( fromX == toX && fromY == toY )
        return; //no rotation

    dmat_stack mstack;
    dmat4x4 mat_dProj = make_mat4x4(m_dProj);
    dmat4x4 mat_dView = make_mat4x4(m_dView);
    mstack.push(&mat_dProj);
    mstack.push(&mat_dView);
//    mstack.push(&dmat4view);

    ScreenMove move;
    move.fromX = fromX;
    move.fromY = fromY;
    move.toX = toX;
    move.toY = toY;
    dquat q_diff = RotationFromScreenMove(move, mstack);

    q_rotation = q_diff * q_rotation;

    UpdateViewMatrix();
}
