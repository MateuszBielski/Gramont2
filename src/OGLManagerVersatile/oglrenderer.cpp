#include "oglrenderer.h"
#include "multimodelmanager.h"

using namespace std;

OglRendererProgress OglRenderer::DrawSingleModelEntry(ModelData& d, unsigned int gl_ProgramId)
{
    bool ok = true;
    ok &= (bool)gl_ProgramId;
    if(!ok)
        return OglRendererProgress::BeforeOgl;

    glUseProgram(gl_ProgramId);
    glBindVertexArray(d.modelVAO);

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_viewParamsfv.matMVP);
    glUniformMatrix4fv(m_loc.mToViewSpace, 1, GL_FALSE, m_viewParamsfv.matToVw);
    glUniform4fv(m_loc.lightProps, 1, m_viewParamsfv.light_position);
    glUniform3fv(m_loc.lightColour, 1, m_viewParamsfv.light_colour);

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);

    glBindVertexArray(0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}
void OglRenderer::DrawIndicesAndFinish(ModelData& d)
{
	glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}
OglRendererProgress OglRenderer::DrawTextureForSingleModelEntry(const unsigned int vao, TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId)
{
    ++startCallCount;
    bool ok = true;
    ok &= (bool)gl_ProgramId;
    ok &= (bool)m_viewParamsfv.matMVP;
    ok &= (bool)m_viewParamsfv.matToVw;
    ok &= (bool)m_viewParamsfv.light_position;
    ok &= (bool)m_viewParamsfv.light_colour;
//    ok &= (bool)m_loc.mMVP;
//    ok &= (bool)m_loc.mToViewSpace;
//    ok &= (bool)m_loc.lightProps;
//    ok &= (bool)m_loc.lightColour;
    if(!ok)
        return OglRendererProgress::BeforeOgl;
    
    glUseProgram(gl_ProgramId);
//    glBindVertexArray(tex.textureVAO);
    glBindVertexArray(vao);
//    glUniformMatrix4fv(m_loc.mModel?, 1, GL_FALSE, m_viewParamsfv.matModel);
    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_viewParamsfv.matMVP);
    glUniformMatrix4fv(m_loc.mToViewSpace, 1, GL_FALSE, m_viewParamsfv.matToVw);
    glUniform4fv(m_loc.lightProps, 1, m_viewParamsfv.light_position);
    glUniform3fv(m_loc.lightColour, 1, m_viewParamsfv.light_colour);
//    glUniform3fv(m_loc.viewPosition?, 1, m_viewParamsfv.viewPosition);
    //without two following models are black rendered
    //order is important
    glBindTexture(GL_TEXTURE_2D, tex.getTextureId());
    glActiveTexture(GL_TEXTURE0 + tex.getTextureUnit());

    glUniform1i(m_loc.stringTexture, tex.getTextureUnit());
//    glUniform1i(m_loc.stringTexture, tex.getTextureId());//nie działa

    DrawIndicesAndFinish(d);
    return OglRendererProgress::Completed;
}
OglRendererProgress OglRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId)
{
    auto& tex = *model->MyTexture();
    auto d = model->GetModelData();
    auto vao = model->getVao();
//    if(!shadUnifLocations.size())
    return DrawTextureForSingleModelEntry(vao,tex, d,gl_ProgramId);
    
}
void OglRenderer::setViewMatrices(spMatrixStack ms)
{
    m_viewParamsfv.matMVP = ms->getModelViewProjectionMatrixfv();
    m_viewParamsfv.matToVw = ms->getViewMatrixfv();
    m_viewParamsfv.matModel = ms->getModelMatrixfv();
}
void OglRenderer::setLightMatrices(myLight* light)
{
    m_viewParamsfv.light_position = light->GetFLightPos();
    m_viewParamsfv.light_colour = light->GetFLightColour();
}
void OglRenderer::setLocationsFrom(spMyOGLShaders shader)
{
    m_loc.mMVP = shader->GetUnifLoc("mMVP");
    m_loc.mToViewSpace = shader->GetUnifLoc("mToViewSpace");
    m_loc.lightProps = shader->GetUnifLoc("lightProps");
    m_loc.lightColour = shader->GetUnifLoc("lightColour");
    m_loc.stringTexture = shader->GetUnifLoc("stringTexture");
}

