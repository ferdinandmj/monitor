#ifndef CAMERA_H
#define CAMERA_H

#include <wx/wx.h>
#include <wx/timer.h>
#include <opencv2/opencv.hpp>
#include "MonitorObject.h"

class Camera: public MonitorObject, public wxEvtHandler
{
public:
    Camera(int id);
    void simulate(); //
    int getID() {return id_;};
    static void initMainPanel(wxWindow* parent);
    static wxPanel* getMainPanel();
    wxImage getImage();

private:
    static wxPanel *mainPanel;
    void load(); // load image from file
    void OnTimer(wxTimerEvent& event);
    int id_;
    wxImage image_;
    wxPanel *panel_;
    wxBitmap scaledBg_;

    cv::VideoCapture cap{0, cv::CAP_V4L};
    wxTimer timer_;
    wxBitmap *bitmap_;

    static constexpr unsigned int FPS{25};
    static constexpr unsigned int TIMERVALUE{1000/FPS};

    //DECLARE_EVENT_TABLE();
};

#endif
