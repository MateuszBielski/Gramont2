#ifndef ModelManager_H
#define ModelManager_H
#include "myoglmanager.h"
#include "bufferloader.h"
#include "oglrenderer.h"

#include <iostream>
#include <memory>
#include <vector>

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
//    ModelManager(myOGLErrHandler* extErrHnd);
    ModelManager();
//    ~ModelManager();
#ifdef TESTOWANIE_F
    upBufferLoader getBufferLoaderForTest() {
        return move(m_BufferLoader);
    };
    upOglRenderer getTexRendererForTest() {
        return move(m_TexRenderer);
    };
    myOGLCamera& getCameraRefForTest() {
        return *m_Camera;
    }
#endif

protected:
    upBufferLoader m_BufferLoader;//should be settable
    upOglRenderer m_TexRenderer;//j/w
    upOglRenderer m_OglRenderer;

    virtual void setMatricesForRender(upOglRenderer& );
    template<typename T>
    void setLocations(unique_ptr<T>&,  vec_locations_T<T>, myOGLShaders&, sha_FunGetStr );

};
//using spModelManager = std::shared_ptr<ModelManager>;
//using upModelManager = std::unique_ptr<ModelManager>;
#endif // ModelManager_H
