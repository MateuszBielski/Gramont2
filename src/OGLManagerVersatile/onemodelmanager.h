#ifndef ManagerPyramidOnlyGeom_H
#define ManagerPyramidOnlyGeom_H
#include "modelmanager.h"
#include "onemodel.h"

class OneModelManager : public ModelManager
{
//    using spOneModelManager
private:
    myOGLShaders   m_ModelShader, m_TextureShader;
    spOneModel model = nullptr;

    const GLchar* modelVertexShader;
    const GLchar* illuminationShader;
    const GLchar* modelFragmentShader;
    const GLchar* textureVertexShader;
    const GLchar* textureFragmentShader;

    void InitModelShader();
    void InitTextureShader();

    static bool isStaticManagerInitialized;
    static std::unique_ptr<OneModelManager> ptr_OneModelManagerStatic;
#ifdef TESTOWANIE_F
    static OneModelManager m_OMM_static_inst;
#endif
    static unsigned short nuConstructors;
    static unsigned short nuDestructors;
    static unsigned short nuInstantions;
    static unsigned short maxNuInstantions;

public:
    OneModelManager(myOGLErrHandler* extErrHnd = NULL);
    ~OneModelManager();
    virtual void SetShadersAndGeometry() override;
    virtual void Draw3d() override;
    virtual void Draw2d() override;
    virtual void ZapiszShaderyDoPlikow() override;
    virtual void OdczytajShaderyZplikow() override;
    virtual void OnMouseLeftDClick(int posX, int posY) override;

#ifdef TESTOWANIE_F
    static OneModelManager& CreateAndInitStatic();
#endif
    void SetModel(spOneModel );
    void DrawTexture();

};
#endif // ManagerPyramidOnlyGeom_H
