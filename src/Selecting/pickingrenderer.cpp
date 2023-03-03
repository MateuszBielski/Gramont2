#include "pickingrenderer.h"

void PickingRenderer::setViewMatrices(spMatrixStack ms)
{
    m_matrices.matMVP = ms->getModelViewProjectionMatrixfv();
    m_matrices.matToVw = ms->getViewMatrixfv();//to erase later
}
void PickingRenderer::setLocationsFrom(spMyOGLShaders shader)
{
    /*
    m_loc.mMVP = shader->GetUnifLoc("gWVP");
    m_drawIndexLocation = shader->GetUnifLoc("gDrawIndex");
    m_objectIndexLocation = shader->GetUnifLoc("gObjectIndex");
     * */
    m_loc.mMVP = shader->GetUnifLoc("mMVP");
    m_objectIndexLocation = shader->GetUnifLoc("modelUniqueId");
}
OglRendererProgress PickingRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId)
{
    startCallCount++;
    /*
    glUseProgram(gl_ProgramId);
    glBindVertexArray(model->getVAOforSelect());

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_TRUE, m_matrices.matMVP);
    glUniform1i(m_drawIndexLocation, m_drawIndex);
    glUniform1i(m_objectIndexLocation, model->getUniqueId()+3);
    ModelData& d = model->GetModelData();
    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
    */
    ModelData& d = model->GetModelData();
    TextureForModel& tex = *model->MyTexture();
    
    glUseProgram(gl_ProgramId);
    glBindVertexArray(model->getVAOforSelect());

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_matrices.matMVP);
    glUniform1i(m_objectIndexLocation, model->getUniqueId());

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    // Unbind
    glBindVertexArray(0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}
