#include "MultiModelManagerAccess.h"
#include "OglRendererMock.h"

MultiModelManagerAccess::MultiModelManagerAccess(MultiModelManager& m):
    manager(m)
{
}

MultiModelManagerAccess::~MultiModelManagerAccess()
{
}

void MultiModelManagerAccess::setTexRenderer(upOglRenderer r)
{
    manager.m_TexRenderer = move(r);
}

template<typename T>
unique_ptr<T> MultiModelManagerAccess::getTexRenderer()
{
    T *tmp = dynamic_cast<T*>(manager.m_TexRenderer.get());
    unique_ptr<T> castedRenderer;
    if(tmp != nullptr) {
        manager.m_TexRenderer.release();
        castedRenderer.reset(tmp);
    }
    return move(castedRenderer);
}
void MultiModelManagerAccess::setMatricesForTexRender()
{
	manager.setMatricesForRender(manager.m_TexRenderer);
}
template unique_ptr<OglRendererMock> MultiModelManagerAccess::getTexRenderer<OglRendererMock>();
