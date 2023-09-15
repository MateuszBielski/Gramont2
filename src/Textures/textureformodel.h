#ifndef TextureForModel_H
#define TextureForModel_H
#include <string>
#include <memory>
#include <glad/glad.h>
#include "textureinmemory.h"

class TextureForModel
{
public:
    TextureForModel() {};
    ~TextureForModel();
    TextureForModel(TextureForModel& );
    enum TextureType {
        None,Diffuse,Height,Normal,TextureTypesSize//None = 0
    };
    virtual void setTextureInMemory(spTextureInMemory);
    void setCoordinates(std::shared_ptr<TextureForModel> );
//    const GLint getTextureUnit();
    GLint& getTextureUnit();
    const GLuint getTextureId();
//    bool CreateAndSetTextureInMemoryWithLoadedImageFile(std::string imageFile);

    const GLfloat * texCoord = nullptr;
    GLuint nuTexCoord = 0;
    GLuint bufTexCoordId = 0;
    GLuint textureVAO = 0;
protected:
    GLint textureUnit = TextureType::Diffuse;
    spTextureInMemory m_texm_ptr;
};

using spTextureForModel = std::shared_ptr<TextureForModel>;

class TextureForModelNotFound : public TextureForModel
{
public:
    TextureForModelNotFound() {
        bufTexCoordId = (unsigned)-1;
    }
    virtual void setTextureInMemory(spTextureInMemory) override {}
};
#endif // TextureForModel_H
