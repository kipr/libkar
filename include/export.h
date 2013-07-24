#ifndef _EXPORT_H_
#define _EXPORT_H_

#ifdef WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#endif