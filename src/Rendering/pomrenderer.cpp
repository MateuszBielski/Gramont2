#include "pomrenderer.h"
#include "pompreproc.h"

OglRendererProgress PomRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId )
{
    ++startCallCount;
    auto vao = model->getVao();
    if(!vao) return OglRendererProgress::VaoNotInited;
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
    glUniformMatrix4fv(shadUnifLocations[(size_t)pomShUnif::model], 1, GL_FALSE, m_viewParamsfv.matModel);
    glUniformMatrix4fv(shadUnifLocations[(size_t)pomShUnif::mMVP], 1, GL_FALSE, m_viewParamsfv.matMVP);
//    glUniformMatrix4fv(m_loc.mToViewSpace, 1, GL_FALSE, m_viewParamsfv.matToVw);
    glUniform4fv(shadUnifLocations[(size_t)pomShUnif::lightPos], 1, m_viewParamsfv.light_position);
//    glUniform3fv(m_loc.lightColour, 1, m_viewParamsfv.light_colour);
    glUniform3fv(shadUnifLocations[(size_t)pomShUnif::viewPos], 1, m_viewParamsfv.viewPosition);

    auto& tex = *model->MyTexture();
    auto d = model->GetModelData();
    
    //są trzy tekstury:
//    diffuseMap;
//    normalMap;
//    depthMap;
    //dla każdej tekstury potrzebne są te trzy
    glActiveTexture(GL_TEXTURE0 + tex.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, tex.getTextureId());
    glUniform1i(shadUnifLocations[(size_t)pomShUnif::diffuseMap], tex.getTextureUnit());// tu nie może być getTextureId(), czy więc getTextureUnit() jest unikalne dla każdej tekstury?
    
    auto& texNormalMap = *model->getTextureOfType(TextureForModel::Normal);
    glActiveTexture(GL_TEXTURE0 + texNormalMap.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, texNormalMap.getTextureId());
    glUniform1i(shadUnifLocations[(size_t)pomShUnif::normalMap], texNormalMap.getTextureUnit());
    
    auto& texHeightMap = *model->getTextureOfType(TextureForModel::Height);
    glActiveTexture(GL_TEXTURE0 + texHeightMap.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, texHeightMap.getTextureId());
    glUniform1i(shadUnifLocations[(size_t)pomShUnif::depthMap], texHeightMap.getTextureUnit());
    
    /*******/
    return OglRendererProgress::BeforeOgl;
    /*******/
    
    DrawIndicesAndFinish(d);
    return OglRendererProgress::Completed;
}
