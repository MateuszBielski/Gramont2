#include "textureformodel.h"

using namespace std;

TextureForModel::TextureForModel(TextureForModel& rtex)
{
    //czy warto flagami zaznaczać, które jeszcze elementy mają być skopiowane?
	texCoord = rtex.texCoord;
    nuTexCoord = rtex.nuTexCoord;
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
const GLint TextureForModel::getTextureUnit()
{
	if(!m_texm_ptr) return 0;
    return m_texm_ptr->getTextureUnit();
}

