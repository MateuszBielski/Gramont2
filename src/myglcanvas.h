#ifndef MyGLCanvas_H
#define MyGLCanvas_H

#include "wx/glcanvas.h"
#include "myframe.h"
#include "myoglmanager.h"

//class myOGLManager;

class MyGLCanvas : public wxGLCanvas
{
public:
    MyGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs);
    ~MyGLCanvas();
//
//    //Used just to know if we must end now because OGL 3.2 isn't available
    bool OglCtxAvailable() {
        return m_oglContext != NULL;
    }
    wxGLContext* UstawOpenGL(int verMajor, int verMinor);

    //Init the OpenGL stuff
    bool oglInit();
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);

private:
    // Members
    MyFrame*      m_parent;
    wxGLContext*  m_oglContext;
    myOGLManager* m_oglManager;
    int           m_winHeight; // We use this var to know if we have been sized

    wxDECLARE_EVENT_TABLE();

};
#endif // MyGLCanvas_H
