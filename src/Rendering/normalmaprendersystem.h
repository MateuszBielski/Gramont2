#ifndef NormalMapRenderSystem_H
#define NormalMapRenderSystem_H

#include <iostream>
#include <memory>
#include "rendersystem.h"
#include "normalpreproc.h"



//https://shot511.github.io/2018-10-12-parallax-mapping/
//obliczanie Tangent i Bitangent
//https://shot511.github.io/2018-10-10-normal-mapping/
//code: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/4.normal_mapping/normal_mapping.cpp
//code verex: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/4.normal_mapping/4.normal_mapping.vs
//code fragment: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/4.normal_mapping/4.normal_mapping.fs

#define NORMAL_SH_UNIF model, mMVP, mToViewSpace, lightColour, lightProps, lightPos, viewPos, diffuseMap, normalMap, normalEnabled, normalShUnifSize
MA_CreateEnum(normalShUnif, NORMAL_SH_UNIF)

class NormalMapRenderSystem : public RenderSystem
{
private:
//    using numberOfUniforms = (size_t)normalShUnif::normalShUnifSize;
//    using numberOfUniforms = (size_t)normalShUnif::normalShUnifSize;
    unsigned int loc_mMVP = 0;
    unsigned int loc_mToViewSpace = 0;
    unsigned int loc_lightProps = 0;
    unsigned int loc_lightColour = 0;
    unsigned int loc_diffuseMap = 0;
    unsigned int loc_normalMap = 0;
    unsigned int loc_normalEnabled = 0;

//    unsigned int loc[numberOfUniforms];
    unsigned int loc[(size_t)normalShUnif::normalShUnifSize];
public:
    NormalMapRenderSystem();
    bool ConfigureShadersAndLocations() override;
    void CreateGraphicBuffers(spOneModel model) override;
    void LoadVAO(spOneModel )  override;
    void Draw(spOneModel ) override;
protected:

};
using spNormalMapRenderSystem = std::shared_ptr<NormalMapRenderSystem>;
using upNormalMapRenderSystem = std::unique_ptr<NormalMapRenderSystem>;
#endif // NormalMapRenderSystem_H
