#include "rendersystemmock.h"


void RenderSystemMock::ReloadVAO(ModelData& d, TextureForModel& tex)
{
	auto adrMd = &d;
	auto adrTe = &tex;
    adressesOfModelDataLoaded.push_back(adrMd);
    adressesOfTextureLoaded.push_back(adrTe);
}
bool RenderSystemMock::ReloadedVAOforModelData(ModelData& d)
{
	for(auto& adr : adressesOfModelDataLoaded)
    {
        if (adr == &d) return true;
    }
    return false;
}
bool RenderSystemMock::ReloadedVAOforTexture(TextureForModel& t)
{
	for(auto& adr : adressesOfTextureLoaded)
    {
        if (adr == &t) return true;
    }
	return false;
}
