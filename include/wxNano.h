#ifndef __WXNANO_LIB__
#define __WXNANO_LIB__
#include "wx/wx.h"
#include <wx/numdlg.h>
#include<string>
#include<memory>

std::shared_ptr<wxInitializer> InitWX(int , char **);



template<typename T>T GetNumber()
{
    T x;
    x= wxGetNumberFromUser("enter number", "your prompt here", "your caption here", 1);
    return x;
}

template<>double GetNumber<double>();

std::string GetFileName();

#endif

