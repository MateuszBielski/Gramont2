#include "oglrenderer.h"

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
OglRendererProgress OglRenderer::DrawTextureForSingleModelEntry(TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId)
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
    glBindVertexArray(tex.textureVAO);

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_matrices.matMVP);
    glUniformMatrix4fv(m_loc.mToViewSpace, 1, GL_FALSE, m_matrices.matToVw);
    glUniform4fv(m_loc.lightProps, 1, m_matrices.light_position);
    glUniform3fv(m_loc.lightColour, 1, m_matrices.light_colour);
    glActiveTexture(GL_TEXTURE0 + tex.textureUnit);
    glBindTexture(GL_TEXTURE_2D, tex.textureId);
    glUniform1i(m_loc.stringTexture, tex.textureUnit);

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    // Unbind
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}
