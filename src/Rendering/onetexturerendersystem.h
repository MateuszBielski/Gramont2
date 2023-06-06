#ifndef OneTextureRenderSystem_H
#define OneTextureRenderSystem_H
#include <iostream>
#include <memory>
#include "rendersystem.h"


class OneTextureRenderSystem : public RenderSystem
{
  public:
    OneTextureRenderSystem();
    virtual ~OneTextureRenderSystem();
	virtual bool ConfigureShadersAndLocations() override;
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
protected:

private:
        
};
using spOneTextureRenderSystem = std::shared_ptr<OneTextureRenderSystem>;
using upOneTextureRenderSystem = std::unique_ptr<OneTextureRenderSystem>;
#endif // OneTextureRenderSystem_H
