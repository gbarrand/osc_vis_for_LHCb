
// For compilers that support precompilation, includes "wx/wx.h".

#include <wx/wx.h>

enum {
  Menu_File_Quit = 100
};

class MyFrame: public wxFrame {
public:
  MyFrame()
  :wxFrame(0,-1,"wxWindows menu sample",wxDefaultPosition,wxSize(400, 250)){
    // create the menubar
    wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );
    // associate the menu bar with the frame
    SetMenuBar(menuBar);

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(Menu_File_Quit,"E&xit\tAlt-X","Quit");
    menuBar->Append(fileMenu, _T("&File"));
  }
  virtual ~MyFrame(){}
protected:
  void OnQuit(wxCommandEvent& WXUNUSED(event)){
    Close(TRUE);
  }
private:
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(Menu_File_Quit,MyFrame::OnQuit)
END_EVENT_TABLE()


class MyApp: public wxApp {
public:
  bool OnInit() {
    //wxInitAllImageHandlers();
    MyFrame* mainFrame = new MyFrame();
    SetTopWindow(mainFrame);
    mainFrame->Show();
    return true;
  }
};

// Below cpp macro Contains the main.

IMPLEMENT_APP(MyApp)

