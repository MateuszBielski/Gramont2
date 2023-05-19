#ifndef TextureForModel_H
#define TextureForModel_H
#include <string>
#include <memory>
#include <glad/glad.h>

class TextureForModel
{
private:
    unsigned char * textureData = nullptr;
    std::string imagePath;
public:
    TextureForModel() {};
    ~TextureForModel();
    void LoadImageFile(std::string imageFile);
    unsigned char * TextureData();
    bool hasImagePath();
    int width = 0,height = 0,nuChannels = 1;
    const GLfloat * texCoord = nullptr;
    GLuint nuTexCoord = 0;
    GLuint bufTexCoordId = 0;
//    GLuint bufTexCoordId = (GLuint)-1;
    GLuint textureUnit = 1;
    GLuint textureId = 0;
    GLuint textureVAO = 0;
protected:

};

using spTextureForModel = std::shared_ptr<TextureForModel>;
#endif // TextureForModel_H
