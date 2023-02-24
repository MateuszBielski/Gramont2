#include "pickingrenderer.h"

void PickingRenderer::setViewMatrices(spMatrixStack ms)
{
    m_matrices.matMVP = ms->getModelViewProjectionMatrixfv();
}
void PickingRenderer::setLocationsFrom(spMyOGLShaders shader)
{
    m_loc.mMVP = shader->GetUnifLoc("gWVP");
    m_drawIndexLocation = shader->GetUnifLoc("gDrawIndex");
    m_objectIndexLocation = shader->GetUnifLoc("gObjectIndex");
   
}
OglRendererProgress PickingRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId)
{
	startCallCount++;
    glUseProgram(gl_ProgramId);
    glBindVertexArray(model->getVAOforSelect());
    
    glUniformMatrix4fv(m_loc.mMVP, 1, GL_TRUE, m_matrices.matMVP);
    glUniform1i(m_drawIndexLocation, m_drawIndex);
    glUniform1i(m_objectIndexLocation, model->getUniqueId());
    ModelData& d = model->GetModelData();
    glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);

    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
    return OglRendererProgress::Completed;
}

