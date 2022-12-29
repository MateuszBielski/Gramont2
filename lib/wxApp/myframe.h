#ifndef MyFrame_H
#define MyFrame_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//using namespace std;
//using namespace Gtk;

class MyGLCanvas;

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

//    void OnAbout(wxCommandEvent& event);
//    void OnQuit(wxCommandEvent& event);
#if wxUSE_LOGWINDOW
    void OnLogWindow(wxCommandEvent& event);
#endif // wxUSE_LOGWINDOW
    void SetOGLString(const wxString& ogls) {
        m_OGLString = ogls;
    }
    bool OGLAvailable();

private:
#if wxUSE_LOGWINDOW
    wxLogWindow* m_LogWin;
#endif // wxUSE_LOGWINDOW
    wxString     m_OGLString;
    MyGLCanvas*  m_mycanvas;

    wxDECLARE_EVENT_TABLE();
};

#endif // MyFrame_H
