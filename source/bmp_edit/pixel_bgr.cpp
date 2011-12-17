/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                        Functions for BGR Pixel IO                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_BGR_CPP__
#define __PIXEL_BGR_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"



/*---------------------------------------------------------------------------
  The Pixel_BGR_Class class (inherits the PixelClass class).
  ---------------------------------------------------------------------------*/
class Pixel_BGR_Class : public PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Pixel_BGR_Class(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~Pixel_BGR_Class(void) {}


   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, iColorRec* Color)
      {
      *Color = iColorRec((int)*(Ptr+2), 
                         (int)*(Ptr+1), 
                         (int)*(Ptr  ), 0);
      }

   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap, and convert it to float type.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, ColorRec* Color)
      {
      *Color = ColorRec((float)*(Ptr+2), 
                        (float)*(Ptr+1), 
                        (float)*(Ptr  ), 0.0f) * _255inv;
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, iColorRec* Color)
      {       
      *(Ptr+2) = (byte)Color->R; 
      *(Ptr+1) = (byte)Color->G;
      *(Ptr  ) = (byte)Color->B;
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap, and convert it from float type to
      the desired bitmap format.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, ColorRec* Color)
      {
      *(Ptr+2) = (byte)(Color->R * 255.0f); 
      *(Ptr+1) = (byte)(Color->G * 255.0f);
      *(Ptr  ) = (byte)(Color->B * 255.0f);
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif