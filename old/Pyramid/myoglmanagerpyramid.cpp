#include <cmath>
#include "myoglmanagerpyramid.h"
#include "../src/textfile.h"
#include "../src/funkcje.h"


myOGLManagerPyramid::myOGLManagerPyramid(myOGLErrHandler* extErrHnd):myOGLManager(extErrHnd),
    gVerts { 100.0f, -40.8248f, -57.7350f,
           0.0f, -40.8248f, 115.4704f,
           -100.0f, -40.8248f, -57.7350f,
           0.0f, 122.4745f,   0.0f
},
gColours { 0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 0.0f, 0.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 0.0f, 0.3f
         },
gNormals { 0.0f,    -1.0f,      0.0f,    /* face 0 1 2 */
           -0.81650f, 0.33333f,  0.47140f, /* face 1 2 3 */
           0.0f,     0.33333f, -0.94281f, /* face 2 3 0 */
           0.81650f, 0.33333f,  0.47140f  /* face 3 0 1 */
         },
gIndices { 0, 1, 2, 3, 0, 1 }

{
}
void myOGLManagerPyramid::SetViewport(int x, int y, int width, int height)
{
    myOGLManager::SetViewport(x,y,width,height);
    // And this object handles its own orthogonal projection
    m_ImmString.SetOrtho(width, height);
}
void myOGLManagerPyramid::SetStringOnPyr(const unsigned char* strImage, int iWidth, int iHeigh)
{
    // Some geometry. We want a rectangle close to face 0-1-2 (X-Z plane).
    // The rectangle must preserve strImage proportions. If the height of the
    // rectangle is "h" and we want to locate it with its largest side parallel
    // to the edge of the face and at distance= h/2, then the rectangle width is
    // rw = edgeLength - 2 * ((h/2 + h + h/2)/tan60) = edgeLength - 4*h/sqrt(3)
    // If h/rw = Prop then
    //    rw = edgeLength / (1+4/sqrt(3)*Prop) and h = Prop * rw

    double edgeLen = MyDistance(myVec3(gVerts[0], gVerts[1], gVerts[2]),
                                myVec3(gVerts[6], gVerts[7], gVerts[8]));
    GLfloat prop = ((GLfloat) iHeigh) / ((GLfloat) iWidth);
    GLfloat rw = float(edgeLen) / (1 + 4 * prop / std::sqrt(3.0f));
    GLfloat h = prop * rw;
    GLfloat de = 2 * h / std::sqrt(3.0f);
    // A bit of separation of the face so as to avoid z-fighting
    GLfloat rY = gVerts[1] - 0.01f; // Towards outside
    GLfloat sVerts[12];
    // The image was created top to bottom, but OpenGL axis are bottom to top.
    // The image would display upside down. We avoid it choosing the right
    // order of vertices and texture coords. See myOGLString::SetStringWithVerts()
    sVerts[0] = gVerts[6] + de;
    sVerts[1] = rY;
    sVerts[2] = gVerts[8] + h / 2;
    sVerts[3] = sVerts[0]     ;
    sVerts[4] = rY;
    sVerts[5] = sVerts[2] + h;
    sVerts[6] = sVerts[0] + rw;
    sVerts[7] = rY;
    sVerts[8] = sVerts[2];
    sVerts[9] = sVerts[6]     ;
    sVerts[10] = rY;
    sVerts[11] = sVerts[5];

    // Normals for the rectangle illumination, same for the four vertices
    const GLfloat strNorms[] = { gNormals[0], gNormals[1], gNormals[2],
                                 gNormals[0], gNormals[1], gNormals[2],
                                 gNormals[0], gNormals[1], gNormals[2],
                                 gNormals[0], gNormals[1], gNormals[2]
                               };

    // The texture data for the string on the face of the pyramid
    m_StringOnPyr.SetStringWithVerts(&m_StringShaders, strImage, iWidth, iHeigh,
                                     sVerts, strNorms);
}

void myOGLManagerPyramid::SetImmutableString(const unsigned char* strImage,
        int iWidth, int iHeigh)
{
    m_ImmString.SetImmutString(&m_ImmutStringSha, strImage, iWidth, iHeigh);
}
void myOGLManagerPyramid::SetShadersAndGeometry()
{
    myOGLManager::SetShadersAndGeometry();
    // The shaders attributes and uniforms
    m_TriangShaders.AddAttrib("in_Position");
    m_TriangShaders.AddAttrib("in_Colour");
    m_TriangShaders.AddAttrib("in_Normal");
    m_TriangShaders.AddUnif("mMVP");
    m_TriangShaders.AddUnif("mToViewSpace");
    m_TriangShaders.AddUnif("lightProps");
    m_TriangShaders.AddUnif("lightColour");
    m_TriangShaders.AddCode(triangVertexShader, GL_VERTEX_SHADER);
    m_TriangShaders.AddCode(illuminationShader, GL_FRAGMENT_SHADER);
    m_TriangShaders.AddCode(triangFragmentShader, GL_FRAGMENT_SHADER);
    m_TriangShaders.Init();
    m_StringShaders.AddAttrib("in_sPosition");
    m_StringShaders.AddAttrib("in_sNormal");
    m_StringShaders.AddAttrib("in_TextPos");
    m_StringShaders.AddUnif("mMVP");
    m_StringShaders.AddUnif("mToViewSpace");
    m_StringShaders.AddUnif("lightProps");
    m_StringShaders.AddUnif("lightColour");
    m_StringShaders.AddUnif("stringTexture");
    m_StringShaders.AddCode(stringsVertexShader, GL_VERTEX_SHADER);
    m_StringShaders.AddCode(illuminationShader, GL_FRAGMENT_SHADER);
    m_StringShaders.AddCode(stringsFragmentShader, GL_FRAGMENT_SHADER);
    m_StringShaders.Init();
    m_ImmutStringSha.AddAttrib("in_sPosition");
    m_ImmutStringSha.AddAttrib("in_TextPos");
    m_ImmutStringSha.AddUnif("mMVP");
    m_ImmutStringSha.AddUnif("stringTexture");
    m_ImmutStringSha.AddCode(stringsImmutableVS, GL_VERTEX_SHADER);
    m_ImmutStringSha.AddCode(stringsImmutableFS, GL_FRAGMENT_SHADER);
    m_ImmutStringSha.Init();
    // The point light. Set its color as full white.
    // In this sample we set the light position same as the camera position
    // In View space, camera position is {0, 0, 0}
    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);
    // The triangles data
    m_Triangles.SetBuffers(&m_TriangShaders, 4, 4, gVerts, gColours, gNormals, gIndices);

    // This string will be placed on a face of the pyramid
    int swi = 0, shi = 0; //Image sizes
    wxString stg("wxWidgets");
    // Set the font. Use a big pointsize so as to smoothing edges.
    wxFont font(wxFontInfo(48).Family(wxFONTFAMILY_MODERN));
    if ( !font.IsOk() )
        font = *wxSWISS_FONT;
    wxColour bgrdColo(*wxBLACK);
    wxColour foreColo(160, 0, 200); // Dark purple
    // Build an array with the pixels. Background fully transparent
    unsigned char* sPixels = MyTextToPixels(stg, font, foreColo, bgrdColo, 0,
                                            &swi, &shi);
    // Send it to GPU
    SetStringOnPyr(sPixels, swi, shi);
    delete[] sPixels; // That memory was allocated at MyTextToPixels

    // This string is placed at left bottom of the window. Its size doesn't
    // change with window size.
    stg = "Rotate the pyramid with\nthe left mouse button";
    font.SetPointSize(14);
    bgrdColo = wxColour(40, 40, 255);
    foreColo = wxColour(*wxWHITE);
    unsigned char* stPixels = MyTextToPixels(stg, font, foreColo, bgrdColo, 80,
                              &swi, &shi);
    SetImmutableString(stPixels, swi, shi);
    delete[] stPixels;

}
void myOGLManagerPyramid::ZapiszShaderyDoPlikow()
{
    textFileWrite("triangVertexShader.c",triangVertexShader);
    textFileWrite("illuminationShader.c",illuminationShader);
    textFileWrite("triangFragmentShader.c",triangFragmentShader);
    textFileWrite("stringsVertexShader.c",stringsVertexShader);
    textFileWrite("stringsFragmentShader.c",stringsFragmentShader);
    textFileWrite("stringsImmutableVS.c",stringsImmutableVS);
    textFileWrite("stringsImmutableFS.c",stringsImmutableFS);
}
void myOGLManagerPyramid::OdczytajShaderyZplikow()
{
    //prawdopodobnie ścieżka musi być dostosowana do katalogu roboczego, czyli sciezki używanej do wywolania: np ./Debug/Gramont2
    auto dodajFolderDoSciezki = [](std::string plik) {
        std::string folder("ShaderyCoreProfile/");
        return folder+plik;
    };
    triangVertexShader = textFileRead(dodajFolderDoSciezki("triangVertexShader.c").c_str());
    illuminationShader = textFileRead(dodajFolderDoSciezki("illuminationShader.c").c_str());
    triangFragmentShader = textFileRead(dodajFolderDoSciezki("triangFragmentShader.c").c_str());
    stringsVertexShader = textFileRead(dodajFolderDoSciezki("stringsVertexShader.c").c_str());
    stringsFragmentShader = textFileRead(dodajFolderDoSciezki("stringsFragmentShader.c").c_str());
    stringsImmutableVS = textFileRead(dodajFolderDoSciezki("stringsImmutableVS.c").c_str());
    stringsImmutableFS = textFileRead(dodajFolderDoSciezki("stringsImmutableFS.c").c_str());
}

void myOGLManagerPyramid::Draw3d()
{
    m_Triangles.Draw(m_Camera.GetFloatMVP(), m_Camera.GetFloatToVw(), &m_Light);
    m_StringOnPyr.Draw(m_Camera.GetFloatMVP(), m_Camera.GetFloatToVw(), &m_Light);
}

void myOGLManagerPyramid::Draw2d()
{
    m_ImmString.Draw(m_ImmString.GetFloatMVP(), NULL, NULL);
}
