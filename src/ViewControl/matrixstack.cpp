#include "matrixstack.h"
#include "mathstuff.h"
#include "oglstuff.h"

MatrixStack::MatrixStack():modelViewProjectionMatrix {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
} {
    m_dToVw = glm::dmat4x4(1.0d);
}

MatrixStack::~MatrixStack()
{

}

void MatrixStack::setModelMatrixdv(const double * modelM, bool * b)
{
    modelMat = modelM;
    modelSetted = true;
    needUpd_model = b;
    *needUpd_model = true;
}

//void MatrixStack::setCamModeMatrixdv(const double* camMode, bool* b)
//{
//    camModeMat = camMode;
//    camModeMatSetted = true;
//    needUpd_camMode = b;
//    //*needUpd_camMode = true; not secured by test
//}
//void MatrixStack::setViewMatrixdv(const double * viewM, bool * b)
//{
//    viewMat = viewM;
//    viewSetted = true;
//    needUpd_view = b;
//    *needUpd_view = true;
//}
void MatrixStack::setViewGlmMatrixdv(glm::dmat4x4 * viewM)
{
    viewGlmMatv = viewM;
}
void MatrixStack::setProjectionMatrixdv(const double * projM, bool * b)
{
    projMat = projM;
    projectionSetted = true;
    needUpd_proj = b;
    *needUpd_proj = true;
}
//glm::dmat4x4& MatrixStack::getViewGlmMatrixRef()
//{
//	return viewGlmMatrix;
//}
void MatrixStack::UpdateMatrices()
{
    bool ok = true;
    ok &= modelSetted;
//    ok &= viewSetted;
    ok &= projectionSetted;
    if(!ok)return;

    bool updateView = false;
    updateView |= *needUpd_model;
//    updateView |= *needUpd_view;

        if(updateView) {
        m_dToVw = glm::make_mat4x4(modelMat);
        m_dToVw =  *viewGlmMatv * m_dToVw;
//        cout<<"\nms ";
//        for(short i = 0; i < 16 ; i++)cout<<viewMat[i]<<", ";
        SetAsGLFloat4x4(glm::value_ptr(m_dToVw), viewMatrix, 16);
    }

    bool updateMVP = updateView;
    updateMVP |= *needUpd_proj;

    if(updateMVP) {
        m_dMVP = glm::make_mat4x4(projMat) * m_dToVw;
        SetAsGLFloat4x4(glm::value_ptr(m_dMVP), modelViewProjectionMatrix, 16);
    }

    *needUpd_model = false;
//    *needUpd_view = false;
    *needUpd_proj = false;
}


const float* MatrixStack::getModelViewProjectionMatrixfv()
{
    return modelViewProjectionMatrix;
}
const float* MatrixStack::getViewMatrixfv()
{
    return viewMatrix;
}

