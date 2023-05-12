#ifndef ModelManager_H
#define ModelManager_H
#include "myoglmanager.h"
#include "bufferloader.h"
#include "oglrenderer.h"
#include "onetexturerendersystem.h"

#include <iostream>
#include <memory>
#include <vector>

using std::string, std::vector, std::tuple;

using OLoc = OglRenderer::Locations;
using t_OLoc_str = tuple<int OLoc::*,string>;
using BLoc = BufferLoader::Locations;
using t_BLoc_str = tuple<int BLoc::*,string>;

template<typename T>
using vec_locations_T = vector<tuple<int T::Locations::* , string>>;

class ModelManager : public myOGLManager
{
private:

public:
    ModelManager();
//    ~ModelManager();
    bool setAndConfigureRenderSystem(spRenderSystem);
#ifdef TESTOWANIE_F
    spBufferLoader getBufferLoaderForTest() {
        return m_BufferLoader;
    };
    spOglRenderer getTexRendererForTest() {
        return m_TexRenderer;
    };
    myOGLCamera& getCameraRefForTest() {
        return *m_Camera;
    }
#endif
    spMyOGLShaders getActiveShader();
    spOglRenderer getActiveRenderer();
protected:

    spRenderSystem m_renderSystem;

    spBufferLoader m_BufferLoader;
    spOglRenderer m_TexRenderer;//j/w
    spOglRenderer m_OglRenderer;

    spOglRenderer activeRenderer;
    spMyOGLShaders activeShader;


    template<typename T>
    void setLocations(std::shared_ptr<T>,  vec_locations_T<T>, myOGLShaders&, sha_FunGetStr );

};
//using spModelManager = std::shared_ptr<ModelManager>;
//using upModelManager = std::unique_ptr<ModelManager>;
#endif // ModelManager_H
