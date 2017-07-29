//
// MATLAB Compiler: 5.2 (R2014b)
// Date: Fri May 12 16:33:22 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:virtualTrebuchetV5" "-T"
// "link:lib" "virtualTrebuchetV5.m" 
//

#ifndef __virtualTrebuchetV5_h
#define __virtualTrebuchetV5_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_virtualTrebuchetV5
#define PUBLIC_virtualTrebuchetV5_C_API __global
#else
#define PUBLIC_virtualTrebuchetV5_C_API /* No import statement needed. */
#endif

#define LIB_virtualTrebuchetV5_C_API PUBLIC_virtualTrebuchetV5_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_virtualTrebuchetV5
#define PUBLIC_virtualTrebuchetV5_C_API __declspec(dllexport)
#else
#define PUBLIC_virtualTrebuchetV5_C_API __declspec(dllimport)
#endif

#define LIB_virtualTrebuchetV5_C_API PUBLIC_virtualTrebuchetV5_C_API


#else

#define LIB_virtualTrebuchetV5_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_virtualTrebuchetV5_C_API 
#define LIB_virtualTrebuchetV5_C_API /* No special import/export declaration */
#endif

extern LIB_virtualTrebuchetV5_C_API 
bool MW_CALL_CONV virtualTrebuchetV5InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_virtualTrebuchetV5_C_API 
bool MW_CALL_CONV virtualTrebuchetV5Initialize(void);

extern LIB_virtualTrebuchetV5_C_API 
void MW_CALL_CONV virtualTrebuchetV5Terminate(void);



extern LIB_virtualTrebuchetV5_C_API 
void MW_CALL_CONV virtualTrebuchetV5PrintStackTrace(void);

extern LIB_virtualTrebuchetV5_C_API 
bool MW_CALL_CONV mlxVirtualTrebuchetV5(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                        *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_virtualTrebuchetV5
#define PUBLIC_virtualTrebuchetV5_CPP_API __declspec(dllexport)
#else
#define PUBLIC_virtualTrebuchetV5_CPP_API __declspec(dllimport)
#endif

#define LIB_virtualTrebuchetV5_CPP_API PUBLIC_virtualTrebuchetV5_CPP_API

#else

#if !defined(LIB_virtualTrebuchetV5_CPP_API)
#if defined(LIB_virtualTrebuchetV5_C_API)
#define LIB_virtualTrebuchetV5_CPP_API LIB_virtualTrebuchetV5_C_API
#else
#define LIB_virtualTrebuchetV5_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_virtualTrebuchetV5_CPP_API void MW_CALL_CONV virtualTrebuchetV5(int nargout, mwArray& Aq, mwArray& Sq, mwArray& X3Inter, mwArray& Y3Inter, const mwArray& Aq0);

#endif
#endif
