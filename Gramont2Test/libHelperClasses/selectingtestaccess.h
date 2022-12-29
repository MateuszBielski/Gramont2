/* %PATH% %FILE% */
#ifndef SelectingTestAccess_H
#define SelectingTestAccess_H
#include "selecting.h"
#include "oglstuff.h"

using namespace std;

class SelectingTestAccess
{
private:
    Selecting& accessed;
public:
    SelectingTestAccess(Selecting& );
    void SetReadyForRendering(bool);
    void setShadersLoaded(bool);
    void setShadersMock(myOGLShaders * shad_mock);
    bool ShadersLoaded();
protected:

};

#endif // SelectingTestAccess_H
