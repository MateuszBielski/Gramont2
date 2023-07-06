#ifndef PomRenderer_H
#define PomRenderer_H
#include "oglrenderer.h"
//#include <iostream>
//#include <memory>

class PomRenderer : public OglRenderer
{
private:

public:
    virtual OglRendererProgress DrawModel(spOneModel, unsigned int  );
protected:

};
using spPomRenderer = std::shared_ptr<PomRenderer>;
using upPomRenderer = std::unique_ptr<PomRenderer>;
#endif // PomRenderer_H
