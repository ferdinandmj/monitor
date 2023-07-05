#include <chrono>
#include <thread>
#include "SensorAnalyzer.h"

wxThread::ExitCode SensorAnalyzer::Entry()
{
    while ( !TestDestroy() ) {
        std::optional<double> value = comm_channel_->receive();
        
        if (value) {
            double changed_value = 0;
            if (value.has_value())
            {
                changed_value = value.value() * 10;
            }

            // Notify the main thread/frame
            wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, NUMBER_UPDATE_ID);
            event.SetInt(int(changed_value));
            parent_->GetEventHandler()->AddPendingEvent(event);

        }
        else
        {
            return (wxThread::ExitCode)0;
        }

    }
    return (wxThread::ExitCode)0;

}

SensorAnalyzer::~SensorAnalyzer()
{
    wxCriticalSectionLocker enter(parent_->GetSensorAnalyzerCS());
    wxMessageOutputDebug().Printf("Destructor");
    // The thread is being destroyed; make sure not to leave dangling pointers around
    parent_->SetSensorAnalyzer();
}
