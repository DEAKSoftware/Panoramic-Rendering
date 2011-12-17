/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                  Functions for Color Table Pixel IO                        */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_C_CPP__
#define __PIXEL_C_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"



/*---------------------------------------------------------------------------
  The Pixel_C_Class class (inherits the PixelClass class).
  ---------------------------------------------------------------------------*/
class Pixel_C_Class : public PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Pixel_C_Class(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~Pixel_C_Class(void) {}


   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, iColorRec* Color)
      {
      if (ColorTablePtr == NULL) {return;}

      byte* ColorTableOffs = ColorTablePtr + (dword)*Ptr * 3;
      *Color = iColorRec((int)*(ColorTableOffs  ), 
                         (int)*(ColorTableOffs+1), 
                         (int)*(ColorTableOffs+2), 
                         (int)*Ptr);            //Return table index in Alpha
      }

   /*-------------------------------------------------------------------------
      Reads the pixel colors from the bitmap, and convert it to float type.
     ------------------------------------------------------------------------*/
   inline void Read(byte* Ptr, ColorRec* Color)
      {
      if (ColorTablePtr == NULL) {return;}

      byte* ColorTableOffs = ColorTablePtr + (dword)*Ptr * 3;
      *Color =  ColorRec((float)*(ColorTableOffs  ), 
                         (float)*(ColorTableOffs+1), 
                         (float)*(ColorTableOffs+2), 0) * _255inv;
      Color->A = (float)*Ptr;                   //Return table index in Alpha
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, iColorRec* Color)
      {
      if (ColorTablePtr == NULL) {return;}

      /*
      //Find the luminance of this color
      float Y  = 0.299f*(float)Color->R + 0.587f*(float)Color->G + 0.114f*(float)Color->B;
      float dY = float_MAX;

      //Find the color table entry with the closest luminance value
      dword EntryMatch = 0;
      for (dword Offs = 0; Offs < 256; Offs+=3)
         {
         float NewY  = 0.299f*(float)ColorTablePtr[Offs  ];
         NewY       += 0.587f*(float)ColorTablePtr[Offs+1];
         NewY       += 0.114f*(float)ColorTablePtr[Offs+2];
         float NewdY = fabs(Y - NewY);
         if (NewdY < dY) {EntryMatch = Offs / 3; dY = NewdY;}
         }
    
      *Ptr = (byte)EntryMatch;
      /**/

      *Ptr = (byte)Color->A;
      }

   /*-------------------------------------------------------------------------
      Writes the pixel colors to the bitmap, and convert it from float type to
      the desired bitmap format.
     ------------------------------------------------------------------------*/
   inline void Write(byte* Ptr, ColorRec* Color)
      {
      if (ColorTablePtr == NULL) {return;}

      /*
      //Find the luminance of this color
      float Y  = (0.299f*Color->R + 0.587f*Color->G + 0.114f*Color->B) * 255.0f;
      float dY = float_MAX;

      //Find the color table entry with the closest luminance value
      dword EntryMatch = 0;
      for (dword Offs = 0; Offs < 256; Offs+=3)
         {
         float NewY  = 0.299f*(float)ColorTablePtr[Offs  ];
         NewY       += 0.587f*(float)ColorTablePtr[Offs+1];
         NewY       += 0.114f*(float)ColorTablePtr[Offs+2];
         float NewdY = fabs(Y - NewY);
         if (NewdY < dY) {EntryMatch = Offs / 3; dY = NewdY;}
         }
    
      *Ptr = (byte)EntryMatch;
      /**/

      *Ptr = (byte)(Color->A * 255.0f);
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif