#include "onetexturerendersystem.h"
#include "shadersPath.h"
#include "textfile.h"

OneTextureRenderSystem::OneTextureRenderSystem()
{

}

OneTextureRenderSystem::~OneTextureRenderSystem()
{

}
bool OneTextureRenderSystem::ConfigureShadersAndLocations()
{
	const char * vertCode = textFileRead(d_textureVertexShaderPath);
    const char * fragIlumCode = textFileRead(d_illuminationShaderPath);
    const char * fragCode = textFileRead(d_textureFragmentShaderPath);
    
    m_shader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_shader->AddCode(fragIlumCode,GL_FRAGMENT_SHADER);
    m_shader->AddCode(fragCode,GL_FRAGMENT_SHADER);

    m_shader->AddAttrib("in_sPosition");
    m_shader->AddAttrib("in_sNormal");
    m_shader->AddAttrib("in_TextPos");
    m_shader->AddUnif("mMVP");
    m_shader->AddUnif("mToViewSpace");
    m_shader->AddUnif("lightProps");
    m_shader->AddUnif("lightColour");
    m_shader->AddUnif("stringTexture");
    
    bool ok = true;
    ok &= (bool)vertCode;
    ok &= (bool)fragIlumCode;
    ok &= (bool)fragCode;
    if(!ok)return false;

    string nameOfFunction = "OneTextureRenderSystem::ConfigureShadersAndLocations";
    m_shader->Init(nameOfFunction);
    m_renderer->setLocationsFrom(m_shader);
    m_BufferLoader->setLocationsFrom(m_shader);
    return true;
}