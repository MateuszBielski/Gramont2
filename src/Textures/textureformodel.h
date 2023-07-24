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
    ~TextureForModel() {};
    TextureForModel(TextureForModel& );
    enum TextureType {
        Image,Height,Normal,TextureTypesSize
    };
    virtual void setTextureInMemory(spTextureInMemory);
    const GLint getTextureUnit();
    const GLuint getTextureId();
//    bool CreateAndSetTextureInMemoryWithLoadedImageFile(std::string imageFile);

    const GLfloat * texCoord = nullptr;
    GLuint nuTexCoord = 0;
    GLuint bufTexCoordId = 0;
    GLuint textureVAO = 0;
protected:
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
