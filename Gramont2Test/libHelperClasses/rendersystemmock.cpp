#include "rendersystemmock.h"


void RenderSystemMock::ReloadVAO(spOneModel model)
{
//    auto& d = model->GetModelData();
//	auto& tex = *model->MyTexture();
    auto adrMd = &model->GetModelData();
	auto adrTe = model->MyTexture().get();
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
