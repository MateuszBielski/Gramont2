#include "pickingrenderer.h"

void PickingRenderer::setViewMatrices(spMatrixStack ms)
{
    m_matrices.matMVP = ms->getModelViewProjectionMatrixfv();
}
void PickingRenderer::setLocationsFrom(spMyOGLShaders shader)
{
    m_loc.mMVP = shader->GetUnifLoc("mMVP");
    m_objectIndexLocation = shader->GetUnifLoc("modelUniqueId");
}
OglRendererProgress PickingRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId)
{
    startCallCount++;
    ModelData& d = model->GetModelData();
    
    glUseProgram(gl_ProgramId);
    glBindVertexArray(model->getVAOforSelect());

    glUniformMatrix4fv(m_loc.mMVP, 1, GL_FALSE, m_matrices.matMVP);
    glUniform1i(m_objectIndexLocation, model->getUniqueId() + BACKGROUND_COMPENSATION);

    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    
    glBindVertexArray(0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}
