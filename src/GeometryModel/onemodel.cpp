#include "onemodel.h"

OneModel::OneModel():addedTextures(TextureForModel::TextureType::TextureTypesSize)
{
	auto num = addedTextures.size();
    auto num2 = num;
}
OneModel::~OneModel()
{
	
}
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
bool OneModel::AddTexture(spTextureForModel tex, TextureForModel::TextureType typeTex)
{
	if(!tex->texCoord || tex->nuTexCoord != data.nuPoints)return false;
    addedTextures[(unsigned int)typeTex] = tex;
    return true;
}
spTextureForModel OneModel::getTextureOfType(TextureForModel::TextureType typeTex)
{
	return addedTextures[(unsigned int)typeTex];
}
