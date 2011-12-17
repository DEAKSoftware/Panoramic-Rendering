/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                        Functions for YUV Pixel IO                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_YUV_CPP__
#define __PIXEL_YUV_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"



/*---------------------------------------------------------------------------
  The Pixel_YUV_Class class (inherits the PixelClass class).
  ---------------------------------------------------------------------------*/
class Pixel_YUV_Class : public PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Pixel_YUV_Class(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~Pixel_YUV_Class(void) {}


   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, iColorRec* Color)
      {
      //Convert YUV values from 9.7 fixed point format to 0.0 - 255.0
      float Y = fix97_to_float(*(sint*)(Ptr  ));        //Y is always positive
      float U = fix97_to_float(*(sint*)(Ptr+2));
      float V = fix97_to_float(*(sint*)(Ptr+4));

      //Compute the RGB colors
      Color->R = (int)(1.15f*U + Y);
      Color->B = (int)(2.03f*V + Y);
      Color->G = (int)(1.704f*(Y - 0.299f*(float)Color->R - 0.114f*(float)Color->B)); //G = (Y - 0.299*R - 0.114*B) * 1.704
      Color->A = 0;

      *Color = Color->Saturate();
      }

   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap, and convert it to float type.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, ColorRec* Color)
      {
      //Convert YUV values from 9.7 fixed point format to 0.0 - 255.0, then
      // scale dwon to 0.0 - 1.0
      float Y = fix97_to_float(*(sint*)(Ptr  )) * _255inv;        //Y is always positive
      float U = fix97_to_float(*(sint*)(Ptr+2)) * _255inv;
      float V = fix97_to_float(*(sint*)(Ptr+4)) * _255inv;

      //Compute the RGB colors, range 0.0 - 1.0
      Color->R = 1.15f*U + Y;
      Color->B = 2.03f*V + Y;
      Color->G = 1.704f*(Y - 0.299f*Color->R - 0.114f*Color->B);
      Color->A = 0;

      *Color = Color->Saturate();
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, iColorRec* Color)
      {       
      //Find YUV values (0.0 - 255.0)
      float Y = 0.299f*(float)Color->R + 0.587f*(float)Color->G + 0.114f*(float)Color->B;
      float U = 0.877f*((float)Color->R - Y);
      float V = 0.493f*((float)Color->B - Y);

      //Convert to 9.7 fixed point format
      *(sint*)(Ptr  ) = (sint)float_to_fix97(Y);              //Y is always positive
      *(sint*)(Ptr+2) = (sint)float_to_fix97(U);
      *(sint*)(Ptr+4) = (sint)float_to_fix97(V);
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap, and convert it from float type to
      the desired bitmap format.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, ColorRec* Color)
      {
      //Find YUV values (0.0 - 1.0)
      float Y = 0.299f*Color->R + 0.587f*Color->G + 0.114f*Color->B;
      float U = 0.877f*(Color->R - Y);
      float V = 0.493f*(Color->B - Y);

      //Scale up to 0 - 255, then convert to 9.7 fixed point format
      *(sint*)(Ptr  ) = (word)float_to_fix97(Y * 255.0f);   //Y is always positive
      *(sint*)(Ptr+2) = (sint)float_to_fix97(U * 255.0f);
      *(sint*)(Ptr+4) = (sint)float_to_fix97(V * 255.0f);

      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif