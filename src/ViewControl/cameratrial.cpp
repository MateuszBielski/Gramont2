#include "cameratrial.h"


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

const double* CameraTrial::getViewMatrixdv()
{
//    return m_dMode;//cause of errors?
    return m_dView;
}

const double* CameraTrial::getProjMatrixdv()
{
    return m_dProj;
}
void CameraTrial::ViewSizeChanged(int newWidth, int newHeight)
{
    myOGLCamera::ViewSizeChanged(newWidth,newHeight);
    needUpdateProjMat = true;
}
