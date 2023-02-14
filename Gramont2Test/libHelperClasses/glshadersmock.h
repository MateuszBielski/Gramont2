/* %PATH% %FILE% */
#ifndef glShadersMock_H
#define glShadersMock_H
#include "oglstuff.h"
#include <memory>

using std::string;

class glShadersMock : public myOGLShaders
{
private:
    bool m_usingCompile = false;
    bool initUsed = false;
public:
    bool UsingCompile();// used in SelectingTest
    bool InitUsed(){ return initUsed;}
//    int "in_sPosition"
    void setAttribLoc(const string&,unsigned int);
    void setUnifLoc(const string&,unsigned int);
    bool hasCodeOfType(unsigned int);
    bool hasAttrib(const string&);
    bool hasUnif(const string&);
    shaVars_v& getShaAttrib(){ return m_shaAttrib;}
    shaVars_v& getShaUnif(){ return  m_shaUnif;}
    shaShas_v& getShaCode(){return m_shaCode;}
    virtual void Init() override {initUsed = true;}
protected:
    virtual bool Compile(GLuint shaId) override ;

};
using spShadersMock = std::shared_ptr<glShadersMock>;
#endif // glShadersMock_H
