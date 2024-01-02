#ifndef RenderSystemManager_H
#define RenderSystemManager_H

#include <iostream>
#include <memory>
#include <rendersystem.h>

using std::vector;

class RenderSystemManager : public RenderSystem
{
private:

public:
    RenderSystemManager() {}
    virtual ~RenderSystemManager() {}
    
    template<typename RsType>
    unsigned AddRenderSystem();
    void Draw(spOneModel ) override;
    void EnableExternalRenderSystem(spRenderSystem);
    void DisableExternalRenderSystem();
    void ConnectModelWithRenderSystem(unsigned uniqueId,unsigned rsId);
    void CurrentModelId(unsigned id){currentModelId = id;}
    vector<spRenderSystem>& getAllRenderSystems(){return m_CollectionOfRendSystems;}
    spOglRenderer ActiveRenderer();
    spMyOGLShaders ActiveShader();
    
    virtual bool ConfigureShadersAndLocations();
    virtual void LoadVAO(spOneModel );
    virtual void CreateGraphicBuffers(spOneModel );
    virtual void CheckModelWasConnected(spOneModel ) override {/*TO DO*/}
    
    spOglRenderer getRenderer() = delete;
protected:
    vector<spRenderSystem> m_CollectionOfRendSystems;
    vector<unsigned> whichSystemForModel;
    spRenderSystem externalRs;
//    spOglRenderer activeRenderer;
//    spMyOGLShaders activeShader;
    bool externalRenderSystemEnabled = false;
    unsigned currentModelId = (unsigned)-1;
};
using spRenderSystemManager = std::shared_ptr<RenderSystemManager>;
using upRenderSystemManager = std::unique_ptr<RenderSystemManager>;
#endif // RenderSystemManager_H
