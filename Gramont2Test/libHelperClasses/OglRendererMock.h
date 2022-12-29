#ifndef OGLRENDERERMOCK_H
#define OGLRENDERERMOCK_H
#include "oglrenderer.h"
#include <stack>


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

using upOglRendererMock = unique_ptr<OglRendererMock>;
#endif // OGLRENDERERMOCK_H
