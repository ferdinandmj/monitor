#include <memory>
#include <vector>
#include <algorithm>
#include <future>
#include <wx/wxprec.h>
#include <wx/valtext.h>
#include "Camera.h"
#include "CameraPanel.h"
#include "SensorAnalyzer.h"
#include "SensorInputFrame.h"
#include "CommChannel.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// TODO camera can be accessed by another object other than the panel
void createMonitorObjects(std::vector<std::shared_ptr<Camera>> &cameras)
{
    for (size_t nc = 1; nc < 5; nc++)
    {
        cameras.push_back(std::make_shared<Camera>(int(nc)));
    }
}

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

private:
    //std::shared_ptr<wxPanel> panels;
};

enum
{
    ID_Hello = 1
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Camera", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show(true);

    //CommChannel<double> comm_channel;
    std::shared_ptr<CommChannel<double>> comm_channel(new CommChannel<double>);

    SensorInputFrame *sensorInputFrame = new SensorInputFrame("Sensors", wxPoint(50, 50), wxSize(450, 340), comm_channel);
    sensorInputFrame->Show(true);

    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(wxTheApp->GetTopWindow(), wxID_ANY, title, pos, size)
{

    ::wxInitAllImageHandlers();

    std::vector<std::shared_ptr<Camera>> cameras;
    createMonitorObjects(cameras);

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    //Camera::initMainPanel(this);

    // Create a FlexGridSizer with 2 rows and 2 columns.
    wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 0, 0);
    sizer->AddGrowableRow(0, 1);
    sizer->AddGrowableRow(1, 1);
    sizer->AddGrowableCol(0, 1);
    sizer->AddGrowableCol(1, 1);

    std::for_each(cameras.begin(), cameras.end(), [](std::shared_ptr<Camera> &i) {
        i->simulate();
    });


    // Create panels for each part of the frame.
    //wxPanel *topLeftPanel = new wxPanel(panel, wxID_ANY);
    CameraPanel *topLeftPanel = new CameraPanel(panel, wxID_ANY, cameras[0]);

    //topLeftPanel->SetBackgroundColour(*wxRED);
    wxPanel *topRightPanel =  new CameraPanel(panel, wxID_ANY, cameras[1]); //new wxPanel(panel, wxID_ANY);
    topRightPanel->SetBackgroundColour(*wxGREEN);
    wxPanel *bottomLeftPanel = new wxPanel(panel, wxID_ANY);
    bottomLeftPanel->SetBackgroundColour(*wxBLUE);
    wxPanel *bottomRightPanel = new wxPanel(panel, wxID_ANY);
    bottomRightPanel->SetBackgroundColour(*wxYELLOW);

    // Add the panels to the sizer.
    sizer->Add(topLeftPanel, 1, wxEXPAND);
    sizer->Add(topRightPanel, 1, wxEXPAND);
    sizer->Add(bottomLeftPanel, 1, wxEXPAND);
    sizer->Add(bottomRightPanel, 1, wxEXPAND);

    panel->SetSizer(sizer);

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    // Delete the camera threads
    Destroy();
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
