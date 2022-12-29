#ifndef Selecting_H
#define Selecting_H

#include <string>
#include "oglstuff.h"

using namespace std;

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
    void LoadShaders();
public:
    Selecting();
    ~Selecting();
    bool Init(); 
    void SetVertexShaderPath(string);
    void SetFragmentShaderPath(string);
    SelectingResult getResult();

protected:
};

#endif // Selecting_H
