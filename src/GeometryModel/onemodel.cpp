#include "onemodel.h"

OneModel::OneModel():addedTextures(TextureForModel::TextureType::TextureTypesSize)
{
	for(int i = 0; i < TextureForModel::TextureType::TextureTypesSize ; i++ )addedTextures[i] = std::make_shared<TextureForModelNotFound>();
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
spTextureForModel OneModel::getTextureOfType(TextureForModel::TextureType typeTex)
{
	return addedTextures[(unsigned int)typeTex];
}
bool OneModel::AddTexture(spTextureForModel tex, TextureForModel::TextureType typeTex)
{
	if(!tex->texCoord || tex->nuTexCoord != data.nuPoints)return false;
    addedTextures[(unsigned int)typeTex] = tex;
    return true;
}
bool OneModel::CopyFromMainTextureAs(TextureForModel::TextureType typeTex)
{
	
    addedTextures[(unsigned int)typeTex] = std::make_unique<TextureForModel>(*m_texture);
    return false;
}
