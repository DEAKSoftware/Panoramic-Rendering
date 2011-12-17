/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                          The Color Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_H__
#define __PIXEL_H__


/*---------------------------------------------------------------------------
  The standard PixelClass.
  ---------------------------------------------------------------------------*/
class PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   byte* ColorTablePtr;

   /*---- Constructor --------------------------------------------------------*/
   PixelClass(void) {ColorTablePtr = NULL;}

   /*---- Destructor ---------------------------------------------------------*/
   ~PixelClass(void) {}

   /*-------------------------------------------------------------------------
      Dummy pixel IO functions.
     ------------------------------------------------------------------------*/
   virtual inline void Read (byte* Ptr, iColorRec* Color) = NULL;
   virtual inline void Read (byte* Ptr, ColorRec*  Color) = NULL;
   virtual inline void Write(byte* Ptr, iColorRec* Color) = NULL;
   virtual inline void Write(byte* Ptr, ColorRec*  Color) = NULL;

   /*==== End of Class =======================================================*/
   };

/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../bmp_edit/pixel_null.cpp"
#include "../bmp_edit/pixel_a.cpp"
#include "../bmp_edit/pixel_c.cpp"
#include "../bmp_edit/pixel_lo.cpp"
#include "../bmp_edit/pixel_hi.cpp"
#include "../bmp_edit/pixel_hia.cpp"
#include "../bmp_edit/pixel_rgb.cpp"
#include "../bmp_edit/pixel_rgba.cpp"
#include "../bmp_edit/pixel_argb.cpp"
#include "../bmp_edit/pixel_bgr.cpp"
#include "../bmp_edit/pixel_abgr.cpp"
#include "../bmp_edit/pixel_bgra.cpp"
#include "../bmp_edit/pixel_yuv.cpp"


/*==== End of file ===========================================================*/
#endif
