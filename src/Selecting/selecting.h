#ifndef Selecting_H
#define Selecting_H

#include <string>
#include "oglstuff.h"
#include "pickingrenderer.h"

using std::string;

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

//    string m_vertexShaderPath;
//    string m_fragmentShaderPath;

//    char * m_vertexShader = nullptr;
//    char * m_fragmentShader = nullptr;

//     * m_vertexShader = nullptr;
    spMyOGLShaders ptr_PickingShader;

    spPickingRenderer m_pickingRenderer;
    int clickedPosX = 0;
    int clickedPosY = 0;
    
    void LoadShaders();
public:
    Selecting();
    ~Selecting();
    void setReadPosition(int posX, int posY);
    spOglRenderer getRenderer();
    template<typename ContenerOfPointers>
    SelectingResult getSelectedFrom(ContenerOfPointers setOfSelectable);
//    auto selectedModel = m_picking->getSelectedFromModels(models);
    bool Init();
    void SetVertexShaderPath(string);
    void SetFragmentShaderPath(string);
    SelectingResult getResult();

protected:
};

using spSelecting = std::shared_ptr<Selecting>;
#endif // Selecting_H
