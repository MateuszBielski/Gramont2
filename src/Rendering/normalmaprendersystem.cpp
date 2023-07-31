#include "normalmaprendersystem.h"
NormalMapRenderSystem::NormalMapRenderSystem()
{
//	m_renderer = std::make_unique<PomRenderer>();
}
bool NormalMapRenderSystem::ConfigureShadersAndLocations()
{
	return true;
}

void NormalMapRenderSystem::CreateGraphicBuffers(spOneModel model)
{
}

void NormalMapRenderSystem::LoadVAO(spOneModel)
{
}
