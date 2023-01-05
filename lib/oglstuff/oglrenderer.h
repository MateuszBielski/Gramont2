/* %PATH% %FILE% */
#ifndef OglRenderer_H
#define OglRenderer_H
#include <memory>
#include "modeldata.h"
#include "textureformodel.h"
#include "bufferloader.h"

using namespace std;

enum class OglRendererProgress
{
    Completed,
    BeforeOgl,
};

class OglRenderer
{
private:
    unsigned int startCallCount = 0;
public:
    struct Matrices {
        const float * matMVP = nullptr;
        const float * matToVw = nullptr;
        const float * light_position = nullptr;
        const float * light_colour = nullptr;
    };
    struct Locations {
        int mMVP = 0;
        int mToViewSpace = 0;
        int lightProps = 0;
        int lightColour = 0;
        int stringTexture = 0;
    };
    Matrices m_matrices;
    Locations m_loc;
    OglRendererProgress DrawSingleModelEntry(ModelData& d, unsigned int gl_ProgramId);
    virtual OglRendererProgress DrawTextureForSingleModelEntry(TextureForModel& tex, ModelData& d, unsigned int gl_ProgramId);
    unsigned int StartCallCount(){return startCallCount;}
protected:

};

using spOglRenderer = shared_ptr<OglRenderer>;
using upOglRenderer = unique_ptr<OglRenderer>;
#endif // OglRenderer_H