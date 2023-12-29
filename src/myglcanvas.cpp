#include "oglstuff.h"
#include "myglcanvas.h"
#include "funkcje.h"


wxBEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
EVT_PAINT(MyGLCanvas::OnPaint)
EVT_SIZE(MyGLCanvas::OnSize)
EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
EVT_MOUSEWHEEL(MyGLCanvas::OnMouseWheel)
EVT_KEY_DOWN(MyGLCanvas::OnKeyDown)
wxEND_EVENT_TABLE()


MyGLCanvas::MyGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs)
    : wxGLCanvas(parent, canvasAttrs)
{
    m_parent = parent;

    m_oglManager = NULL;
    m_winHeight = 0; // We have not been sized yet

    int verMajor = 2;
    int verMinor = 1;
    UstawOpenGL(verMajor,verMinor);

}

MyGLCanvas::~MyGLCanvas()
{
    if ( m_oglContext )
        SetCurrent(*m_oglContext);

    if ( m_oglManager ) {
        delete m_oglManager;
        m_oglManager = NULL;
    }

    if ( m_oglContext ) {
        delete m_oglContext;
        m_oglContext = NULL;
    }
}

wxGLContext* MyGLCanvas::UstawOpenGL(int verMajor, int verMinor )
{
    // Explicitly create a new rendering context instance for this canvas.
    wxGLContextAttrs ctxAttrs;
#ifndef __WXMAC__
    // An impossible context, just to test IsOk()
    ctxAttrs.PlatformDefaults().OGLVersion(99, 2).EndList();
    m_oglContext = new wxGLContext(this, NULL, &ctxAttrs);

    if ( !m_oglContext->IsOK() ) {
#if wxUSE_LOGWINDOW
        wxLogMessage("Trying to set OpenGL 99.2 failed, as expected.");
#endif // wxUSE_LOGWINDOW
        delete m_oglContext;
        ctxAttrs.Reset();
#endif //__WXMAC__
//        ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 2).EndList();

        ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(verMajor, verMinor).EndList();
        m_oglContext = new wxGLContext(this, NULL, &ctxAttrs);
#ifndef __WXMAC__
    }
#endif //__WXMAC__

    if ( !m_oglContext->IsOK() ) {
        wxMessageBox("This sample needs an OpenGL 3.1 capable driver.\nThe app will end now.",
                     "OpenGL version error", wxOK | wxICON_INFORMATION, this);
        delete m_oglContext;
        m_oglContext = NULL;
    } else {
#if wxUSE_LOGWINDOW
        std::string messageOk = "OpenGL Core Profile "+std::to_string(verMajor)+"."+std::to_string(verMinor)+" successfully set.";
        wxLogMessage(messageOk.c_str() );
#endif // wxUSE_LOGWINDOW
    }
    return m_oglContext;
}

//bool MyGLCanvas::oglInit()
//{
//    if ( !m_oglContext )
//        return false;
//
//    // The current context must be set before we get OGL pointers
//    SetCurrent(*m_oglContext);
//
//
//    // Initialize our OGL pointers
////    if ( !myOGLManager::Init(m_oglContext) ) {
//    if ( !myOGLManager::Init() ) {
//        wxMessageBox("Error: Some OpenGL pointer to function failed.",
//                     "OpenGL initialization error", wxOK | wxICON_INFORMATION, this);
//        return false;
//    }
//
//    // Create our OGL manager, pass our OGL error handler
////    auto fun = &fOGLErrHandler;
//
////    m_oglManager = new OGLManager(&fOGLErrHandler);
//    m_oglManager = new MultiModelManager(&fOGLErrHandler);
//
//
//    // Get the GL version for the current OGL context
//    wxString sglVer = "\nUsing OpenGL version: ";
//    sglVer += wxString::FromUTF8(
//                  reinterpret_cast<const char *>(m_oglManager->GetGLVersion()) );
//    // Also Vendor and Renderer
//    sglVer += "\nVendor: ";
//    sglVer += wxString::FromUTF8(
//                  reinterpret_cast<const char *>(m_oglManager->GetGLVendor()) );
//    sglVer += "\nRenderer: ";
//    sglVer += wxString::FromUTF8(
//                  reinterpret_cast<const char *>(m_oglManager->GetGLRenderer()) );
//    // For the menu "About" info
//    m_parent->SetOGLString(sglVer);
//
//    // Load some data into GPU
//    m_oglManager->SetShadersAndGeometry();
//
//
//    return true;
//}

void MyGLCanvas::OnSize(wxSizeEvent& event)
{
    event.Skip();

    // If this window is not fully initialized, dismiss this event
    if ( !IsShownOnScreen() )
        return;

    if ( !m_oglManager ) {
        //Now we have a context, retrieve pointers to OGL functions
        if ( !oglInit() )
            return;
        //Some GPUs need an additional forced paint event
        PostSizeEvent();
    }

    // This is normally only necessary if there is more than one wxGLCanvas
    // or more than one wxGLContext in the application.
    SetCurrent(*m_oglContext);

    // It's up to the application code to update the OpenGL viewport settings.
    const wxSize size = event.GetSize() * GetContentScaleFactor();
    m_winHeight = size.y;
    m_oglManager->SetViewport(0, 0, size.x, m_winHeight);

    // Generate paint event without erasing the background.
    Refresh(false);
}

void MyGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    // This is a dummy, to avoid an endless succession of paint messages.
    // OnPaint handlers must always create a wxPaintDC.
    wxPaintDC dc(this);

    // Avoid painting when we have not yet a size
    if ( m_winHeight < 1 || !m_oglManager )
        return;

    // This should not be needed, while we have only one canvas
    SetCurrent(*m_oglContext);

    // Do the magic
    m_oglManager->Render();

    SwapBuffers();
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
{
    event.Skip();

    // GL 0 Y-coordinate is at bottom of the window
    int oglwinY = m_winHeight - event.GetY();

    if ( event.LeftIsDown() ) {
        if ( ! event.Dragging() ) {
            // Store positions
            m_oglManager->OnMouseButDown(event.GetX(), oglwinY);
        } else {
            // Rotation
            m_oglManager->OnMouseRotDragging( event.GetX(), oglwinY );

            // Generate paint event without erasing the background.
            Refresh(false);
        }
    }
    if( event.LeftDClick()) {
        m_oglManager->OnMouseLeftDClick(event.GetX(),oglwinY);
    }

    if(event.MiddleIsDown()) {
        if ( ! event.Dragging() ) {
            m_oglManager->OnMouseButDown(event.GetX(), oglwinY);
        } else {
            m_oglManager->OnMouseMiddleClick(event.GetX(),oglwinY);
            Refresh(false);
        }

    }
}
void MyGLCanvas::OnMouseWheel(wxMouseEvent &event )
{
    m_oglManager->OnMouseWheel(event.GetWheelRotation());
//    event.Skip();//?
    Refresh(false);
}
void MyGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode() == 83) { //"s"
        m_oglManager->SwitchViewControl();
    }
}
