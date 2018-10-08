// Contents of DLLDefines.h
#ifndef _wxHighgui_DLLDEFINES_H_
#define _wxHighgui_DLLDEFINES_H_

/* Cmake will define MyLibrary_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MyLibrary_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32)
  #if defined(wxHighgui_EXPORTS)
    #define  wxHighgui_EXPORT __declspec(dllexport)
  #else 
	#if defined(WXUSINGDLL)
		#define  wxHighgui_EXPORT __declspec(dllimport)
      #else
		#define  wxHighgui_EXPORT
	#endif
  #endif /* wxHighgui_EXPORTS */
#else /* defined (_WIN32) */
 #define wxHighgui_EXPORT
#endif

#endif /* _wxHighgui_DLLDEFINES_H_ */
