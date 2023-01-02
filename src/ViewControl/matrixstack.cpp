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

void MatrixStack::setModelMatrixdv(const double * modelM, bool * b)
{
    modelMat = modelM;
    modelSetted = true;
    needUpd_model = b;
    *needUpd_model = true;
}


void MatrixStack::setViewMatrixdv(const double * viewM, bool * b)
{
    viewMat = viewM;
    viewSetted = true;
    needUpd_view = b;
    *needUpd_view = true;
}
void MatrixStack::setProjectionMatrixdv(const double * projM, bool * b)
{
    projMat = projM;
    projectionSetted = true;
    needUpd_proj = b;
    *needUpd_proj = true;
}
void MatrixStack::UpdateMatrices()
{
    bool ok = true;
    ok &= modelSetted;
    ok &= viewSetted;
    ok &= projectionSetted;
    if(!ok)return;
    
    double m_dToVw[16];
    double m_dMVP[16];
    
    bool updateView = false;
    updateView |= *needUpd_model;
    updateView |= *needUpd_view;
    
    if(updateView) {
        MyMatMul4x4(viewMat, modelMat, m_dToVw);
        SetAsGLFloat4x4(m_dToVw, viewMatrix, 16);
    }
    
    bool updateMVP = updateView;
    updateMVP |= *needUpd_proj;
    
    if(updateMVP) {
        MyMatMul4x4(projMat, m_dToVw, m_dMVP);
        SetAsGLFloat4x4(m_dMVP, modelViewProjectionMatrix, 16);
    }

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
