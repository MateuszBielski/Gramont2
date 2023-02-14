#include "pickingrenderer.h"


OglRendererProgress PickingRenderer::DrawTextureForSingleModelEntry(TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId)
{
	startCallCount++;
    return OglRendererProgress::Completed;
}
