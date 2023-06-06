#ifndef Selecting_H
#define Selecting_H

#include <string>
#include <vector>
#include "oglstuff.h"
#include "pickingrenderer.h"
#include "pickingbuffloader.h"
#include "selectable.h"
#include "rendersystem.h"

using std::string, std::vector;

class SelectingResult
{
    bool m_selectingDone;
    spSelectable selected;
public:
    SelectingResult();
    SelectingResult(spSelectable sel);
    bool selectingDone();
    spSelectable getSelected();
};

class Selecting : public RenderSystem
{
#ifdef TESTOWANIE_F
    friend class SelectingTestAccess;
#endif
private:
    bool shadersLoaded = false;
//    bool readyForRendering = false;
    bool needUpdateFrameBuffer = true;
    bool frameBufferUpdated = false;

    int selectedModelId = -1;
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

    void CreateAndLoadFrameBuffer();
    void UpdateFrameBuffer();
    bool ConfigurePickingShader();
public:
    Selecting();
    ~Selecting();
    struct PixelInfo {
        float notUsed_1;
        float ObjectID;
        float notUsed_2;

        PixelInfo() {
            notUsed_1 = 0.0f;
            ObjectID = -2.0f;
            notUsed_2 = 0.0f;
        }
    };
    virtual void LoadVAO(spOneModel ) override;
    virtual bool ConfigureShadersAndLocations() override;
    void RegisterSelectable(vector<spSelectable>&& );
    void setWindowSize(unsigned int, unsigned int);
    void setReadPosition(int posX, int posY);
    void UpdateSelectedModelId(PixelInfo& pxi);
    void EnableWritingToFrameBuffer();
    void DisableWritingToFrameBuffer();
    void ReadInClickedPosition();
    SelectingResult getResult();


    void SetVertexShaderPath(string);
    void SetFragmentShaderPath(string);
protected:
    PixelInfo ReadPixel(unsigned int x, unsigned int y);
};

using spSelecting = std::shared_ptr<Selecting>;
#endif // Selecting_H
