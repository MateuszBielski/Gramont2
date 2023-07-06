/* %PATH% %FILE% */
#ifndef OglRenderer_H
#define OglRenderer_H
#include <memory>
#include "modeldata.h"
#include "textureformodel.h"
#include "bufferloader.h"
#include "matrixstack.h"
#include "oglstuff.h"

enum class OglRendererProgress
{
    Completed,
    BeforeOgl,
    VaoNotInited,
};
class MultiModelManager;

class OglRenderer
{
private:
    
public:
    struct ViewParamsfv {
        const float * matModel = nullptr;
        const float * matMVP = nullptr;
        const float * matToVw = nullptr;
        const float * light_position = nullptr;
        const float * light_colour = nullptr;
        const float * viewPosition = nullptr;
    };
    struct Locations {
        int mMVP = 0;
        int mToViewSpace = 0;
        int lightProps = 0;
        int lightColour = 0;
        int stringTexture = 0;
    };
    ViewParamsfv m_viewParamsfv;
    Locations m_loc;
    vector<unsigned> shadUnifLocations;//to replace with struct Locations
    OglRendererProgress DrawSingleModelEntry(ModelData& d, unsigned int gl_ProgramId);
    virtual OglRendererProgress DrawTextureForSingleModelEntry(const unsigned int vao, TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId);
    virtual OglRendererProgress DrawModel(spOneModel, unsigned int  );
    unsigned int StartCallCount(){return startCallCount;}
    virtual void setViewMatrices(spMatrixStack );
    virtual void setLightMatrices(myLight * );
    virtual void setLocationsFrom(spMyOGLShaders);
protected:
    unsigned int startCallCount = 0;
    void DrawIndicesAndFinish(ModelData& d);
//    spMatrixStack m_matrixStack = nullptr;
};

using spOglRenderer = std::shared_ptr<OglRenderer>;
using upOglRenderer = std::unique_ptr<OglRenderer>;
#endif // OglRenderer_H
