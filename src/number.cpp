#include "wx/wx.h"
#include <wx/numdlg.h>
#include<iostream>
#include<string>

class GetNumberApp : public wxApp
{
public:
    int number;
    virtual bool OnInit()
    {
        number = wxGetNumberFromUser("enter number", "your prompt here", "your caption here", 1);

        return false;
    }
};

class GetTextApp : public wxApp
{
public:
    wxString texte;
    virtual bool OnInit()
    {
        texte = wxGetTextFromUser("enter number", "your prompt here", "");

        return false;
    }

};

class GetFileNameApp : public wxApp
{
public:
    std::string fileName;
    virtual bool OnInit()
    {
        fileName = wxFileSelector("Choose a file to open");

        return false;
    }
};

std::string GetFileName()
{
    std::string x;
    GetFileNameApp* pApp = new GetFileNameApp();
    wxApp::SetInstance(pApp);
    wxEntryStart(0, NULL);
    wxTheApp->OnInit();

    x = pApp->fileName;
    // cleaning up...
    wxTheApp->OnExit();
    wxEntryCleanup();

    return x;
}

template<class T>T GetNumber()
{
    T x;
    GetNumberApp* pApp = new GetNumberApp();
    wxApp::SetInstance(pApp);
    wxEntryStart(0, NULL);
    wxTheApp->OnInit();

    x = pApp->number;
    // cleaning up...
    wxTheApp->OnExit();
    wxEntryCleanup();

    return x;
}

template<>double GetNumber<double>()
{
    double x;
    GetTextApp* pApp = new GetTextApp();
    wxApp::SetInstance(pApp);
    wxEntryStart(0, NULL);
    wxTheApp->OnInit();

    pApp->texte.ToCDouble(&x);
    // cleaning up...
    wxTheApp->OnExit();
    wxEntryCleanup();

    return x;
}
