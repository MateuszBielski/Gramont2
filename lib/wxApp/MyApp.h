#ifndef PYRSAMLE_H
#define PYRSAMLE_H

class MyApp: public wxApp
{
public:
    MyApp() {}
    bool OnInit() wxOVERRIDE;
};

/*
class MyGLCanvas;

// The main frame class



// The canvas window
class MyGLCanvas : public wxGLCanvas
{
public:
    MyGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs);
    ~MyGLCanvas();

    //Used just to know if we must end now because OGL 3.2 isn't available
    bool OglCtxAvailable()
        {return m_oglContext != NULL;}

    //Init the OpenGL stuff
    bool oglInit();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouse(wxMouseEvent& event);

private:
    // Members
    MyFrame*      m_parent;
    wxGLContext*  m_oglContext;
    myOGLManager* m_oglManager;
    int           m_winHeight; // We use this var to know if we have been sized

    wxDECLARE_EVENT_TABLE();
};


// IDs for the controls and the menu commands
enum
{
    Pyramid_Quit = wxID_EXIT,
    Pyramid_About = wxID_ABOUT,
    Pyramid_LogW = wxID_HIGHEST + 10
};
*/
#endif // PYRSAMLE_H
