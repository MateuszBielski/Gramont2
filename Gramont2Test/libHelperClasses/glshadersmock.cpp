#include "glshadersmock.h"


bool glShadersMock::Compile(GLuint shaId)
{
    m_usingCompile = true;
    return true;
}
bool glShadersMock::UsingCompile()
{
    return m_usingCompile;
}
void glShadersMock::setAttribLoc(const string& name,unsigned int loc)
{
    bool isset = false;
    for (shaVars_v::iterator it = m_shaAttrib.begin(); it != m_shaAttrib.end(); ++it) {
        if ( it->name == name && it->loc != (GLuint)-1 ) {
            it->loc = loc;
            isset = true;
            break;
        }
    }
    if(!isset)
        m_shaAttrib.push_back(shaVars {loc,name});
}
void glShadersMock::setUnifLoc(const string& name, unsigned int loc)
{
    bool isset = false;
    for (shaVars_v::iterator it = m_shaUnif.begin(); it != m_shaUnif.end(); ++it) {
        if ( it->name == name && it->loc != (GLuint)-1 ) {
            it->loc = loc;
            isset = true;
            break;
        }
    }
    if(!isset)
        m_shaUnif.push_back(shaVars {loc,name});
//m_shaUnif
}
bool glShadersMock::hasCodeOfType(unsigned int codeType)
{
    bool result = false;
    for(auto& code : m_shaCode)
    {
        if(code.typeSha == codeType)
        {
            result = true;
            break;
        }
    }
    return result;
}

bool glShadersMock::hasAttrib(const string& name)
{
    bool result = false;
    for(auto& attr : m_shaAttrib)
    {
        if(attr.name == name)
        {
            result = true;
            break;
        }
    }
    return result;
}

bool glShadersMock::hasUnif(const string& name)
{
     bool result = false;
    for(auto& unif : m_shaUnif)
    {
        if(unif.name == name)
        {
            result = true;
            break;
        }
    }
    return result;
}
