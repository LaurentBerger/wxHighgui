#include "wxNano.h"
#include<memory>
#include<iostream>
#include<string>
#include <wx/evtloop.h>

int argc = 0;
char **argv = NULL;

class ocvFrame;

std::shared_ptr<wxInitializer> init = InitWX(argc, argv);
std::shared_ptr<std::map<int, int>> InitWX2CVflags();
std::shared_ptr<std::map<int, int>> InitWX2CVtypes();
std::shared_ptr<std::map<std::string, std::shared_ptr<ocvFrame>> > winList;
wxGUIEventLoop *eventLoop;


std::shared_ptr<std::map<int, int>> evtWX2CVflags = InitWX2CVflags();
std::shared_ptr<std::map<int, int>> evtWX2CVtypes = InitWX2CVtypes();

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

std::shared_ptr<std::map<int, int>> InitWX2CVflags()
{
    std::shared_ptr<std::map<int, int>> evtWX2CVflags = std::make_shared<std::map<int, int>>();
    evtWX2CVflags.get()->insert(std::make_pair(wxEVT_LEFT_DOWN, cv::EVENT_FLAG_LBUTTON));
    evtWX2CVflags.get()->insert(std::make_pair(wxEVT_RIGHT_DOWN, cv::EVENT_FLAG_RBUTTON));
    evtWX2CVflags.get()->insert(std::make_pair(wxEVT_MIDDLE_DOWN, cv::EVENT_FLAG_MBUTTON));
    evtWX2CVflags.get()->insert(std::make_pair(wxMOD_CONTROL, cv::EVENT_FLAG_CTRLKEY));
    evtWX2CVflags.get()->insert(std::make_pair(wxMOD_SHIFT, cv::EVENT_FLAG_SHIFTKEY));
    evtWX2CVflags.get()->insert(std::make_pair(wxMOD_ALT, cv::EVENT_FLAG_ALTKEY));

    return evtWX2CVflags;
}

std::shared_ptr<std::map<int, int>> InitWX2CVtypes()
{
    std::shared_ptr<std::map<int, int>> evtWX2CVtypes = std::make_shared<std::map<int, int>>();

    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_MOTION, cv::EVENT_MOUSEMOVE));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_LEFT_DOWN, cv::EVENT_LBUTTONDOWN));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_RIGHT_DOWN, cv::EVENT_RBUTTONDOWN));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_MIDDLE_DOWN, cv::EVENT_MBUTTONDOWN));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_LEFT_UP, cv::EVENT_LBUTTONUP));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_RIGHT_UP, cv::EVENT_RBUTTONUP));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_MIDDLE_UP, cv::EVENT_MBUTTONUP));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_LEFT_DCLICK, cv::EVENT_LBUTTONDBLCLK));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_RIGHT_DCLICK, cv::EVENT_RBUTTONDBLCLK));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_MIDDLE_DCLICK, cv::EVENT_MBUTTONDBLCLK));
    evtWX2CVtypes.get()->insert(std::make_pair(wxEVT_MOUSEWHEEL, cv::EVENT_MOUSEWHEEL));

    return evtWX2CVtypes;
}


enum
{
    ID_ROTATE_LEFT = wxID_HIGHEST + 1,
    ID_ROTATE_RIGHT,
    ID_RESIZE,
    ID_PAINT_BG
};

class ocvImage : public  wxWindow
{
public:
    ocvImage(wxPanel *parent, const wxString& desc, const wxImage& image, const wxPoint pos, const wxSize& s, double scale = 1.0) :
        wxWindow(parent, -1, pos, s)
    {
        posImage = pos;
        InitOcvImage();
        BindEvent();
        m_bitmap = wxBitmap(image, wxBITMAP_SCREEN_DEPTH, scale);
    }

    ocvImage(wxPanel *parent, const wxString& desc, const wxPoint pos, const wxSize& s, double scale = 1.0) :
        wxWindow(parent, -1, pos, s)
    {
        posImage = pos;
        InitOcvImage();
        BindEvent();
        m_bitmap = wxBitmap();
    }

    void setImage(const wxImage& image)
    {
        m_bitmap = wxBitmap(image, wxBITMAP_SCREEN_DEPTH, 1.0);
    }
    int GetKey()
    { 
        int c = key; 
        key = 0; 
        return c; 
    };
    void setMouseCallback(wxNano::MouseCallback om, void *ud = 0)
    {
        userdata = ud;
        mouseCB = om;
    }

    void MoveImage(wxPoint d)
    {
        posImage += d;
    }
private:
    void InitOcvImage()
    {
        mouseCB = NULL;
        userdata = NULL;
        id = 100;
        m_zoom = 1;
    }
    void BindEvent(void)
    {
        Bind(wxEVT_PAINT, &ocvImage::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &ocvImage::OnMouse, this);
        Bind(wxEVT_RIGHT_DOWN, &ocvImage::OnMouse, this);
        Bind(wxEVT_RIGHT_DOWN, &ocvImage::OnMouse, this);
        Bind(wxEVT_LEFT_UP, &ocvImage::OnMouse, this);
        Bind(wxEVT_RIGHT_UP, &ocvImage::OnMouse, this);
        Bind(wxEVT_RIGHT_UP, &ocvImage::OnMouse, this);
        Bind(wxEVT_ERASE_BACKGROUND, &ocvImage::OnEraseBackground, this);
        Bind(wxEVT_KEY_DOWN, &ocvImage::OnKey, this);
    }

    void OnEraseBackground(wxEraseEvent& WXUNUSED(event))
    {
        // do nothing here to be able to see how transparent images are shown
    }

    void OnKey(wxKeyEvent& event)
    {
        key = event.GetKeyCode();
    }

    void OnMouse(wxMouseEvent& event)
    {
        xMouse = event.GetPosition().x;
        yMouse = event.GetPosition().y;
        SetFocus();
        if (mouseCB != NULL)
        {
            int type = 0, flag = 0;
            auto t = evtWX2CVtypes.get()->find(event.GetEventType());
            auto f = evtWX2CVflags.get()->find(event.m_altDown);
            if (t != evtWX2CVtypes.get()->end())
                type = t->second;
            if (f != evtWX2CVflags.get()->end())
                flag = f->second;
            (*mouseCB)(type, xMouse, yMouse, flag, userdata);
        }
        //        eventMouse = event.GetKeyCode();
    }

    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        wxPaintDC dc(this);

        dc.Clear();

        dc.SetUserScale(m_zoom, m_zoom);

        const wxSize size = GetClientSize();
        dc.DrawBitmap(m_bitmap,
            dc.DeviceToLogicalX(0),
            dc.DeviceToLogicalY(0),
            true /* use mask */
        );
/*        dc.DrawBitmap(m_bitmap,
            dc.DeviceToLogicalX((size.x - m_zoom * m_bitmap.GetWidth()) / 2),
            dc.DeviceToLogicalY((size.y - m_zoom * m_bitmap.GetHeight()) / 2),
            true 
        );*/
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

    }

    void OnZoom(wxCommandEvent& event)
    {
        if (event.GetId() == wxID_ZOOM_IN)
            m_zoom *= 1.2;
        else if (event.GetId() == wxID_ZOOM_OUT)
            m_zoom /= 1.2;
        else // wxID_ZOOM_100
            m_zoom = 1.;

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

    }

private:
    wxPoint posImage;
    wxBitmap m_bitmap;
    double m_zoom;
    int key;
    int xMouse, yMouse;
    int eventMouse;
    int flagsMouse;
    wxNano::MouseCallback mouseCB;
    void *userdata;
    wxWindowID id;

};


class ocvFrame : public wxFrame
{
public:
    ocvFrame(wxFrame *parent, const wxString& desc, const wxImage& image, double scale = 1.0)
    {

        InitOcvframe();
        Create(parent, desc, wxBitmap(image, wxBITMAP_SCREEN_DEPTH, scale));
        panel = new wxPanel(this, -1);
        panel->Show();
        wxSize s(image.GetWidth(), image.GetHeight());
        wxPoint p(0, 0);
        ocv = new ocvImage(panel, desc, image,p,s);
        ocv->Move(p);
        ocv->Show(true);
        panel->Fit();

    }
    ocvFrame(wxFrame *parent, const wxString& desc)
    {

        InitOcvframe();
        Create(parent, desc);
        panel = new wxPanel(this, -1);
        panel->Show();
        wxSize s(0, 0);
        wxPoint p(0, 0);
        ocv = new ocvImage(panel, desc, p, s);
        ocv->Move(p);
        ocv->Show(true);
        panel->Fit();

    }

    void MatUpdate(const wxImage& image, const wxString& desc)
    {

        wxSize s(image.GetWidth(), image.GetHeight());
        wxSize sa= GetSize();
        s = wxSize(std::max(sa.GetWidth(), s.GetWidth()), std::max(sa.GetHeight(), s.GetHeight()));
        SetSize(s);
        ocv->SetSize(s);
//        wxPoint p(0, 0);
        ocv->setImage(image);
//        ocv->Move(p);
        ocv->Show(true);
        ocv->Refresh(true);
        panel->Fit();

    }

    void createTrackbar(const std::string &trackbarname, int *value, int count, wxNano::TrackbarCallback onChange, void *userdata)
    {
        if (sliderList.get()->find(trackbarname) != sliderList.get()->end())
            return;
        wxPoint p(0, 0);
        p.y += sliderList.get()->size() * 20;
        wxSize s=ocv->GetClientSize();
        s.SetHeight(20);
        wxSlider *sl = new wxSlider(panel, id++, *value, 0, 100,p,s);
        sl->SetName(wxString(trackbarname));
        sl->Show(true);
        p.y += s.GetHeight();
        ocv->MoveImage(p);
        ocv->Move(p);
        sliderList.get()->insert(std::make_pair(trackbarname, wxNano::TrackbarManager{ sl, onChange, userdata }));
        BindEvent();
        Refresh(true);

    }
    void OnClose(wxCloseEvent& event)
    {
        delete ocv;
        delete panel;
        Destroy();
    }

private:
    void InitOcvframe()
    {
        mouseCB = NULL;
        userdata = NULL;
        sliderList = std::make_shared<std::map<std::string, wxNano::TrackbarManager>>();
        id = 100;
    }
    void BindEvent(void)
    {
        Bind(wxEVT_CLOSE_WINDOW, &ocvFrame::OnClose, this);
        Bind(wxEVT_SLIDER, &ocvFrame::OnSlider, this);
        /*        Bind(wxEVT_LEFT_DOWN, &ocvFrame::OnMouse, this);
        Bind(wxEVT_RIGHT_DOWN, &ocvFrame::OnMouse, this);
        Bind(wxEVT_RIGHT_DOWN, &ocvFrame::OnMouse, this);
        Bind(wxEVT_LEFT_UP, &ocvFrame::OnMouse, this);
        Bind(wxEVT_RIGHT_UP, &ocvFrame::OnMouse, this);
        Bind(wxEVT_RIGHT_UP, &ocvFrame::OnMouse, this);
        Bind(wxEVT_ERASE_BACKGROUND, &ocvFrame::OnEraseBackground, this);
        Bind(wxEVT_KEY_DOWN, &ocvFrame::OnKey, this);*/
    }
    bool Create(wxFrame *parent,
        const wxString& desc,
        const wxBitmap& bitmap)
    {
        Create(parent, desc);

        SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());
        UpdateStatusBar();

        Show();

        return true;
    }

    bool Create(wxFrame *parent,
        const wxString& desc)
    {
        if (!wxFrame::Create(parent, wxID_ANY,
            wxString::Format(wxT("Image from %s"), desc),
            wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE))
            return false;
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
        SetStatusText("images", 1);

        SetClientSize(300, 300);
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

    void OnMouse(wxMouseEvent& event)
    {
        xMouse = event.GetPosition().x;
        yMouse = event.GetPosition().y;
        if (mouseCB != NULL)
        {
            int type = 0, flag = 0;
            auto t = evtWX2CVtypes.get()->find(event.GetEventType());
            auto f = evtWX2CVflags.get()->find(event.m_altDown);
            if (t != evtWX2CVtypes.get()->end())
                type = t->second;
            if (f != evtWX2CVflags.get()->end())
                flag = f->second;
            (*mouseCB)(type, xMouse, yMouse, flag, userdata);
        }
        //        eventMouse = event.GetKeyCode();
    }

    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        return;
    }

    void OnSave(wxCommandEvent& WXUNUSED(event))
    {
    }

    void OnResize(wxCommandEvent& WXUNUSED(event))
    {
/*        wxImage img(m_bitmap.ConvertToImage());

        const wxSize size = GetClientSize();
        img.Rescale(size.x, size.y, wxIMAGE_QUALITY_HIGH);
        m_bitmap = wxBitmap(img);

        UpdateStatusBar();*/
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
    }
    
    void OnSlider(wxCommandEvent & event)
    {
        wxSlider *s=(wxSlider*)(event.GetEventObject());
        wxString name = s->GetName();
        auto q = sliderList.get();
        
        if (q!=NULL)
        {
            auto p = q->find(std::string(name.c_str()));
            if (p != sliderList.get()->end())
            {
                wxNano::TrackbarManager t = p->second;
                if (t.fct)
                    (*t.fct)(s->GetValue(), t.user);
            }
        }
    }

    void UpdateStatusBar()
    {
/*        wxLogStatus(this, wxT("Image size: (%d, %d), zoom %.2f"),
            m_bitmap.GetWidth(),
            m_bitmap.GetHeight(),
            m_zoom);*/
        Refresh();
    }
public :
    ocvImage *ocv;
    wxPanel *panel;
private:
    wxFrame * frameCtrl;
//    wxBitmap m_bitmap;
    double m_zoom;
    int key;
    int xMouse, yMouse;
    int eventMouse;
    int flagsMouse;
    wxNano::MouseCallback mouseCB;
    void *userdata;
    std::shared_ptr<std::map<std::string, wxNano::TrackbarManager >> sliderList;
    wxWindowID id;

};




namespace wxNano {

    void namedWindow(const std::string &winname, int  	flags )
    {
        if (winList.get() == NULL)
            winList = std::make_shared<std::map<std::string, std::shared_ptr<ocvFrame>>>();
        if (winList.get()->find(winname) != winList.get()->end())
            return;
        const wxString desc(winname);

        std::shared_ptr<ocvFrame> x = std::make_shared<ocvFrame>((wxFrame*)NULL, winname);
        winList.get()->insert(make_pair(winname, x));
        x.get()->Show(false);
    }

    void imshow(const std::string &winname, cv::InputArray  	mat)
    {
        if (winList.get() == NULL)
            winList = std::make_shared<std::map<std::string, std::shared_ptr<ocvFrame>>>();
        const wxString desc(winname);
        cv::Mat img = mat.getMat(),imgrgb;
        if (img.channels()!=1)
            cvtColor(img, imgrgb, cv::COLOR_BGR2RGB);
        else
            cvtColor(img, imgrgb, cv::COLOR_GRAY2RGB);
        wxImage image(wxSize(img.cols, img.rows));
        image.SetMask(0);
        image.SetData(imgrgb.ptr(0), true);
        std::shared_ptr<ocvFrame> x;
        if (winList.get()->find(winname) != winList.get()->end())
        {
            x = winList.get()->find(winname)->second;
            x->MatUpdate(image,winname);
        }
        else
        {
            x = std::make_shared<ocvFrame>((wxFrame*)NULL, desc, image, 1.0);
            winList.get()->insert(make_pair(winname, x));
        }
        x.get()->Show(true);
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
            c = o.get()->ocv->GetKey();
            if (c)
                return c;
        }
        return c;
    }

    void destroyAllWindows()
    {
        if (!winList.get())
            return;
        for (auto q : *winList.get())
        {
            std::shared_ptr<ocvFrame> o = q.second;
            wxCloseEvent w(wxEVT_CLOSE_WINDOW, -1);
            q.second->OnClose(w);
        }
        winList.get()->clear();
    }
    void destroyWindow(const std::string &  	winname)
    {
        if (!winList.get())
            return;
        auto q = winList.get()->find(winname);
        if (q != winList.get()->end())
        {
            wxCloseEvent w(wxEVT_CLOSE_WINDOW, -1);
            q->second.get()->OnClose(w);
            winList.get()->erase(winname);

        }

    }
    void setMouseCallback(const std::string &winname, MouseCallback onMouse, void *userdata)
    {
        auto q = winList.get()->find(winname);
        if (q != winList.get()->end())
        {
            q->second.get()->ocv->setMouseCallback(onMouse, userdata);
        }

    }
    void createTrackbar(const std::string &trackbarname, const std::string &winname, int *value, int count, TrackbarCallback onChange, void *userdata)
    {
        auto q = winList.get()->find(winname);
        if (q != winList.get()->end())
        {
            q->second.get()->createTrackbar(trackbarname, value, count,onChange, userdata);
        }

    }

}

