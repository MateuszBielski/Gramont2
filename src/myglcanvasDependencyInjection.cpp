#include "myglcanvas.h"
//#include "myoglmanagerpyramid.h"
//#define OGLManager myOGLManagerPyramid;
//#include "onemodelmanagerOld.h"
//#include "onemodelmanager.h"
//#define OGLManager(x) OneModelManager(x);
#include "multimodelmanager.h"
#define OGLManager(x) MultiModelManager(x);

bool MyGLCanvas::oglInit()
{
    if ( !m_oglContext )
        return false;

    // The current context must be set before we get OGL pointers
    SetCurrent(*m_oglContext);


    // Initialize our OGL pointers
//    if ( !myOGLManager::Init(m_oglContext) ) {
    if ( !myOGLManager::Init() ) {
        wxMessageBox("Error: Some OpenGL pointer to function failed.",
                     "OpenGL initialization error", wxOK | wxICON_INFORMATION, this);
        return false;
    }

    // Create our OGL manager, pass our OGL error handler
//    auto fun = &fOGLErrHandler;

//    m_oglManager = new OGLManager(&fOGLErrHandler);
    m_oglManager = new MultiModelManager(&fOGLErrHandler);


    // Get the GL version for the current OGL context
    wxString sglVer = "\nUsing OpenGL version: ";
    sglVer += wxString::FromUTF8(
                  reinterpret_cast<const char *>(m_oglManager->GetGLVersion()) );
    // Also Vendor and Renderer
    sglVer += "\nVendor: ";
    sglVer += wxString::FromUTF8(
                  reinterpret_cast<const char *>(m_oglManager->GetGLVendor()) );
    sglVer += "\nRenderer: ";
    sglVer += wxString::FromUTF8(
                  reinterpret_cast<const char *>(m_oglManager->GetGLRenderer()) );
    // For the menu "About" info
    m_parent->SetOGLString(sglVer);

    // Load some data into GPU
    m_oglManager->SetShadersAndGeometry();


    return true;
}
