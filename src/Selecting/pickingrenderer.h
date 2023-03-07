#ifndef PickingRenderer_H
#define PickingRenderer_H

#include <iostream>
#include <memory>
#include "oglrenderer.h"
#define BACKGROUND_COMPENSATION 2

class PickingRenderer : public OglRenderer
{
private:
    unsigned int m_drawIndexLocation = 0;
    unsigned int m_objectIndexLocation = 0;
    int m_drawIndex = 988;
public:
    virtual OglRendererProgress DrawModel(spOneModel, unsigned int )override;
    virtual void setViewMatrices(spMatrixStack ) override;
    virtual void setLocationsFrom(spMyOGLShaders)override;
protected:

};
using spPickingRenderer = std::shared_ptr<PickingRenderer>;
using upPickingRenderer = std::unique_ptr<PickingRenderer>;
#endif // PickingRenderer_H
