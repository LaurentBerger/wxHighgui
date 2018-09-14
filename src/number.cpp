#include "wxNano.h"
#include<memory>
#include<iostream>
#include<string>

int argc = 0;
char **argv = NULL;
std::shared_ptr<wxInitializer> init=InitWX(argc,argv);


std::shared_ptr<wxInitializer> InitWX(int argc,char **argv)
{
    std::shared_ptr<wxInitializer> init= std::make_shared<wxInitializer>(argc, argv);
    if (!init->IsOk()) {
        wxPrintf("Failed to initialize wxWidgets.\n");
        return init;
    }

    wxApp::SetInstance(new wxApp());
    return init;
}

std::string GetFileName()
{
    wxString fileName = wxFileSelector("Choose a file to open");
    return std::string(fileName.c_str());
}

template<>double GetNumber<double>(std::string texte1 = std::string(), std::string texte2 = std::string(), std::string texte3 = std::string())
{
    double x;

    wxString texte = wxGetTextFromUser("enter number", "your prompt here", "");
    texte.ToCDouble(&x);

    return x;
}

