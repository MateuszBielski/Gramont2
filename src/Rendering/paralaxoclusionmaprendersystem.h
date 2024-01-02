#ifndef ParalaxOclusionMapRenderSystem_H
#define ParalaxOclusionMapRenderSystem_H

#include <iostream>
#include <memory>
#include <map>
#include "rendersystem.h"
#include "pompreproc.h"


//https://shot511.github.io/2018-10-12-parallax-mapping/
//obliczanie Tangent i Bitangent
//https://shot511.github.io/2018-10-10-normal-mapping/
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/5.3.parallax_occlusion_mapping/5.3.parallax_mapping.vs
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/5.3.parallax_occlusion_mapping/5.3.parallax_mapping.fs

using std::map;

class ParalaxOclusionMapRenderSystem : public RenderSystem
{
private:
    map<unsigned, unsigned> tbnInversedBufferIdForModel;
    vec_for_subbuf CalculateInversedMatricesForModel(spOneModel model);
public:
    ParalaxOclusionMapRenderSystem();
    virtual bool ConfigureShadersAndLocations();
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
    void Draw(spOneModel ) override;
protected:

};
using spParalaxOclusionMapRenderSystem = std::shared_ptr<ParalaxOclusionMapRenderSystem>;
using upParalaxOclusionMapRenderSystem = std::unique_ptr<ParalaxOclusionMapRenderSystem>;
#endif // ParalaxOclusionMapRenderSystem_H
