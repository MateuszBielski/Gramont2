#ifndef TextureInMemory_H
#define TextureInMemory_H
#include <string>
#include <memory>
#include <glad/glad.h>

class TextureInMemory
{
private:
    unsigned char * textureData = nullptr;
    std::string imagePath;
    GLint textureUnit = 1;
    GLuint textureId = 0;

public:
    TextureInMemory(std::string imageFile);
    ~TextureInMemory();
    void LoadImageFile(std::string imageFile);
    unsigned char * TextureData();
    bool hasImagePath();
    GLint& getTextureUnit();
    GLuint& getTextureId();

    int width = 0,height = 0,nuChannels = 1;
};

using spTextureInMemory = std::shared_ptr<TextureInMemory>;

#endif // TextureInMemory_H
