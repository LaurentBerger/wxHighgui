#include "wxNano.h"
#include<iostream>
#include<string>

#include <wx/evtloop.h>

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
 
    cv::Mat img = cv::imread("g:/lib/opencv/samples/data/baboon.jpg");
    wxNano::imshow("test", img);
    int code = 0;
    do
    {
        code = wxNano::waitKey(1000);
        std::cout << code << "\n";
    } 
    while (code != 27);
    std::string s = wxNano::GetFileName();
    std::cout << "File name " << s << "\n";
//    dlg->Destroy();

    // cleaning up...
}
