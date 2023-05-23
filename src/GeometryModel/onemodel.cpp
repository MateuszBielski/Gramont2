#include "onemodel.h"


ModelData& OneModel::GetModelData()
{
    return data;
}
spTextureForModel OneModel::MyTexture()
{
    return m_texture;
}
unsigned int& OneModel::getVao()
{
    return VAO;
//    return m_texture->textureVAO;
}
unsigned int* OneModel::getVaoPtr()
{
    return &VAO;
//    return &m_texture->textureVAO;
}
