#include "modelmanager.h"
#ifdef TESTOWANIE_F
#include "glFunctionsMock.h"
#endif

using namespace std;

ModelManager::ModelManager():myOGLManager()
{
#ifdef TESTOWANIE_F
    GlFunctionsMock functionsMock;
    functionsMock.Define();
#endif
    
    m_BufferLoader = make_unique<BufferLoader>();
    m_TexRenderer = make_unique<OglRenderer>();
    m_OglRenderer = make_unique<OglRenderer>();
    m_Camera = make_unique<myOGLCamera>();
}
//ModelManager::~ModelManager()
//{
//}

template<typename T>
void ModelManager::setLocations(shared_ptr<T> rend, vec_locations_T<T> vec, myOGLShaders& shader,  sha_FunGetStr FunGetByString)
{
    for(auto& name : vec) {
        rend->m_loc.*(get<0>(name)) = (shader.*FunGetByString)(get<1>(name));//ok
    }
}



//for linker
template void ModelManager::setLocations<OglRenderer>(shared_ptr<OglRenderer>, vec_locations_T<OglRenderer>, myOGLShaders&, sha_FunGetStr);
template void ModelManager::setLocations<BufferLoader>(shared_ptr<BufferLoader>, vec_locations_T<BufferLoader>, myOGLShaders&, sha_FunGetStr);
