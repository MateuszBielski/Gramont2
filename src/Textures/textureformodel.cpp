#include "textureformodel.h"
#include <cstring>

using namespace std;

TextureForModel::TextureForModel(TextureForModel& rtex)
{
    //czy warto flagami zaznaczać, które jeszcze elementy mają być skopiowane?
	texCoord = rtex.texCoord;
    nuTexCoord = rtex.nuTexCoord;
}
TextureForModel::~TextureForModel()
{
	//if(texCoord)delete [] texCoord;//is bad. Memory of coords may belongs to Model
}
void TextureForModel::setTextureInMemory(spTextureInMemory texm)
{
	m_texm_ptr = texm;
}
const GLuint TextureForModel::getTextureId()
{
    if(!m_texm_ptr) return 0;
    return m_texm_ptr->getTextureId();
}
//const GLint TextureForModel::getTextureUnit()
//{
//	if(!m_texm_ptr) return 0;
//    return m_texm_ptr->getTextureUnit();
//}
GLint& TextureForModel::getTextureUnit()
{
    return textureUnit;
}
void TextureForModel::setCoordinates(spTextureForModel tex)
{
	nuTexCoord = tex->nuTexCoord;
    if(texCoord)delete [] texCoord;
    float * tempTexCoord = new float[nuTexCoord * 2];
    memcpy(tempTexCoord,tex->texCoord,nuTexCoord * 2 * sizeof(float));
    texCoord = tempTexCoord;
}



//[  FAILED  ] BufferLoader_Te.NotCreateTextureBuffersIfCreated -do sprawdzenia
//[  FAILED  ] BufferLoader_Te.CreateTextureBuffersIf_InitialValue_NotCreated -do sprawdzenia

