#ifndef FUNKCJE_H
#define FUNKCJE_H
#include <glad/glad.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

void fOGLErrHandler(int err, int glerr, const GLchar* glMsg);
unsigned char* MyTextToPixels(const wxString& sText,     // The string
                              const wxFont& sFont,       // Font to use
                              const wxColour& sForeColo, // Foreground colour
                              const wxColour& sBackColo, // Background colour
                              unsigned char cAlpha,      // Background transparency
                              int* width, int* height);
unsigned char* MyImgToArray(const wxImage& img, const wxColour& cTrans, unsigned char cAlpha);
#endif
