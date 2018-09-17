#include "wxNano.h"
#include<iostream>
#include<string>

#include <wx/evtloop.h>

#define myGui wxNano

class MyEventLoop : public wxEventLoopBase 
{ 
};

void main(int argc,char**argv)
{
    
    int l1 = wxNano::GetNumber<int>();
    std::cout << "First number " << l1 << "\n";
   
    unsigned char luc = wxNano::GetNumber<unsigned char>();
    std::cout << "First number " << luc << "\n";
    short ls = wxNano::GetNumber<short>();
    std::cout << "First number " << ls << "\n";
    double l2 = wxNano::GetNumber<double>("texte1","texte2");
    std::cout << "Second number " << l2 << "\n";
 
    std::string s = wxNano::GetFileName();
    cv::Mat img = cv::imread(s.c_str());
    cv::Mat img2 = cv::imread("g:/lib/opencv/samples/data/lena.jpg");
    myGui::imshow("myWxImage", img);
    myGui::imshow("myWxImage2", img2);
    int code = 0;
    do
    {
        code = myGui::waitKey(1000);
        if (code)
            std::cout << code << "\n";
    } 
    while (code != 27);
    std::cout << "File name " << s << "\n";

    myGui::destroyWindow("myWxImage");
    myGui::destroyAllWindows();

    // cleaning up...
}
