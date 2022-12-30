#include "matrixstack.h"
#include "mathstuff.h"
#include "oglstuff.h"

MatrixStack::MatrixStack():modelViewProjectionMatrix {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
} {
}

MatrixStack::~MatrixStack()
{

}
void MatrixStack::setNeedUpdateModelMat(bool* b)
{
    needUpd_model = b;
}

//void MatrixStack::setModelMatrixdv(const double * modelM, bool * b)
//{
//    modelMat = modelM;
//    modelSetted = true;
//    needUpd_model = b;
//    *needUpd_model = true;
//}
void MatrixStack::setModelMatrixdv(const double * modelM)
{
    modelMat = modelM;
    modelSetted = true;
    *needUpd_model = true;
}

void MatrixStack::setViewMatrixdv(const double * viewM)
{
    viewMat = viewM;
    viewSetted = true;
    *needUpd_view = true;
}
void MatrixStack::setProjectionMatrixdv(const double * projM)
{
    projMat = projM;
    projectionSetted = true;
    *needUpd_proj = true;
}
void MatrixStack::UpdateMatrices()
{
    bool ok = true;
    ok &= modelSetted;
    ok &= viewSetted;
    ok &= projectionSetted;
//    ok &= (bool)needUpd_model;
//    ok &= (bool)needUpd_view;
//    ok &= (bool)needUpd_proj;
    if(!ok)return;
    double m_dToVw[16];
    double m_dMVP[16];
    if(*needUpd_model || *needUpd_view)
        MyMatMul4x4(viewMat, modelMat, m_dToVw);
    if(*needUpd_proj)
        MyMatMul4x4(projMat, m_dToVw, m_dMVP);
    SetAsGLFloat4x4(m_dMVP, modelViewProjectionMatrix, 16);
    *needUpd_model = false;
    *needUpd_view = false;
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



void MatrixStack::setNeedUpdateViewMat(bool* b)
{
    needUpd_view = b;
}

void MatrixStack::setNeedUpdateProjectionMat(bool* b)
{
    needUpd_proj = b;
}
