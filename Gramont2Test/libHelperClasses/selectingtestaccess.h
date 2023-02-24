/* %PATH% %FILE% */
#ifndef SelectingTestAccess_H
#define SelectingTestAccess_H
#include "selecting.h"
#include "oglstuff.h"

class SelectingTestAccess
{
private:
    Selecting& accessed;
public:
    SelectingTestAccess(Selecting& );
    void SetReadyForRendering(bool);
    void setShadersLoaded(bool);
    void setShader(spMyOGLShaders);
    bool ShadersLoaded();
    int getClickedPosX();
    int getClickedPosY();
    unsigned int getWindowWidth();
    unsigned int getWindowHeight();
    bool Inited();
protected:

};

#endif // SelectingTestAccess_H
