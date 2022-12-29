#ifndef MatrixStack_H
#define MatrixStack_H
#include <iostream>
#include <memory>

using namespace std;


class MatrixStack
{
public:
    MatrixStack();
    virtual ~MatrixStack();
    void setModelMatrixdv(const double *);
    void setViewMatrixdv(const double *);
    void setProjectionMatrixdv(const double *);
    void setNeedUpdateModelMat(bool *);
    void setNeedUpdateViewMat(bool *);
    void setNeedUpdateProjectionMat(bool *);

    void UpdateMatrices();

    const float * getModelViewProjectionMatrixfv();
    const float * getViewMatrixfv();
#ifdef TESTOWANIE_F
    const double * getViewMatrixdv(){return viewMat;}
    const double * getProjMatrixdv(){return projMat;}
#endif
protected:
    const double * modelMat = nullptr;
    const double * viewMat = nullptr;
    const double * projMat = nullptr;
    bool * needUpd_model, * needUpd_view, * needUpd_proj;
    float modelViewProjectionMatrix[16];
    float viewMatrix[16];
    bool modelSetted = false, viewSetted = false, projectionSetted = false;
private:

};
using spMatrixStack = shared_ptr<MatrixStack>;
using upMatrixStack = unique_ptr<MatrixStack>;
#endif // MatrixStack_H
