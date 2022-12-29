#include "textureformodel.h"
#define STB_IMAGE_IMPLEMENTATION
//#define STBI_NO_FAILURE_STRINGS
//#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "iostream"

using namespace std;

void TextureForModel::LoadImageFile(string imageFile)
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

unsigned char* TextureForModel::TextureData()
{
//    unsigned char * data = new unsigned char [100];
    return textureData;
}
TextureForModel::~TextureForModel()
{
    stbi_image_free(textureData);
}
bool TextureForModel::hasImagePath()
{
    return (bool)imagePath.size();
}
