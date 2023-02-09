#include "matrixstackmock.h"

using namespace std;

void MatrixStackMock::setModelGlmMatrixdv(glm::dmat4 * mat)
{
    stackGivenModelMatrices.push(mat);
}
