/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/* Header file for the 3D engine. Common definitions that are used            */
/* through-out the code are placed here.                                      */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __STD_INC_H__
#define __STD_INC_H__


/*----------------------------------------------------------------------------
   Standard library includes
  ----------------------------------------------------------------------------*/
//Include all the standard libraries needed by the 3D engine
#if defined (WIN32) || defined (WIN32_NT)
#  include <windows.h>
#  include <winbase.h>
#else
#  include <varargs.h>
#  include <strings.h>
#  define  stricmp  strcasecmp                  //This function name must be given an alias in Linux
#  define  strnicmp strncasecmp                 //This function name must be given an alias in Linux
#endif
#include <gl.h>
#include <glu.h>
#include <glut.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <time.h>


/*----------------------------------------------------------------------------
   Type definitions and keyword definitions
  ----------------------------------------------------------------------------*/
#if defined (WIN32) || defined (WIN32_NT)
   typedef signed __int16     sint;
   typedef signed __int64     qint;
   typedef unsigned char      byte;
   typedef unsigned __int16   word;
   typedef unsigned __int32   dword;
   typedef unsigned __int64   qword;
#else
   typedef signed short int   sint;
   typedef signed long int    qint;
   typedef unsigned char      byte;
   typedef unsigned short int word;
   typedef unsigned int       dword;
   typedef unsigned long int  qword;
   #define __cdecl
   #define __forceinline inline
#endif


/*----------------------------------------------------------------------------
   3D Engine title
  ----------------------------------------------------------------------------*/
char* Title = "Cosmic Ray [Tau] - by Dominik Deak";


/*==== End of file ===========================================================*/
#endif
