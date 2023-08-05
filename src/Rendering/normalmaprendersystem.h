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

class NormalMapRenderSystem : public RenderSystem
{
private:

public:
    NormalMapRenderSystem();
    virtual bool ConfigureShadersAndLocations();
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
protected:
    
};
using spNormalMapRenderSystem = std::shared_ptr<NormalMapRenderSystem>;
using upNormalMapRenderSystem = std::unique_ptr<NormalMapRenderSystem>;
#endif // NormalMapRenderSystem_H
