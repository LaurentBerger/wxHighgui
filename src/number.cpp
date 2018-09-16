#include "wxNano.h"
#include<memory>
#include<iostream>
#include<string>
#include <wx/evtloop.h>

int argc = 0;
char **argv = NULL;

class ocvFrame;

std::shared_ptr<wxInitializer> init = InitWX(argc, argv);
std::shared_ptr<std::map<std::string, std::shared_ptr<ocvFrame>> > winList;
wxGUIEventLoop *eventLoop;

std::shared_ptr<wxInitializer> InitWX(int argc,char **argv)
{
    //https://docs.wxwidgets.org/trunk/classwx_initializer.html#a7b53997659917e3703a6fe2950fe56a2
    std::shared_ptr<wxInitializer> init= std::make_shared<wxInitializer>(argc, argv);
    wxInitializeStockLists();
    if (!init.get()->IsOk()) {
        wxPrintf("Failed to initialize wxWidgets.\n");
        return init;
    }
    // https://docs.wxwidgets.org/trunk/classwx_app_console.html#a7ca751aa37cd3f920e20d9d967ad413d
    wxApp::SetInstance(new wxApp());
    eventLoop = new wxGUIEventLoop;
    return init;
}

enum
{
    ID_ROTATE_LEFT = wxID_HIGHEST + 1,
    ID_ROTATE_RIGHT,
    ID_RESIZE,
    ID_PAINT_BG
};





class ocvFrame : public wxFrame
{
public:
    ocvFrame(wxFrame *parent, const wxString& desc, const wxImage& image, double scale = 1.0)
    {
        Create(parent, desc, wxBitmap(image, wxBITMAP_SCREEN_DEPTH, scale),
            image.GetImageCount(desc));
        Bind(wxEVT_PAINT,&ocvFrame::OnPaint,this);
        Bind(wxEVT_ERASE_BACKGROUND, &ocvFrame::OnEraseBackground, this);
        Bind(wxEVT_KEY_DOWN, &ocvFrame::OnKey, this);
    }

    ocvFrame(wxFrame *parent, const wxString& desc, const wxBitmap& bitmap)
    {
        Create(parent, desc, bitmap);
        Bind(wxEVT_PAINT, &ocvFrame::OnPaint, this);
        Bind(wxEVT_ERASE_BACKGROUND, &ocvFrame::OnEraseBackground, this);
        Bind(wxEVT_KEY_DOWN, &ocvFrame::OnKey, this);
    }
    int GetKey() { int c = key; key = 0; return c; };

private:
    bool Create(wxFrame *parent,
        const wxString& desc,
        const wxBitmap& bitmap,
        int numImages = 1)
    {
        if (!wxFrame::Create(parent, wxID_ANY,
            wxString::Format(wxT("Image from %s"), desc),
            wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE))
            return false;

        m_bitmap = bitmap;
        m_zoom = 1.;

        wxMenu *menu = new wxMenu;
        menu->Append(wxID_SAVEAS);
        menu->AppendSeparator();
        menu->AppendCheckItem(ID_PAINT_BG, wxT("&Paint background"),
            "Uncheck this for transparent images");
        menu->AppendSeparator();
        menu->Append(ID_RESIZE, wxT("&Fit to window\tCtrl-F"));
        menu->Append(wxID_ZOOM_IN, "Zoom &in\tCtrl-+");
        menu->Append(wxID_ZOOM_OUT, "Zoom &out\tCtrl--");
        menu->Append(wxID_ZOOM_100, "Reset zoom to &100%\tCtrl-1");
        menu->AppendSeparator();
        menu->Append(ID_ROTATE_LEFT, wxT("Rotate &left\tCtrl-L"));
        menu->Append(ID_ROTATE_RIGHT, wxT("Rotate &right\tCtrl-R"));

        wxMenuBar *mbar = new wxMenuBar;
        mbar->Append(menu, wxT("&Image"));
        SetMenuBar(mbar);

        mbar->Check(ID_PAINT_BG, true);

        CreateStatusBar(2);
        if (numImages != 1)
            SetStatusText(wxString::Format("%d images", numImages), 1);

        SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());

        UpdateStatusBar();

        Show();

        return true;
    }

    void OnEraseBackground(wxEraseEvent& WXUNUSED(event))
    {
        // do nothing here to be able to see how transparent images are shown
    }

    void OnKey(wxKeyEvent& event)
    {
        key = event.GetKeyCode();
    }

    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        wxPaintDC dc(this);

        if (GetMenuBar()->IsChecked(ID_PAINT_BG))
            dc.Clear();

        dc.SetUserScale(m_zoom, m_zoom);

        const wxSize size = GetClientSize();
        dc.DrawBitmap
        (
            m_bitmap,
            dc.DeviceToLogicalX((size.x - m_zoom * m_bitmap.GetWidth()) / 2),
            dc.DeviceToLogicalY((size.y - m_zoom * m_bitmap.GetHeight()) / 2),
            true /* use mask */
        );
    }

    void OnSave(wxCommandEvent& WXUNUSED(event))
    {
    }

    void OnResize(wxCommandEvent& WXUNUSED(event))
    {
        wxImage img(m_bitmap.ConvertToImage());

        const wxSize size = GetClientSize();
        img.Rescale(size.x, size.y, wxIMAGE_QUALITY_HIGH);
        m_bitmap = wxBitmap(img);

        UpdateStatusBar();
    }

    void OnZoom(wxCommandEvent& event)
    {
        if (event.GetId() == wxID_ZOOM_IN)
            m_zoom *= 1.2;
        else if (event.GetId() == wxID_ZOOM_OUT)
            m_zoom /= 1.2;
        else // wxID_ZOOM_100
            m_zoom = 1.;

        UpdateStatusBar();
    }

    void OnRotate(wxCommandEvent& event)
    {
        double angle = 5;
        if (event.GetId() == ID_ROTATE_LEFT)
            angle = -angle;

        wxImage img(m_bitmap.ConvertToImage());
        img = img.Rotate(angle, wxPoint(img.GetWidth() / 2, img.GetHeight() / 2));
        if (!img.IsOk())
        {
            wxLogWarning(wxT("Rotation failed"));
            return;
        }

        m_bitmap = wxBitmap(img);

        UpdateStatusBar();
    }

    void UpdateStatusBar()
    {
        wxLogStatus(this, wxT("Image size: (%d, %d), zoom %.2f"),
            m_bitmap.GetWidth(),
            m_bitmap.GetHeight(),
            m_zoom);
        Refresh();
    }

    private:
    wxBitmap m_bitmap;
    double m_zoom;
    int key;

};

namespace wxNano {

    void imshow(const std::string &winname, cv::InputArray  	mat)
    {
        if (winList.get() == NULL)
            winList = std::make_shared<std::map<std::string, std::shared_ptr<ocvFrame>>>();
        const wxString desc(winname);
        cv::Mat img = mat.getMat();
        wxImage *image = new wxImage(wxSize(img.cols,img.rows));
        image->SetMask(0);
        image->SetData(img.ptr(0), true);

        std::shared_ptr<ocvFrame> x = std::make_shared<ocvFrame>((wxFrame*)NULL, desc, *image, 1.0);
        winList.get()->insert(make_pair(winname, x));
        x.get()->Show();
    }

    std::string GetFileName()
    {
        wxString fileName = wxFileSelector("Choose a file to open");
        return std::string(fileName.c_str());
    }

    template<>double GetNumber<double>(std::string texte1 = std::string(), std::string texte2 = std::string(), std::string texte3 = std::string())
    {
        double x;

        wxString texte = wxGetTextFromUser(wxString(texte1), wxString(texte2), wxString(texte3));
        texte.ToCDouble(&x);

        return x;
    }

    int waitKey(int tps)
    {
        eventLoop->DispatchTimeout(tps);
        int c = 0;
        for (auto q : *winList.get())
        {
            std::shared_ptr<ocvFrame> o = q.second;
            c = o.get()->GetKey();
        }
        return c;
    }

    void destroyAllWindows()
    {
        for (auto q : *winList.get())
        {
            std::shared_ptr<ocvFrame> o = q.second;
            q.second->Close(true);
        }

    }
    void destroyWindow(const std::string &  	winname)
    {
        for (auto q : *winList.get())
        {
            std::shared_ptr<ocvFrame> o = q.second;
            if (q.first==winname)
                q.second->Close(true);
        }

    }

}


