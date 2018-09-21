/*#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  */

#include "wxNano.h"
#include<iostream>
#include<string>

#include <wx/evtloop.h>

#define myGui wxNano

struct MyStruct {
    cv::Mat img;
    std::string  winname;
};

void onMouse(int evt, int x, int y, int flags, void *f)
{
    if (evt == cv::EVENT_LBUTTONDOWN)
        std::cout << evt << " EVENT_LBUTTONDOWN " << x << " " << y << " " << flags << "\n";
    if (evt == cv::EVENT_LBUTTONUP)
        std::cout << evt << " EVENT_LBUTTONUP " << x << " " << y << " " << flags << "\n";
}

void onTrackbar(int pos, void *userdata)
{
    MyStruct *p = (MyStruct *)userdata;

    cv::Mat img;
    cv::threshold(p->img, img, pos, 255, cv::THRESH_BINARY);
    myGui::imshow(p->winname.c_str(), img);
    std::cout << pos << "\n";
}

int main(int argc,char**argv)
{
/*    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetBreakAlloc(10763);    */
    int l1 = wxNano::GetNumber<int>();
    std::cout << "First number " << l1 << "\n";
/*    unsigned char luc = wxNano::GetNumber<unsigned char>();
    std::cout << "First number " << luc << "\n";
    short ls = wxNano::GetNumber<short>();
    std::cout << "First number " << ls << "\n";
    double l2 = wxNano::GetNumber<double>("texte1","texte2");
    std::cout << "Second number " << l2 << "\n";*/
 
    std::string s = wxNano::GetFileName();
    cv::Mat img = cv::imread(s);
    myGui::namedWindow(s);
    myGui::imshow(s, img);
    s = wxNano::GetFileName();
    cv::Mat img2 = cv::imread(s);
    myGui::imshow(s, img2);
    myGui::setMouseCallback(s,onMouse,NULL);
    int code = 0;
    do
    {
        code = myGui::waitKey(40);
        if (code)
            std::cout << code << "\n";
    } 
    while (code != 27);
    code = 0;
    int valeur = 3;
    int j = 0;
    MyStruct myStruct;
    myStruct.img = img2;
    myStruct.winname = s;
    do
    {
        code = myGui::waitKeyEx(40);
        if (code == 'A')
        {
            myGui::createTrackbar(cv::format("test%d",j++), s.c_str(), &valeur, 256,onTrackbar,&myStruct);
            std::cout << "trackbar " << j << "\n";
        }
        else  if (code)
            std::cout << code << "\n";
    } 
    while (code != 27);

    myGui::destroyWindow("myWxImage");
    myGui::destroyAllWindows();
    _CrtDumpMemoryLeaks();
    return 0;
}
