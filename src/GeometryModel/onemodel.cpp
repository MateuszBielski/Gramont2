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
bool OneModel::AddTexture(spTextureForModel tex, GLuint nuTexCoord, GLfloat* texCoord, TextureForModel::TextureType typeTex)
{
	if(!nuTexCoord || !texCoord)return false;
    tex->texCoord = texCoord;
    tex->nuTexCoord = nuTexCoord;
    addedTextures[(unsigned int)typeTex] = tex;
    return true;
}
