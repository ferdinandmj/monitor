#ifndef SENSORINPUTFRAME_H
#define SENSORINPUTFRAME_H

#include <future>
#include <wx/wx.h>
#include <wx/valtext.h>
#include "CommChannel.h"


class SensorAnalyzer;  // forward declaration

class SensorInputFrame : public wxFrame
{
public:
    SensorInputFrame(const wxString& title, const wxPoint& pos, const wxSize& size, std::shared_ptr<CommChannel<double>> comm_channel);
    double GetTextCtrl(int index)
    {
        double value = 0;
        if (index < 0 || index >= 4)
        {
            // Throw an exception or handle error in some way
            return 0;
        }
        m_textCtrls[index]->GetValue().ToDouble(&value);

        return value;
}

    wxCriticalSection& GetSensorAnalyzerCS();
    void SetSensorAnalyzer();

private:
    wxTextCtrl *m_textCtrls[4];

    std::shared_ptr<CommChannel<double>> comm_channel_;
    SensorAnalyzer* sensor_analyzer_;
    // Protects the sensor_analyzer_ pointer
    wxCriticalSection sensor_analyzer_CS_;
    DECLARE_EVENT_TABLE();

    bool Evaluate();

    void OnEvaluate(wxCommandEvent& event);
    void OnTextChange(wxCommandEvent& event);
    void OnThreadEvent(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);


};


#endif