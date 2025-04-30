#ifndef OneColourRenderSystem_H
#define OneColourRenderSystem_H

#include <iostream>
#include <memory>
#include <map>
#include "rendersystem.h"
#include "onecolourpreproc.h"


using std::map;

class OneColourRenderSystem : public RenderSystem
{
private:
    unsigned int loc[(size_t)oneColShUnif::oneColShUnifSize];
public:
    OneColourRenderSystem();
    virtual bool ConfigureShadersAndLocations();
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
    void Draw(spOneModel ) override;
protected:

};
using spOneColourRenderSystem = std::shared_ptr<OneColourRenderSystem>;
using upOneColourRenderSystem = std::unique_ptr<OneColourRenderSystem>;
#endif // OneColourRenderSystem_H
