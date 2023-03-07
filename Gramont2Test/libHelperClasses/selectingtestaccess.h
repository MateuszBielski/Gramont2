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
//    void SetReadyForRendering(bool);
    void setShadersLoaded(bool);
    void setShader(spMyOGLShaders);
    void setFBO(unsigned int);
    void setNeedUpdateFrameBuffer(bool);
    void setFramebufferUpdated(bool);
    void setSelectedModelId(int);
    int SelectedModelId();
    void UpdateFrameBuffer();
    bool ShadersLoaded();
    int getClickedPosX();
    int getClickedPosY();
    unsigned int getWindowWidth();
    unsigned int getWindowHeight();
    bool Inited();
    bool needUpdateFrameBuffer();
    bool FrameBufferUpdated();
protected:

};

#endif // SelectingTestAccess_H
