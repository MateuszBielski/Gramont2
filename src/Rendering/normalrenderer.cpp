#include "normalpreproc.h"
#include "normalrenderer.h"

OglRendererProgress NormalRenderer::DrawModel(spOneModel model, unsigned int gl_ProgramId )
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
//    glUniformMatrix4fv(shadUnifLocations[(size_t)normalShUnif::model], 1, GL_FALSE, m_viewParamsfv.matModel);
    glUniformMatrix4fv(shadUnifLocations[(size_t)normalShUnif::mMVP], 1, GL_FALSE, m_viewParamsfv.matMVP);
    glUniformMatrix4fv(shadUnifLocations[(size_t)normalShUnif::mToViewSpace], 1, GL_FALSE, m_viewParamsfv.matToVw);
    //glUniform3fv(shadUnifLocations[(size_t)normalShUnif::lightPos], 1, m_viewParamsfv.light_position);//first three is position
    glUniform4fv(shadUnifLocations[(size_t)normalShUnif::lightProps], 1, m_viewParamsfv.light_position);
    glUniform3fv(shadUnifLocations[(size_t)normalShUnif::lightColour], 1, m_viewParamsfv.light_colour);
    //glUniform3fv(shadUnifLocations[(size_t)normalShUnif::viewPos], 1, m_viewParamsfv.viewPosition);

    auto& tex = *model->MyTexture();
    auto d = model->GetModelData();

    glActiveTexture(GL_TEXTURE0 + tex.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, tex.getTextureId());
    glUniform1i(shadUnifLocations[(size_t)normalShUnif::diffuseMap], tex.getTextureUnit());
    
//    int normalEnabled = 1;
//    auto& texNormalMap = *model->getTextureOfType(TextureForModel::Normal);
//    if(texNormalMap.bufTexCoordId != (unsigned)-1) {
//        glActiveTexture(GL_TEXTURE0 + texNormalMap.getTextureUnit());
//        glBindTexture(GL_TEXTURE_2D, texNormalMap.getTextureId());
//        glUniform1i(shadUnifLocations[(size_t)normalShUnif::normalMap], texNormalMap.getTextureUnit());
//    }else{normalEnabled = 0;}
//
//    glUniform1i(shadUnifLocations[(size_t)normalShUnif::normalEnabled], (int)normalEnabled);
    /*******/
//    return OglRendererProgress::BeforeOgl;
    /*******/

    DrawIndicesAndFinish(d);
    return OglRendererProgress::Completed;
}
