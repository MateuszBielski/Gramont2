#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

//#include "oglstuff.h"
//#include "wx/glcanvas.h"
#include "MyApp.h"
#include "myframe.h"
//#include <string>




wxIMPLEMENT_APP(MyApp);

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

//    // create the main application window
    MyFrame* frame = new MyFrame("wxWidgets OpenGL Gramont2");
//
//    //Exit if the required visual attributes or OGL context couldn't be created
//    if ( ! frame->OGLAvailable() )
//        return false;
//
//    // As of October 2015 GTK+ needs the frame to be shown before we call SetCurrent()
    frame->Show(true);

    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor

//

// event handlers

//void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
//{
//    // true is to force the frame to close
//    Close(true);
//}

//void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
//{
//    wxMessageBox(wxString::Format
//                 (
//                    "Welcome to %s!\n"
//                    "\n"
//                    "This is the wxWidgets OpenGL Pyramid sample.\n"
//                    "%s\n",
//                    wxVERSION_STRING,
//                    m_OGLString
//                 ),
//                 "About wxWidgets pyramid sample",
//                 wxOK | wxICON_INFORMATION,
//                 this);
//}

//#if wxUSE_LOGWINDOW
//void MyFrame::OnLogWindow(wxCommandEvent& WXUNUSED(event))
//{
//    if ( m_LogWin->GetFrame()->IsIconized() )
//        m_LogWin->GetFrame()->Restore();
//
//    if ( ! m_LogWin->GetFrame()->IsShown() )
//        m_LogWin->Show();
//
//    m_LogWin->GetFrame()->SetFocus();
//}
//#endif // wxUSE_LOGWINDOW

//bool MyFrame::OGLAvailable()
//{
//    //Test if visual attributes were accepted.
//    if ( ! m_mycanvas )
//        return false;
//
//    //Test if OGL context could be created.
//    return m_mycanvas->OglCtxAvailable();
//}

// ----------------------------------------------------------------------------
// Function for receiving messages from OGLstuff and passing them to the log window
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// These two functions allow us to convert a wxString into a RGBA pixels array
// ----------------------------------------------------------------------------

// Creates a 4-bytes-per-pixel, RGBA array from a wxImage.
// If the image has alpha channel, it's used. If not, pixels with 'cTrans' color
// get 'cAlpha' alpha; an the rest of pixels get alpha=255 (opaque).
//


//MyGLCanvas::~MyGLCanvas()
//{
//
//}
