/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                       The 3D Point Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __POINTREC_H__
#define __POINTREC_H__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#if defined (INTEL_x86_SSE)
   #include "../math/pointrec_float.x86_sse.cpp"
#else
   #include "../math/pointrec_float.cpp"
#endif

#include "../math/pointrec_int.cpp"


/*==== End of file ===========================================================*/
#endif