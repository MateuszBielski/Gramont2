#include "cameratrial.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



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
        for(short i = 0 ; i < 16 ; i++)
        {
            sum += m_fMVP[i];
        }
        if(sum > 0.0f){
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

