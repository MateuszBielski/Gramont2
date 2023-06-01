#ifndef RenderSystemMock_H
#define RenderSystemMock_H

#include <iostream>
#include <memory>
#include "rendersystem.h"
#include <list>

class RenderSystemMock : public RenderSystem
{
private:
    std::list<ModelData *> adressesOfModelDataLoaded;
    std::list<TextureForModel *> adressesOfTextureLoaded;

public:
    bool ConfigureShadersAndLocations(){return true;};
//    virtual void ReloadVAO(ModelData& d, TextureForModel& tex) override;
    virtual void ReloadVAO(spOneModel ) override;
    bool ReloadedVAOforModelData(ModelData& d);
    bool ReloadedVAOforTexture(TextureForModel&);
protected:

};
using spRenderSystemMock = std::shared_ptr<RenderSystemMock>;
using upRenderSystemMock = std::unique_ptr<RenderSystemMock>;
#endif // RenderSystemMock_H
