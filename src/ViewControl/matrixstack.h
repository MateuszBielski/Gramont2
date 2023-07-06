#ifndef MatrixStack_H
#define MatrixStack_H
#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MatrixStack
{
public:
    MatrixStack();
    virtual ~MatrixStack();
//    void setModelMatrixdv(const double *, bool *);//-
    void setProjectionMatrixdv(const double *, bool *);//-
    void setViewGlmMatrixdv(glm::dmat4x4 *);
    void setProjectionGlmMatrixdv(glm::dmat4x4 *);
#ifdef TESTOWANIE_F
    virtual
#endif
    void setModelGlmMatrixdv(glm::dmat4 *);
    void UpdateMatrices();

    const float * getModelViewProjectionMatrixfv();
    const float * getViewMatrixfv();
    const float * getModelMatrixfv();
    glm::dmat4x4 * getViewGlmMatrixdv() {return viewGlmMatv;}
#ifdef TESTOWANIE_F
    glm::dmat4x4 * getProjGlmMatrixdv() {return projGlmMatv;}
#endif
protected:
    glm::dmat4x4 * modelGlmMatv = nullptr;
    glm::dmat4x4 * viewGlmMatv = nullptr;
    glm::dmat4x4 * projGlmMatv = nullptr;
    bool * needUpd_model, * needUpd_proj, * needUpd_camMode;
    float modelViewProjectionMatrix[16];
    float viewMatrix[16];
    float modelMatrix[16];
    bool modelSetted = false, viewSetted = false, projectionSetted = false, camModeMatSetted = false;
private:
    glm::dmat4x4 m_dToVw, m_dMVP;

};
using spMatrixStack = std::shared_ptr<MatrixStack>;
using upMatrixStack = std::unique_ptr<MatrixStack>;
#endif // MatrixStack_H
