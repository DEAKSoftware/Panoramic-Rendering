/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                   Functions for Lo Color Pixel IO                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_LO_CPP__
#define __PIXEL_LO_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"



/*---------------------------------------------------------------------------
  The Pixel_LO_Class class (inherits the PixelClass class).
  ---------------------------------------------------------------------------*/
class Pixel_LO_Class : public PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Pixel_LO_Class(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~Pixel_LO_Class(void) {}


   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, iColorRec* Color)
      {
      *Color = iColorRec((int)((*((word*)Ptr) & 0x00007C00) >> 7),
                         (int)((*((word*)Ptr) & 0x000003E0) >> 2),
                         (int)((*((word*)Ptr) & 0x0000001F) << 3), 0);
      }

   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap, and convert it to float type.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, ColorRec* Color)
      {
      *Color =  ColorRec((float)((*((word*)Ptr) & 0x00007C00) >> 7),
                         (float)((*((word*)Ptr) & 0x000003E0) >> 2),
                         (float)((*((word*)Ptr) & 0x0000001F) << 3), 0) * _255inv;
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, iColorRec* Color)
      {
      *(word*)Ptr = (word)(
                          ((Color->R << 7) & 0x00007C00) |
                          ((Color->G << 2) & 0x000003E0) |
                           (Color->B >> 3)
                          );
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap, and convert it from float type to
      the desired bitmap format.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, ColorRec* Color)
      {
      *(word*)Ptr = (word)(
                          (((word)(Color->R * 255.0f) << 7) & 0x00007C00) |
                          (((word)(Color->G * 255.0f) << 2) & 0x000003E0) |
                           ((word)(Color->B * 255.0f) >> 3)
                          );
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif