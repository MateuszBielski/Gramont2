#include "funkcje.h"
#include "oglstuff.h"

void fOGLErrHandler(int err, int glerr, const GLchar* glMsg)
{
#if wxUSE_LOGWINDOW
    wxString msg;

    switch (err) {
    case myoglERR_SHADERCREATE:
        msg = _("Error in shader creation.");
        break;
    case myoglERR_SHADERCOMPILE:
        msg = _("Error in shader compilation.");
        break;
    case myoglERR_SHADERLINK:
        msg = _("Error in shader linkage.");
        break;
    case myoglERR_SHADERLOCATION:
        msg = _("Error: Can't get uniforms locations.");
        break;
    case myoglERR_BUFFER:
        msg = _("Error: Can't load buffer. Likely out of GPU memory.");
        break;
    case myoglERR_TEXTIMAGE:
        msg = _("Error: Can't load texture. Likely out of GPU memory.");
        break;
    case myoglERR_DRAWING_TRI:
        msg = _("Error: Can't draw the triangles.");
        break;
    case myoglERR_DRAWING_STR:
        msg = _("Error: Can't draw the string.");
        break;
    case myoglERR_JUSTLOG:
        msg = _("Log info: ");
        break;
    default:
        msg = _("Not a GL message.");
    }

    if ( glerr != GL_NO_ERROR )
        msg += wxString::Format(_(" GL error %d. "), glerr);
    else if ( err == 0 )
        msg = _("Information: ");
    else if ( err != myoglERR_JUSTLOG )
        msg += _(" GL reports: ");

    if ( glMsg != NULL )
        msg += wxString::FromUTF8( reinterpret_cast<const char *>(glMsg) );

    wxLogMessage(msg);
#endif // wxUSE_LOGWINDOW
}

// Creates an array of bytes that defines the pixels of the string.
// The background color has cAlpha transparency. 0=transparent, 255=opaque
//
// NOTE: The returned pointer must be deleted somewhere in the app.
unsigned char* MyTextToPixels(const wxString& sText,     // The string
                              const wxFont& sFont,       // Font to use
                              const wxColour& sForeColo, // Foreground colour
                              const wxColour& sBackColo, // Background colour
                              unsigned char cAlpha,      // Background transparency
                              int* width, int* height)   // Image sizes
{
    if ( sText.IsEmpty() )
        return NULL;

    // The dc where we temporally draw
    wxMemoryDC mdc;

    mdc.SetFont(sFont);

    // Measure
    mdc.GetMultiLineTextExtent(sText, width, height);

    /* This code should be used for old graphics cards.
       But this sample uses OGL Core Profile, so the card is not that old.

    // Adjust sizes to power of two. Needed for old cards.
    int sizP2 = 4;
    while ( sizP2 < *width )
        sizP2 *= 2;
    *width = sizP2;
    sizP2 = 4;
    while ( sizP2 < *height )
        sizP2 *= 2;
    *height = sizP2;
    */

    // Now we know dimensions, let's draw into a memory dc
    wxBitmap bmp(*width, *height, 24);
    mdc.SelectObject(bmp);
    // If we have multiline string, perhaps not all of the bmp is used
    wxBrush brush(sBackColo);
    mdc.SetBackground(brush);
    mdc.Clear(); // Make sure all of bmp is cleared
    // Colours
    mdc.SetBackgroundMode(wxPENSTYLE_SOLID);
    mdc.SetTextBackground(sBackColo);
    mdc.SetTextForeground(sForeColo);
    // We draw the string and get it as an image.
    // NOTE: OpenGL axis are bottom to up. Be aware when setting the texture coords.
    mdc.DrawText(sText, 0, 0);
    mdc.SelectObject(wxNullBitmap); // bmp must be detached from wxMemoryDC

    // Bytes from the image. Background pixels become transparent with the
    // cAlpha transparency value.
    unsigned char *res = MyImgToArray(bmp.ConvertToImage(), sBackColo, cAlpha);

    return res;
}

// NOTE: The returned pointer must be deleted somewhere in the app.
unsigned char* MyImgToArray(const wxImage& img, const wxColour& cTrans, unsigned char cAlpha)
{
    int w = img.GetWidth();
    int h = img.GetHeight();
    int siz = w * h;
    unsigned char *resArr = new unsigned char [siz * 4];
    unsigned char *res = resArr;
    unsigned char *sdata = img.GetData();
    unsigned char *alpha = NULL;
    if ( img.HasAlpha() )
        alpha = img.GetAlpha();
    // Pixel by pixel
    for ( int i = 0; i < siz; i++ ) {
        //copy the colour
        res[0] = sdata[0] ;
        res[1] = sdata[1] ;
        res[2] = sdata[2] ;
        if ( alpha != NULL ) {
            //copy alpha
            res[3] = alpha[i] ;
        } else {
            // Colour cTrans gets cAlpha transparency
            if ( res[0] == cTrans.Red() && res[1] == cTrans.Green() && res[2] == cTrans.Blue() )
                res[3] = cAlpha;
            else
                res[3] = 255 ;
        }
        sdata += 3 ;
        res += 4 ;
    }

    return resArr;
}
void FillMatricesWithRealData_1(double * model,double * view, double * proj, float * expect)
{

    double matrix_model[] = {0.9467833, -0.0096355, -0.3217274, 0.0000000, 0.0422298, 0.9946301, 0.0944857, 0.0000000, 0.3190893, -0.1030440, 0.9421061, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 1.0000000 };
    double matrix_view[] = {0.9659258, 0.0801606, 0.2460926, 0.0000000, 0.0000000, 0.9508289, -0.3097168, 0.0000000, -0.2588190, 0.2991634, 0.9184301, 0.0000000, 0.0000000, 0.0000000, -395.4403082, 1.0000000 };
    double matrix_projection[] = {1.8647538, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 2.7474774, 0.0000000, 0.0000000, 0.0000000, 0.0000000, -2.8301946, -1.0000000, 0.0000000, 0.0000000, -979.4510744, 0.0000000 };
    float matrixExpect[] = {1.8606356, -0.0810948, 0.1684064, 0.0595035, 0.0304628, 2.6853154, 0.5968390, 0.2108827, 0.1200559, 0.5754452, -2.7614169, -0.9756986, 0.0000000, 0.0000000, 139.7219543, 395.4403076};
    memcpy(model,matrix_model,sizeof(double)*16);
    memcpy(view,matrix_view,sizeof(double)*16);
    memcpy(proj,matrix_projection,sizeof(double)*16);
    memcpy(expect,matrixExpect,sizeof(float)*16);
}
float round_to(float r, int d)
{
    float decim = 10;
    for(short i = 1 ; i < d ; ++i)decim *= 10;
    return round(r*decim)/decim;
}