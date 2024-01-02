#ifndef NormalRenderer_H
#define NormalRenderer_H
#include "oglrenderer.h"
//#include <iostream>
//#include <memory>

class NormalRenderer : public OglRenderer
{
private:

public:
    virtual OglRendererProgress DrawModel(spOneModel, unsigned int  );
protected:

};
using spNormalRenderer = std::shared_ptr<NormalRenderer>;
using upNormalRenderer = std::unique_ptr<NormalRenderer>;
#endif // NormalRenderer_H
