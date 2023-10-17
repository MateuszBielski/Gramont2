#ifndef RenderSystemManager_H
#define RenderSystemManager_H

#include <iostream>
#include <memory>
#include <rendersystem.h>

using std::vector;

class RenderSystemManager
{
private:

public:
    template<typename RsType>
    unsigned AddRenderSystem();
    void EnableExternalRenderSystem(spRenderSystem);
    void DisableExternalRenderSystem();
    void ConnectModelWithRenderSystem(unsigned uniqueId,unsigned rsId);
    void CurrentModelId(unsigned id){currentModelId = id;}
    void CheckModelWasConnected(unsigned id){/*TO DO*/}
    vector<spRenderSystem>& getAllRenderSystems(){return m_rss;}
    spOglRenderer ActiveRenderer();
    spMyOGLShaders ActiveShader();
protected:
    vector<spRenderSystem> m_rss;
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
