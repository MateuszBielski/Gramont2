#ifndef Selecting_H
#define Selecting_H

#include <string>
#include "oglstuff.h"


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
    
    string m_vertexShaderPath;
    string m_fragmentShaderPath;
    
    char * m_vertexShader = nullptr;
    char * m_fragmentShader = nullptr;
    
    myOGLShaders * m_pickingShader = nullptr;
    upPickingRenderer m_pickingRenderer;
    void LoadShaders();
public:
    Selecting();
    ~Selecting();
   void setReadPosition(posX,posY); //ver 2
   upOglrenderer& getRenderer(){return m_pickingRenderer;}
   template<typename ContenerOfPointers>
   SelectingResult getSelectedFrom(ContenerOfPointers setOfSelectable);
//    auto selectedModel = m_picking->getSelectedFromModels(models);
    bool Init(); 
    void SetVertexShaderPath(string);
    void SetFragmentShaderPath(string);
    SelectingResult getResult();

protected:
};

#endif // Selecting_H
