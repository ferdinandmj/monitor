#include "SensorInputFrame.h"
#include "SensorAnalyzer.h"

#include "events.h"

wxBEGIN_EVENT_TABLE(SensorInputFrame, wxFrame)
    EVT_COMMAND  (NUMBER_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, SensorInputFrame::OnThreadEvent)
    EVT_CLOSE(SensorInputFrame::OnClose)
wxEND_EVENT_TABLE()

SensorInputFrame::SensorInputFrame(const wxString& title, const wxPoint& pos, const wxSize& size, std::shared_ptr<CommChannel<double>> comm_channel)
    : wxFrame(NULL, wxID_ANY, title, pos, size), comm_channel_(comm_channel)
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxTextValidator validator(wxFILTER_DIGITS);

    // Start the worker thread for input changes
    sensor_analyzer_ = new SensorAnalyzer(this, comm_channel_);
    if (sensor_analyzer_->Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't create the thread!");
        delete sensor_analyzer_;
        sensor_analyzer_ = NULL;
    }


    for(int i = 0; i < 4; i++) {
        m_textCtrls[i] = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, validator);
        sizer->Add(m_textCtrls[i], 1, wxEXPAND);
        // Bind the event handler to EVT_TEXT event
        m_textCtrls[i]->Bind(wxEVT_TEXT_ENTER, &SensorInputFrame::OnTextChange, this);


    }


    wxButton *button = new wxButton(panel, wxID_ANY, "Evaluate");
    button->Bind(wxEVT_BUTTON, &SensorInputFrame::OnEvaluate, this);
    sizer->Add(button, 0, wxALL, 5);  // Add button to sizer with border

    panel->SetSizer(sizer);

}

bool SensorInputFrame::Evaluate()
{
    for (int i = 0; i < 4; i++)
    {
        double value = 0;
        if(m_textCtrls[i]->GetValue().ToDouble(&value) && value > 10)
        {
            return true;
        }
    }
    return false;
}

void SensorInputFrame::OnEvaluate(wxCommandEvent& event)
{
    std::future<bool> eval_ftr = std::async([&]() {
        return this->Evaluate();
    });

    eval_ftr.wait();
    bool sensor_evaluation = eval_ftr.get();
    if (sensor_evaluation == true) {
        wxMessageDialog dialog(this, "Out of range", "Threshold", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
    }
}

void SensorInputFrame::OnTextChange(wxCommandEvent& event)
{

    // Retrieve the textCtrl that triggered the event
    wxTextCtrl* changedTextCtrl = static_cast<wxTextCtrl*>(event.GetEventObject());

    // Retrieve the updated value from the changedTextCtrl
    wxString value_str = changedTextCtrl->GetValue();

    double value = 1;
    value_str.ToDouble(&value);

    // Send the value to the communication channel
    comm_channel_->send(std::move(value));

}


void SensorInputFrame::OnThreadEvent(wxCommandEvent& event)
{
/*    wxCriticalSectionLocker enter(sensor_analyzer_CS_);
    if (!comm_channel_)*/
    comm_channel_->close();
    int test = event.GetInt();
    int i = 7;
    wxString Foobar;
    Foobar.Printf( wxT("Hello I have %d cookies."), test);
    wxMessageBox(Foobar);
}


wxCriticalSection& SensorInputFrame::GetSensorAnalyzerCS()
{
    return sensor_analyzer_CS_;
}

void SensorInputFrame::SetSensorAnalyzer()
{
    if (sensor_analyzer_) {
        sensor_analyzer_ = NULL;
        wxMessageOutputDebug().Printf("deleting the pointer");
    }

}

void SensorInputFrame::OnClose(wxCloseEvent& event)
{
    {
        wxCriticalSectionLocker enter(sensor_analyzer_CS_);
        comm_channel_->close();
        if (sensor_analyzer_)         // does the thread still exist?
        {
            wxMessageOutputDebug().Printf("MYFRAME: deleting thread");
            if (sensor_analyzer_->Delete() != wxTHREAD_NO_ERROR )
                wxLogError("Can't delete the thread!");
            else {
                SetSensorAnalyzer();
                wxMessageOutputDebug().Printf("MYFRAME: deleted thread");
            }
        }
    }       // exit from the critical section to give the thread
            // the possibility to enter its destructor
            // (which is guarded with m_pThreadCS critical section!)
    while (1)
    {
        { // was the ~MyThread() function executed?
            wxCriticalSectionLocker enter(sensor_analyzer_CS_);
            wxMessageOutputDebug().Printf("MYFRAME: while 1");
            if (!sensor_analyzer_) break;
        }
        // wait for thread completion
        wxThread::This()->Sleep(1);
    }


    Destroy();
}
