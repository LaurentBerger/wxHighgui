#include "wxNano.h"
#include<iostream>
#include<string>

#include <wx/evtloop.h>

#define myGui wxNano

void onMouse(int evt, int x, int y, int flags, void *f)
{
    if (evt == cv::EVENT_LBUTTONDOWN)
        std::cout << evt << " EVENT_LBUTTONDOWN " << x << " " << y << " " << flags << "\n";
    if (evt == cv::EVENT_LBUTTONUP)
        std::cout << evt << " EVENT_LBUTTONUP " << x << " " << y << " " << flags << "\n";
}

void onTrackbar(int pos, void *userdata)
{
    std::cout << pos << "\n";
}

int main(int argc,char**argv)
{
    
    int l1 = wxNano::GetNumber<int>();
    std::cout << "First number " << l1 << "\n";
   
/*    unsigned char luc = wxNano::GetNumber<unsigned char>();
    std::cout << "First number " << luc << "\n";
    short ls = wxNano::GetNumber<short>();
    std::cout << "First number " << ls << "\n";
    double l2 = wxNano::GetNumber<double>("texte1","texte2");
    std::cout << "Second number " << l2 << "\n";*/
 
    std::string s = wxNano::GetFileName();
    cv::Mat img = cv::imread(s.c_str());
//    myGui::namedWindow()
    myGui::imshow(s.c_str(), img);
/*    s = wxNano::GetFileName();
    cv::Mat img2 = cv::imread(s.c_str());
    myGui::imshow(s.c_str(), img2);*/
    myGui::setMouseCallback(s.c_str(),onMouse,NULL);
    int code = 0;
    do
    {
        code = myGui::waitKey(1000);
        if (code)
            std::cout << code << "\n";
    } 
    while (code != 27);
    code = 0;
    int valeur = 3;
    int j = 0;
    do
    {
        code = myGui::waitKey(1000);
        if (code == 'A')
        {
            myGui::createTrackbar(cv::format("test%d",j++), s.c_str(), &valeur, 256,onTrackbar);
            std::cout << "trackbar " << j << "\n";
        }
        else  if (code)
            std::cout << code << "\n";
    } 
    while (code != 27);

    myGui::destroyWindow("myWxImage");
    myGui::destroyAllWindows();

    return 0;
}
