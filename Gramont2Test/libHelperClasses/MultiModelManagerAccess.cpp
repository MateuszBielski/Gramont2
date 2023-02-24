#include "MultiModelManagerAccess.h"
#include "OglRendererMock.h"

using namespace std;

MultiModelManagerAccess::MultiModelManagerAccess(MultiModelManager& m):
    manager(m)
{
    
}

MultiModelManagerAccess::~MultiModelManagerAccess()
{
}

void MultiModelManagerAccess::setTexRenderer(spOglRenderer r)
{
    manager.m_TexRenderer = r;
}


void MultiModelManagerAccess::setMatricesForTexRender()
{
    manager.m_TexRenderer->setViewMatrices(manager.m_ptrMatrixStack);
    manager.m_TexRenderer->setLightMatrices(&manager.m_Light);
}
bool MultiModelManagerAccess::CameraDoesViewControl()
{
	return manager.doesCameraViewControl;
}
spSelecting MultiModelManagerAccess::getModelSelecting()
{
	return manager.m_selecting;
}
//do wykorzystania w innym miejscu
//template<typename T>
//unique_ptr<T> MultiModelManagerAccess::getTexRenderer()
//{
//    T *tmp = dynamic_cast<T*>(manager.m_TexRenderer.get());
//    unique_ptr<T> castedRenderer;
//    if(tmp != nullptr) {
//        manager.m_TexRenderer.release();
//        castedRenderer.reset(tmp);
//    }
//    return move(castedRenderer);
//}

//template unique_ptr<OglRendererMock> MultiModelManagerAccess::getTexRenderer<OglRendererMock>();
