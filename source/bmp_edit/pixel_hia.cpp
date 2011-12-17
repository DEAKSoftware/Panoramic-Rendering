/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                   Functions for Hi + Alpha Color Pixel IO                  */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_HIA_CPP__
#define __PIXEL_HIA_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"



/*---------------------------------------------------------------------------
  The Pixel_HIA_Class class (inherits the PixelClass class).
  ---------------------------------------------------------------------------*/
class Pixel_HIA_Class : public PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Pixel_HIA_Class(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~Pixel_HIA_Class(void) {}


   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, iColorRec* Color)
      {
      *Color = iColorRec((int)((*((word*)Ptr) & 0x0000F800) >> 8),
                         (int)((*((word*)Ptr) & 0x000007E0) >> 3),
                         (int)((*((word*)Ptr) & 0x0000001F) << 3), 
                         (int)*(Ptr+2));
      }

   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap, and convert it to float type.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, ColorRec* Color)
      {
      *Color =  ColorRec((float)((*((word*)Ptr) & 0x0000F800) >> 8),
                         (float)((*((word*)Ptr) & 0x000007E0) >> 3),
                         (float)((*((word*)Ptr) & 0x0000001F) << 3),
                         (float)*(Ptr+2)) * _255inv;
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, iColorRec* Color)
      {
      *(word*)Ptr = (word)(
                          ((Color->R << 8) & 0x0000F800) |
                          ((Color->G << 3) & 0x000007E0) |
                           (Color->B >> 3)
                          );
      
      *(Ptr+2) = (byte)Color->A;
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap, and convert it from float type to
      the desired bitmap format.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, ColorRec* Color)
      {
      *(word*)Ptr = (word)(
                          (((word)(Color->R * 255.0f) << 8) & 0x0000F800) |
                          (((word)(Color->G * 255.0f) << 3) & 0x000007E0) |
                           ((word)(Color->B * 255.0f) >> 3)
                          );
      
      *(Ptr+2) = (byte)(Color->A * 255.0f);
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif