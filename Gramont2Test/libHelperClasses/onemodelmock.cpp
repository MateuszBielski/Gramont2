#include "onemodelmock.h"

using namespace std;


OneModelMock::OneModelMock()
{
    m_texture = make_shared<TextureForModel>();
}

ModelData& OneModelMock::GetModelData()
{
    m_getModelDataUsed = true;
    return OneModel::GetModelData();
}
bool OneModelMock::GetModelDataUsed()
{
    return m_getModelDataUsed;
}
