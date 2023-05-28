#ifndef CAMERAPANEL_H
#define CAMERAPANEL_H

#include <wx/wx.h>
#include "Camera.h"

class CameraPanel : public wxPanel {
public:
    CameraPanel(wxWindow *parent, wxWindowID id, std::shared_ptr<Camera> camera)
        : wxPanel(parent, id), m_camera(camera) {
        Bind(wxEVT_PAINT, &CameraPanel::OnPaint, this);
    }

private:
    void OnPaint(wxPaintEvent &event) {
        wxPaintDC dc(this);
        wxSize panelSize = GetSize();
        wxBitmap scaledImage = wxBitmap(
            m_camera->getImage().Scale(panelSize.GetWidth(), panelSize.GetHeight(), wxIMAGE_QUALITY_NORMAL));
        dc.DrawBitmap(scaledImage, 0, 0);
    }

    std::shared_ptr<Camera> m_camera;
};

#endif