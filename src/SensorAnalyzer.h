#ifndef SENSORANALYZER_H
#define SENSORANALYZER_H


#include "wx/wx.h"
#include <atomic>
#include "SensorInputFrame.h"
#include "events.h"
#include "CommChannel.h"

// The ID we'll use to identify our event
const int NUMBER_UPDATE_ID = 100000;

// A class to do time consuming sensor values analysis in the background
class SensorAnalyzer : public wxThread
{

public:
    SensorAnalyzer(SensorInputFrame *parent, std::shared_ptr<CommChannel<double>> comm_channel):
        wxThread(wxTHREAD_DETACHED), parent_(parent), comm_channel_(comm_channel), stop_requested_(false)
    {

    }

    ~SensorAnalyzer();

private:
    std::shared_ptr<CommChannel<double>> comm_channel_;
    std::atomic<bool> stop_requested_;
    SensorInputFrame *parent_;
    virtual ExitCode Entry();

};


#endif