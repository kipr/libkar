#ifndef _COMPAT_HPP_
#define _COMPAT_HPP_

#ifdef WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#endif