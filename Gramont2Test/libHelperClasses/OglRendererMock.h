#ifndef OGLRENDERERMOCK_H
#define OGLRENDERERMOCK_H
#include "oglrenderer.h"
#include <stack>

using std::stack;

class OglRendererMock : public OglRenderer
{
public:
    OglRendererMock();
    virtual ~OglRendererMock();
    virtual OglRendererProgress DrawTextureForSingleModelEntry(TextureForModel& tex, 
    ModelData& d, unsigned int gl_ProgramId) override;
    stack<float>& getSumsOfmatMVP();
private:
    stack<float> sumsOfmatMVP;
};

using upOglRendererMock = std::unique_ptr<OglRendererMock>;
using spOglRendererMock = std::shared_ptr<OglRendererMock>;
#endif // OGLRENDERERMOCK_H
