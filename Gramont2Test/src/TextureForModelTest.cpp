#include <gtest/gtest.h>
#include "textureformodel.h"
#include "triangle.h"
#include "surface.h"
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
TEST(Texture,setCoordinatesFromOtherTexture)
{
    TextureForModel tex1;
    Surface surf(3,5,50,32);
    spTextureForModel spTex2 = surf.MyTexture();

    tex1.setCoordinates(spTex2);
    ASSERT_EQ(24,tex1.nuTexCoord);
//    for(int i = 0; i < tex1.nuTexCoord; i++)
//    {
//        cout<<"\n "<<i<<" "<<spTex2->texCoord[i * 2]<<", "<<spTex2->texCoord[i * 2 + 1];
//    }
//    15 1, 0.6
    ASSERT_FLOAT_EQ(1,tex1.texCoord[15 * 2]);
    ASSERT_FLOAT_EQ(0.6,tex1.texCoord[15 * 2 + 1]);
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
