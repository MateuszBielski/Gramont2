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
template<typename T>
void CreateRandomMatrixTv(T * mat,T low,T high)
{
    int lowInt = (int)floor(low);
    int highInt = (int)ceil(high);
    T rangeInt = highInt - lowInt;
    T range = high - low;
    T ratio = range/rangeInt;
    for(short i = 0 ; i < 16 ; i++) {
        mat[i] = low + ratio * (rand() % (highInt - lowInt + 1 ));
//        cout<<"\n"<<mat[i];
    }
}
void FillMatricesWithRealData_1(double * model,double * view, double * proj, float * expect);
float round_to(float r, int d);
#endif
