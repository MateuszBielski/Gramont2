#ifndef MatrixStackMock_H
#define MatrixStackMock_H

#include <iostream>
#include <memory>
#include <stack>
#include "matrixstack.h"


class MatrixStackMock : public MatrixStack
{
private:
    std::stack<glm::dmat4 *> stackGivenModelMatrices;
public:
    virtual void setModelGlmMatrixdv(glm::dmat4 *) override;
    std::stack<glm::dmat4 *>& getStackGivenModelMatricesRef(){return stackGivenModelMatrices;}
protected:

};
using spMatrixStackMock = std::shared_ptr<MatrixStackMock>;
using upMatrixStackMock = std::unique_ptr<MatrixStackMock>;
#endif // MatrixStackMock_H
