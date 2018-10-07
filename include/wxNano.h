#ifndef __WXNANO_LIB__
#define __WXNANO_LIB__
#include "DLLDefines.h"
#include "wx/wx.h"
#include <wx/numdlg.h>
#include<string>
#include<vector>
#include<map>
#include<memory>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>


wxHighgui_EXPORT std::shared_ptr<wxInitializer> InitWX(int, char **);

namespace wxNano {

typedef void(*MouseCallback) (int event, int x, int y, int flags, void *userdata);
typedef void(*TrackbarCallback) (int pos, void *userdata);

wxHighgui_EXPORT  struct TrackbarManager
{
    wxSlider *s;
    TrackbarCallback fct;
    void *user;
};
template<typename T>wxHighgui_EXPORT T GetNumber(std::string texte1 = std::string(), std::string texte2 = std::string(), std::string texte3 = std::string());


template<typename T> T GetNumber(std::string texte1, std::string texte2, std::string texte3)
{
    T x;
    if (typeid(T) == typeid(int))
        x = wxGetNumberFromUser("enter number", "your prompt here", "your caption here", 1, INT_MIN, INT_MAX);
    if (typeid(T) == typeid(short))
        x = wxGetNumberFromUser("enter number", "your prompt here", "your caption here", 1, SHRT_MIN, SHRT_MAX);
    if (typeid(T) == typeid(unsigned char))
        x = wxGetNumberFromUser("enter number", "your prompt here", "your caption here", 1, 0, 255);
    if (typeid(T) == typeid(char))
        x = wxGetNumberFromUser("enter number", "your prompt here", "your caption here", 1, -128, 127);
    return x;
}

template<> wxHighgui_EXPORT double GetNumber<double>(std::string texte1, std::string texte2, std::string texte3);

wxHighgui_EXPORT std::string GetFileName();
wxHighgui_EXPORT int waitKey(int tps=0);
wxHighgui_EXPORT int waitKeyEx(int delay = 0);
wxHighgui_EXPORT void imshow(const std::string &  	winname, cv::InputArray  	mat);
wxHighgui_EXPORT void namedWindow(const std::string &winname, int  	flags = cv::WINDOW_AUTOSIZE);
wxHighgui_EXPORT void destroyAllWindows();
wxHighgui_EXPORT void destroyWindow(const std::string &  	winname);
wxHighgui_EXPORT void setMouseCallback(const std::string &winname, MouseCallback onMouse, void *userdata = NULL);
wxHighgui_EXPORT void createTrackbar(const std::string &trackbarname, const std::string &winname, int *value, int count, TrackbarCallback onChange=NULL, void *userdata=NULL);
wxHighgui_EXPORT int getMouseWheelDelta();
wxHighgui_EXPORT int getTrackbarPos(const std::string &trackbarname, const std::string &winname);

wxHighgui_EXPORT void moveWindow(const std::string &winname, int x, int y);
wxHighgui_EXPORT void resizeWindow(const std::string &winname, int x, int y);
wxHighgui_EXPORT void resizeWindow(const std::string &winname, cv::Size &size);
wxHighgui_EXPORT void setTrackbarMax(const std::string &trackbarname, const std::string &winname, int maxval);
wxHighgui_EXPORT void setTrackbarMin(const std::string &trackbarname, const std::string &winname, int minval);
wxHighgui_EXPORT void setTrackbarPos(const std::string &trackbarname, const std::string &winname, int pos);
wxHighgui_EXPORT void setWindowTitle(const std::string &winname, const std::string &title);
};




#endif

