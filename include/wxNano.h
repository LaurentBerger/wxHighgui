#ifndef __WXNANO_LIB__
#define __WXNANO_LIB__
#include "wx/wx.h"
#include <wx/numdlg.h>
#include<string>
#include<memory>
#include<opencv2/opencv.hpp>


std::shared_ptr<wxInitializer> InitWX(int , char **);

namespace wxNano {

typedef void(*MouseCallback) (int event, int x, int y, int flags, void *userdata);
typedef void(*TrackbarCallback) (int pos, void *userdata);

template<typename T>T GetNumber(std::string texte1 = std::string(), std::string texte2 = std::string(), std::string texte3 = std::string())
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

template<>double GetNumber<double>(std::string texte1, std::string texte2, std::string texte3);

std::string GetFileName();
int waitKey(int tps);
void imshow(const std::string &  	winname, cv::InputArray  	mat);
void destroyAllWindows();
void destroyWindow(const std::string &  	winname);
void setMouseCallback(const std::string &winname, MouseCallback onMouse, void *userdata = NULL);
void createTrackbar(const std::string &trackbarname, const std::string &winname, int *value, int count, TrackbarCallback onChange=NULL, void *userdata=NULL);

};




#endif

