#include <gtest/gtest.h>
#include "textureformodel.h"
#include "triangle.h"
//#include "../src/Surface/surface.h"
TEST(Texture,ReturnNullForNotExistingFile)
{
    TextureInMemory tex1("NieIstniejacyPlik.jpg");
    ASSERT_EQ(nullptr,tex1.TextureData());
}
TEST(Texture,NotNullDataPointer)
{
    TextureInMemory tex1("Gramont2Test/Resources/10x10image.jpg");
    ASSERT_NE(nullptr,tex1.TextureData());
}
TEST(Texture,ImageSize)
{
    TextureInMemory tex1("Gramont2Test/Resources/10x10image.jpg");
    ASSERT_EQ(10,tex1.width);
    ASSERT_EQ(10,tex1.height);
    ASSERT_EQ(3,tex1.nuChannels);
}
TEST(Texture,NotKnowImagePath)
{
    TextureInMemory tex("");
    ASSERT_FALSE(tex.hasImagePath());
}
TEST(Texture,KnowImagePath)
{
    TextureInMemory tex("NieIstniejacyPlik.jpg");
    ASSERT_TRUE(tex.hasImagePath());
}
TEST(Triangle,nuTextureCoordinates)
{
    spOneModel triang = make_shared<Triangle>();
    ASSERT_EQ(3,triang->MyTexture()->nuTexCoord);
}
TEST(Triangle,TextureCoordinates)
{
    spOneModel triang = make_shared<Triangle>();

    GLfloat expected[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };
    const GLfloat * texCoord = triang->MyTexture()->texCoord;
    ASSERT_NE(nullptr,texCoord);
    for(short i = 0 ; i < 6 ; i++) {
        ASSERT_EQ(expected[i],texCoord[i]);
    }
}
