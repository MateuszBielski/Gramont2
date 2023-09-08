#ifndef ParalaxOclusionMapRenderSystem_H
#define ParalaxOclusionMapRenderSystem_H

#include <iostream>
#include <memory>
#include "rendersystem.h"
#include "pompreproc.h"


//https://shot511.github.io/2018-10-12-parallax-mapping/
//obliczanie Tangent i Bitangent
//https://shot511.github.io/2018-10-10-normal-mapping/
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/5.3.parallax_occlusion_mapping/5.3.parallax_mapping.vs
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/5.3.parallax_occlusion_mapping/5.3.parallax_mapping.fs

class ParalaxOclusionMapRenderSystem : public RenderSystem
{
private:

public:
    ParalaxOclusionMapRenderSystem();
    virtual bool ConfigureShadersAndLocations();
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
protected:
    
};
using spParalaxOclusionMapRenderSystem = std::shared_ptr<ParalaxOclusionMapRenderSystem>;
using upParalaxOclusionMapRenderSystem = std::unique_ptr<ParalaxOclusionMapRenderSystem>;
#endif // ParalaxOclusionMapRenderSystem_H
