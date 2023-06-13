#ifndef ParalaxOclusionMapRenderSystem_H
#define ParalaxOclusionMapRenderSystem_H

#include <iostream>
#include <memory>
#include "rendersystem.h"

//https://cplusplus.com/articles/2wA0RXSz/
const vector<string> explode(const string& s, const char& c);

#define POM_SH_ATTR aPos, aNormal, aTexCoords, aTangent, aBitangent, pomShAttrSize
#define POM_SH_UNIF projection, view, model, lightPos, viewPos, pomShUnifSize
#define MA_CreateEnum( className, ...) enum class className { __VA_ARGS__};
#define Q(...) #__VA_ARGS__
#define MA_CreateStrings( varName, ...) const vector<string> varName = explode( Q(__VA_ARGS__), ',');

MA_CreateEnum(pomShAttr, POM_SH_ATTR)
MA_CreateEnum(pomShUnif, POM_SH_UNIF)



class ParalaxOclusionMapRenderSystem : public RenderSystem
{
private:

public:
    virtual bool ConfigureShadersAndLocations();
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
protected:

};
using spParalaxOclusionMapRenderSystem = std::shared_ptr<ParalaxOclusionMapRenderSystem>;
using upParalaxOclusionMapRenderSystem = std::unique_ptr<ParalaxOclusionMapRenderSystem>;
#endif // ParalaxOclusionMapRenderSystem_H
