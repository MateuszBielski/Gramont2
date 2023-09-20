#include "matrixstack.h"
#include "mathstuff.h"
#include "oglstuff.h"

using namespace std;

MatrixStack::MatrixStack():modelViewProjectionMatrix {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
} {
    m_dToVw = glm::dmat4x4(1.0f);
}

MatrixStack::~MatrixStack()
{

}
//
//void MatrixStack::setModelMatrixdv(const double * modelM, bool * b)
//{
////    modelMat = modelM;
//    modelSetted = true;
//    needUpd_model = b;
//    *needUpd_model = true;
//}
void MatrixStack::setModelGlmMatrixdv(glm::dmat4x4 * modelM)
{
    modelGlmMatv = modelM;
    SetAsGLFloat4x4(glm::value_ptr(*modelM),modelMatrix,16);
//    modelSetted = true;
}
void MatrixStack::setViewGlmMatrixdv(glm::dmat4x4 * viewM)
{
    viewGlmMatv = viewM;
}
void MatrixStack::setProjectionGlmMatrixdv(glm::dmat4x4* projM)
{
    projGlmMatv = projM;
}

void MatrixStack::setProjectionMatrixdv(const double * projM, bool * b)
{
//    projGlmMatv = projM;
    projectionSetted = true;
    needUpd_proj = b;
    *needUpd_proj = true;
}
void MatrixStack::setInvViewGlmMatrixdv(glm::dmat4x4* mat)
{
    invViewGlmMatv = mat;
}
void MatrixStack::UpdateMatrices()
{
    bool ok = true;
    ok &= modelGlmMatv != nullptr;
    ok &= viewGlmMatv != nullptr;
    ok &= projGlmMatv != nullptr;
    if(!ok)return;

    bool updateView = true;
//    bool updateView = false;
//    updateView |= *needUpd_model;
//    updateView |= *needUpd_view;

        if(updateView) {
        m_dToVw = *modelGlmMatv;
        m_dToVw =  *viewGlmMatv * m_dToVw;
//        cout<<"\nms ";
//        for(short i = 0; i < 16 ; i++)cout<<viewMat[i]<<", ";
        SetAsGLFloat4x4(glm::value_ptr(m_dToVw), viewMatrix, 16);
        invModelViewGlmMat = (*invViewGlmMatv) * inverse(*modelGlmMatv);
//        invModelViewGlmMat = inverse(*modelGlmMatv) * (*invViewGlmMatv);
//        invModelViewGlmMat = (*invViewGlmMatv);
        SetAsGLFloat4x4(glm::value_ptr(m_dToVw), invModelViewMatrix, 16);
    }

    bool updateMVP = updateView;
//    updateMVP |= *needUpd_proj;

    if(updateMVP) {
        m_dMVP = *projGlmMatv * m_dToVw;
        SetAsGLFloat4x4(glm::value_ptr(m_dMVP), modelViewProjectionMatrix, 16);
    }

//    *needUpd_model = false;
//    *needUpd_view = false;
//    *needUpd_proj = false;
}


const float* MatrixStack::getModelViewProjectionMatrixfv()
{
    return modelViewProjectionMatrix;
}
const float* MatrixStack::getViewMatrixfv()
{
    return viewMatrix;
}
const float* MatrixStack::getModelMatrixfv()
{
	return modelMatrix;
}
const float* MatrixStack::getInvModelViewMatrixfv()
{
	return invModelViewMatrix;
}

//glm::dmat4x4* MatrixStack::getViewGlmMatrixv()
//{
//    return viewGlmMatv;
//}
