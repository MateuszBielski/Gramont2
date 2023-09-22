#include "cameratrial.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/vec_swizzle.hpp>

using namespace std;

CameraTrial::CameraTrial():q_rotation(1.0,0.0,0.0,0.0)
{
    position = dvec3(m_camPosition.x,m_camPosition.y,m_camPosition.z);
    position3f[0] = static_cast<float>(m_camPosition.x);
    position3f[1] = static_cast<float>(m_camPosition.y);
    position3f[2] = static_cast<float>(m_camPosition.z);
    target = dvec3(m_camTarget.x,m_camTarget.y,m_camTarget.z);
    camUp = dvec3(m_camUp.x,m_camUp.y,m_camUp.z);
    rotCenter = target;
    m_scale = 1.0;
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
dmat4x4* CameraTrial::getViewGlmMatrixdv()
{
    return &dmat4view;
}
dmat4x4* CameraTrial::getInvViewGlmMatrixdv()
{
    return &dmat4inv_view;
}
dmat4x4* CameraTrial::getProjGlmMatrixdv()
{
    return &dmat4proj;
}
float * CameraTrial::getPositonfv()
{
    //to jest potrzebne w rendererze dla ParalaxOclusionMapRenderSystem
    return position3f;
}
void CameraTrial::ViewSizeChanged(int newWidth, int newHeight)
{
    myOGLCamera::ViewSizeChanged(newWidth,newHeight);
    dmat4proj = glm::make_mat4x4(m_dProj);
    needUpdateProjMat = true;
}
void CameraTrial::UpdateViewMatrix()
{
    dvec3 newPosition = position;
    transformation = scale(dmat4x4(1.0),dvec3(m_scale,m_scale,m_scale));
    newPosition = xyz(transformation * dvec4(position,1.0));

    transformation = toMat4(q_rotation);
    newPosition = xyz(transformation * dvec4(newPosition,1.0));
    dvec3 newCamUp = xyz(transformation * dvec4(camUp,1.0));
    dvec3 newTarget = xyz(transformation * dvec4(target,1.0));
    dmat4view = lookAt(newPosition + rotCenter,newTarget + rotCenter,newCamUp);
//    dmat4inv_view = inverse(transformation);
    dmat4inv_view = inverse(dmat4view);
//    position3f[0] = newPosition.x + rotCenter.x;
//    position3f[1] = newPosition.y + rotCenter.y;
//    position3f[2] = newPosition.z + rotCenter.z;
    /**DEBUG***/
//    dvec3 newCamPos = newPosition + rotCenter;
//    cout<<"\ncamPosition:\n"<<newCamPos.x<<" "<<newCamPos.y<<" "<<newCamPos.z;
    /*****/
    camDistance = glm::distance(newPosition,newTarget);

    MyPerspective(m_fov, aspect, m_nearD, m_farD, m_dProj);
    dmat4proj = glm::make_mat4x4(m_dProj);
}

void CameraTrial::MoveOnScreenPlane(int m_mousePrevX,int  m_mousePrevY,int  posX, int posY)
{
    double xdiff = 1.0f * (posX - m_mousePrevX);
    double ydiff = 1.0f * (posY - m_mousePrevY);

    glm::dvec4 moveInCameraCoord(xdiff,ydiff,0.0,0.0);
    glm::dvec4 moveInWorldCoord =
        inverse(dmat4view) *
        moveInCameraCoord;

    rotCenter = rotCenter - xyz(moveInWorldCoord);

    UpdateViewMatrix();
}



dquat CameraTrial::RotationFromScreenMove(ScreenMove& move, bool reverseAngle)
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
    dvec4 axisInWorldSpace(inverse(dmat4view) * dvec4(axis,0.0));

    axis = normalize(xyz(axisInWorldSpace));
    double angle = glm::angle(v1,v2);
    if(reverseAngle) angle = -angle;
//    cout<<"\nangle"<<axis.x<<", "<<axis.y<<", "<<axis.z;
    return angleAxis(angle, axis);
}
void CameraTrial::MouseRotation(int fromX, int fromY, int toX, int toY)
{
    if ( fromX == toX && fromY == toY )
        return; //no rotation

    ScreenMove move;
    move.fromX = fromX;
    move.fromY = fromY;
    move.toX = toX;
    move.toY = toY;
    bool reverseAngle = true;
    dquat q_diff = RotationFromScreenMove(move,reverseAngle);

    q_rotation = q_diff * q_rotation;

    UpdateViewMatrix();
}
void CameraTrial::MoveBackForWard(int distance)
{
    double factor = 1.05;
    if(distance < 0) {
        m_scale *= factor;
    } else m_scale /= factor;
    m_farD = m_nearD + camDistance;
//    MyPerspective(m_fov, aspect, m_nearD, m_farD, m_dProj);
    UpdateViewMatrix();
}
