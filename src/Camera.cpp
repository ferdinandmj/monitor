#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include "Camera.h"

/*BEGIN_EVENT_TABLE(Camera, wxEvtHandler)
        EVT_TIMER(wxID_ANY, Camera::OnTimer)
END_EVENT_TABLE()*/

wxPanel* Camera::mainPanel = nullptr;

Camera::Camera(int id): id_(id), cap(0)
{
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    this->Connect(wxID_ANY, wxEVT_TIMER, wxTimerEventHandler(Camera::OnTimer));
    timer_.SetOwner(this);
    timer_.Start(TIMERVALUE, true);
}

void Camera::initMainPanel(wxWindow* parent)
{
    mainPanel = new wxPanel(parent, wxID_ANY);
}

wxPanel* Camera::getMainPanel()
{
    return mainPanel;
}

void Camera::simulate()
{
    //camera objects load their streams in parallel, for now just load an image
    threads.emplace_back(std::thread(&Camera::load, this));
}

void Camera::load()
{
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::string image_name = std::to_string(getID()) + ".png";
    //wxMessageOutputDebug().Printf("getID %d", getID());
    std::cout << image_name << std::endl;
    //image_ = wxImage(image_name, wxBITMAP_TYPE_PNG);
    if ( !image_.IsOk() )
    {
        // need to lock here, accessed by multiple threads
        std::cout << "Can't load image " << image_name << std::endl;
    }
    //panel_ = new wxPanel(mainPanel, wxID_ANY);

}

wxImage Camera::getImage()
{
    return image_;
}


void Camera::OnTimer(wxTimerEvent& event)
{
    if (cap.isOpened())
    {
        cv::Mat frame;

        cap >> frame; // get a new frame from camera
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        image_ = wxImage(frame.cols, frame.rows, frame.data, true);

    }

    else
    {
        std::cout << "Can't load camera " << std::endl;
    }

}