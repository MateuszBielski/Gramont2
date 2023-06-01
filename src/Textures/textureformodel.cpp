#include "textureformodel.h"
#define STB_IMAGE_IMPLEMENTATION
//#define STBI_NO_FAILURE_STRINGS
//#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "iostream"

using namespace std;

TextureInMemory::TextureInMemory(std::string imageFile)
{
	LoadImageFile(imageFile);
}
void TextureInMemory::LoadImageFile(string imageFile)
{
//    int x,y,n;
    stbi_set_flip_vertically_on_load(true);
    textureData = stbi_load(imageFile.c_str(),&width,&height,&nuChannels,0);
    imagePath = imageFile;
//    int nu = width * height * nuChannels;
//    unsigned char tempData[nu];
//    memcpy(tempData,textureData,nu*sizeof(unsigned char));
//    if(!textureData)
//    {
//        cout<<stbi_failure_reason();
//    }
}

unsigned char* TextureInMemory::TextureData()
{
//    unsigned char * data = new unsigned char [100];
    return textureData;
}
TextureInMemory::~TextureInMemory()
{
    stbi_image_free(textureData);
}
bool TextureInMemory::hasImagePath()
{
    return (bool)imagePath.size();
}
GLint& TextureInMemory::getTextureUnit()
{
    return textureUnit;
}
GLuint& TextureInMemory::getTextureId()
{	
	return textureId;
}
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

