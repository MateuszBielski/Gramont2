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
void CameraTrial::ViewSizeChanged(int newWidth, int newHeight)
{
    myOGLCamera::ViewSizeChanged(newWidth,newHeight);
    needUpdateProjMat = true;
}
void CameraTrial::UpdateViewMatrix()
{
    dvec3 newPosition = position;
    transformation = scale(dmat4x4(1.0),dvec3(m_scale,m_scale,m_scale));
    newPosition = xyz(transformation * dvec4(position,0.0));
    
    transformation = toMat4(q_rotation);
    newPosition = xyz(transformation * dvec4(newPosition,0.0));
    dvec3 newCamUp = xyz(transformation * dvec4(camUp,0.0));
    dvec3 newTarget = xyz(transformation * dvec4(target,0.0));
    
    dmat4view = lookAt(newPosition + rotCenter,newTarget + rotCenter,newCamUp);
    camDistance = glm::distance(newPosition,newTarget);
    
}

void CameraTrial::MoveOnSreenPlane(int m_mousePrevX,int  m_mousePrevY,int  posX, int posY)
{
    double xdiff = 1.0d * (posX - m_mousePrevX);
    double ydiff = 1.0d * (posY - m_mousePrevY);

    glm::dvec4 moveInCameraCoord(xdiff,ydiff,0.0,0.0);
    glm::dvec4 moveInWorldCoord = 
    inverse(dmat4view) * 
    moveInCameraCoord;
    
    rotCenter = rotCenter - xyz(moveInWorldCoord);
    
    UpdateViewMatrix();
}



dquat CameraTrial::RotationFromScreenMove(ScreenMove& move)
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
//    cout<<"\nangle"<<axis.x<<", "<<axis.y<<", "<<axis.z;
    return angleAxis(-angle, axis);
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
    dquat q_diff = RotationFromScreenMove(move);

    q_rotation = q_diff * q_rotation;
    
    UpdateViewMatrix();
}
void CameraTrial::MoveBackForWard(int distance)
{
	double factor = 1.05;
    if(distance < 0)
    {
        m_scale *= factor;
    }
    else m_scale /= factor;
    m_farD = m_nearD + camDistance;
    MyPerspective(m_fov, aspect, m_nearD, m_farD, m_dProj);
    UpdateViewMatrix();
}
void CameraTrial::UpdateViewMatrixTwoMatrices()
{
    transformation = dmat4x4(1.0d);
    transformation = translate(transformation, target);
    transformation = transformation * toMat4(q_rotation);
    transformation = translate(transformation, -target);

    dmat4view = lookAt(position,target,camUp);
    dmat4view = dmat4view * transformation;
}
void CameraTrial::UpdateViewMatrixCenterOfRotNotStable()
{
    dvec3 newCamUp = xyz(toMat4(q_rotation) * dvec4(camUp,0.0));
    dvec3 newTarget = xyz(toMat4(q_rotation) * dvec4(target,0.0));
    
    transformation = dmat4x4(1.0d);
    transformation = translate(transformation, newTarget);
    transformation = transformation * toMat4(q_rotation);
    transformation = translate(transformation, -newTarget);
    
    dvec3 newPosition = xyz(transformation * dvec4(position,0.0));
    
    
    dmat4view = lookAt(newPosition,newTarget,newCamUp);
}

