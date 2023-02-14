#ifndef PickingRenderer_H
#define PickingRenderer_H

#include <iostream>
#include <memory>
#include "oglrenderer.h"

class PickingRenderer : public OglRenderer
{
private:

public:
    virtual OglRendererProgress DrawTextureForSingleModelEntry(TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId) override;
protected:

};
using spPickingRenderer = std::shared_ptr<PickingRenderer>;
using upPickingRenderer = std::unique_ptr<PickingRenderer>;
#endif // PickingRenderer_H
