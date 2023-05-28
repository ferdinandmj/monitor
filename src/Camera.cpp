#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include "Camera.h"

wxPanel* Camera::mainPanel = nullptr;

void Camera::initMainPanel(wxWindow* parent) {
    mainPanel = new wxPanel(parent, wxID_ANY);
}

wxPanel* Camera::getMainPanel() {
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
    std::string image_name = std::to_string(getID()+1) + ".png";
	image_ = wxImage("1.png", wxBITMAP_TYPE_PNG);
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