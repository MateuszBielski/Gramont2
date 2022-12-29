#ifndef OneModelManager_H
#define OneModelManager_H
#include "../oglstuff.h"

using namespace std;
//using namespace Gtk;


class OneModelManager : public myOGLManager
{
private:
    myOGLShaders   m_ModelShader;
//    string vertexShaderPath, fragmentShaderPath;
    const GLchar* modelVertexShader;
    const GLchar* modelFragmentShader;
    const GLchar* illuminationShader;
    
    GLuint modelVertBufId;
    GLuint modelColorNormalBufId;
    GLuint modelIndexBufId;
    GLuint modelVAO = 0;
public:
    OneModelManager(myOGLErrHandler* extErrHnd = NULL);
    void Clear();
    virtual void SetShadersAndGeometry() override;
    virtual void Draw3d() override;
    virtual void OdczytajShaderyZplikow() override;

protected:

private:

};

#endif // OneModelManager_H
