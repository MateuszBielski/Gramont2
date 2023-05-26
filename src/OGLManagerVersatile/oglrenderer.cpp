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

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_matrices.matMVP);
    glUniformMatrix4fv(m_loc.mToViewSpace, 1, GL_FALSE, m_matrices.matToVw);
    glUniform4fv(m_loc.lightProps, 1, m_matrices.light_position);
    glUniform3fv(m_loc.lightColour, 1, m_matrices.light_colour);

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);

    glBindVertexArray(0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}
OglRendererProgress OglRenderer::DrawTextureForSingleModelEntry(const unsigned int vao, TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId)
{
    ++startCallCount;
    bool ok = true;
    ok &= (bool)gl_ProgramId;
    ok &= (bool)m_matrices.matMVP;
    ok &= (bool)m_matrices.matToVw;
    ok &= (bool)m_matrices.light_position;
    ok &= (bool)m_matrices.light_colour;
//    ok &= (bool)m_loc.mMVP;
//    ok &= (bool)m_loc.mToViewSpace;
//    ok &= (bool)m_loc.lightProps;
//    ok &= (bool)m_loc.lightColour;
    if(!ok)
        return OglRendererProgress::BeforeOgl;

    glUseProgram(gl_ProgramId);
//    glBindVertexArray(tex.textureVAO);
    glBindVertexArray(vao);

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_matrices.matMVP);
    glUniformMatrix4fv(m_loc.mToViewSpace, 1, GL_FALSE, m_matrices.matToVw);
    glUniform4fv(m_loc.lightProps, 1, m_matrices.light_position);
    glUniform3fv(m_loc.lightColour, 1, m_matrices.light_colour);
    //without two following models are black rendered
    //order is important
    glActiveTexture(GL_TEXTURE0 + tex.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, tex.getTextureId());

    glUniform1i(m_loc.stringTexture, tex.getTextureUnit());

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    // Unbind
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}
OglRendererProgress OglRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId)
{
    auto& tex = *model->MyTexture();
    auto d = model->GetModelData();
    auto vao = model->getVao();
    return DrawTextureForSingleModelEntry(vao,tex, d,gl_ProgramId);
}
void OglRenderer::setViewMatrices(spMatrixStack ms)
{
    m_matrices.matMVP = ms->getModelViewProjectionMatrixfv();
    m_matrices.matToVw = ms->getViewMatrixfv();
}
void OglRenderer::setLightMatrices(myLight* light)
{
    m_matrices.light_position = light->GetFLightPos();
    m_matrices.light_colour = light->GetFLightColour();
}
void OglRenderer::setLocationsFrom(spMyOGLShaders shader)
{
    m_loc.mMVP = shader->GetUnifLoc("mMVP");
    m_loc.mToViewSpace = shader->GetUnifLoc("mToViewSpace");
    m_loc.lightProps = shader->GetUnifLoc("lightProps");
    m_loc.lightColour = shader->GetUnifLoc("lightColour");
    m_loc.stringTexture = shader->GetUnifLoc("stringTexture");
}
