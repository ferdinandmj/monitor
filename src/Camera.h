#ifndef CAMERA_H
#define CAMERA_H

#include <wx/wx.h>
#include "MonitorObject.h"

class Camera: public MonitorObject
{
public:
	Camera(int id) {id_ = id;};
	void simulate(); //
	int getID() {return id_;};
	static void initMainPanel(wxWindow* parent);
	static wxPanel* getMainPanel();
	wxImage getImage();

private:
	static wxPanel *mainPanel;
	void load(); // load image from file
	int id_;
	wxImage image_;
	wxPanel *panel_;
	wxBitmap scaledBg_;
};

#endif
