#include "wxNano.h"
#include<iostream>
#include<string>

#include <wx/evtloop.h>

#define myGui wxNano

void onMouse(int evt, int x, int y, int flags, void *f)
{
    std::cout << evt << " " << x << " " << y << " " << flags;
}

int main(int argc,char**argv)
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
    myGui::imshow(s.c_str(), img);
    s = wxNano::GetFileName();
    cv::Mat img2 = cv::imread(s.c_str());
    myGui::imshow(s.c_str(), img2);
    myGui::setMouseCallback(s.c_str(),onMouse,NULL);
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

    return 0;
}
