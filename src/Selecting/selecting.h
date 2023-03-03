#ifndef Selecting_H
#define Selecting_H

#include <string>
#include <vector>
#include "oglstuff.h"
#include "pickingrenderer.h"
#include "pickingbuffloader.h"
#include "selectable.h"

using std::string, std::vector;

class SelectingResult
{
    bool m_selectingDone;
public:
    SelectingResult(bool sd = false);
    bool selectingDone();
};

class Selecting
{
#ifdef TESTOWANIE_F
    friend class SelectingTestAccess;
#endif
private:
    bool shadersLoaded = false;
    bool readyForRendering = false;
    bool needUpdateFrameBuffer = true;
    bool frameBufferUpdated = false;

    unsigned int m_fbo = 0;
    unsigned int m_pickingTexture;
    unsigned int m_depthTexture;
    unsigned int WindowWidth = 1, WindowHeight = 1;
    const char * vertCode = nullptr;
    const char * fragCode = nullptr; 

    spMyOGLShaders m_pickingShader;
    spPickingRenderer m_pickingRenderer;
    spPickingBuffLoader m_pickingBuffLoader;

    int clickedPosX = 0;
    int clickedPosY = 0;
    bool inited = false;
    vector<spSelectable> registeredForSelection;

    void LoadShaders();
    void CreateAndLoadFrameBuffer();
    void UpdateFrameBuffer();
    bool ConfigurePickingShader();
public:
    Selecting();
    ~Selecting();
    void EnableWritingToFrameBuffer();
    void DisableWritingToFrameBuffer();
    bool Init();
    void setReadPosition(int posX, int posY);
    spOglRenderer getRenderer();
    spMyOGLShaders getShader();
    spBufferLoader getBufferLoader();
    void RegisterSelectable(vector<spSelectable>&& );
//    template<typename ContenerOfPointers>
//    SelectingResult getSelectedFrom(ContenerOfPointers setOfSelectable);
//    auto selectedModel = m_picking->getSelectedFromModels(models);
    struct PixelInfo {
        float ObjectID;
        float DrawID;
        float PrimID;

        PixelInfo() {
            ObjectID = 0.0f;
            DrawID = 0.0f;
            PrimID = 0.0f;
        }
    };

    PixelInfo ReadPixel(unsigned int x, unsigned int y);


    void SetVertexShaderPath(string);
    void SetFragmentShaderPath(string);
    SelectingResult getResult();
    void setWindowSize(unsigned int, unsigned int);

protected:
};

using spSelecting = std::shared_ptr<Selecting>;
#endif // Selecting_H
