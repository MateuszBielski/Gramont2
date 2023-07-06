#include "OglRendererMock.h"

OglRendererMock::OglRendererMock()
{
}

OglRendererMock::~OglRendererMock()
{
}
OglRendererProgress OglRendererMock::DrawTextureForSingleModelEntry(const unsigned int vao, TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId)
{
    float sum = 0;
    if(m_viewParamsfv.matMVP)
        for(short y = 0 ; y < 16 ; y++)sum += m_viewParamsfv.matMVP[y];
    sumsOfmatMVP.push(sum);
    return OglRendererProgress::Completed;
}
stack<float>& OglRendererMock::getSumsOfmatMVP()
{
    return sumsOfmatMVP;
}
