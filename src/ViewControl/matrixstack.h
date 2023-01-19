#ifndef MatrixStack_H
#define MatrixStack_H
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;


class MatrixStack
{
public:
    MatrixStack();
    virtual ~MatrixStack();
    void setModelMatrixdv(const double *, bool *);
//    void setViewMatrixdv(const double *, bool *);
    void setProjectionMatrixdv(const double *, bool *);
//    void setCamModeMatrixdv(const double *, bool *);
    void setViewGlmMatrixdv(glm::dmat4x4 *);

    void UpdateMatrices();

    const float * getModelViewProjectionMatrixfv();
    const float * getViewMatrixfv();
    glm::dmat4x4 * getViewGlmMatrixv();
#ifdef TESTOWANIE_F
//    const double * getViewMatrixdv(){return viewMat;}
    const double * getProjMatrixdv(){return projMat;}
//    const double * getCamModeMatrixdv(){return camModeMat;}
#endif
protected:
    const double * modelMat = nullptr;
//    const double * viewMat = nullptr;
    const double * projMat = nullptr;
//    const double * camModeMat = nullptr;
    glm::dmat4x4 * viewGlmMatv = nullptr;
    bool * needUpd_model, * needUpd_proj, * needUpd_camMode;
    float modelViewProjectionMatrix[16];
    float viewMatrix[16];
    bool modelSetted = false, viewSetted = false, projectionSetted = false, camModeMatSetted = false;
private:
    glm::dmat4x4 m_dToVw, m_dMVP;

};
using spMatrixStack = shared_ptr<MatrixStack>;
using upMatrixStack = unique_ptr<MatrixStack>;
#endif // MatrixStack_H
