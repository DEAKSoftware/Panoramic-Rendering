/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                      Functions for NULL Pixel IO                           */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PIXEL_NULL_CPP__
#define __PIXEL_NULL_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"



/*---------------------------------------------------------------------------
  The Pixel_NULL_Class class (inherits the PixelClass class).
  ---------------------------------------------------------------------------*/
class Pixel_NULL_Class : public PixelClass
   {

   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Pixel_NULL_Class(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~Pixel_NULL_Class(void) {}


   /*-------------------------------------------------------------------------
      Dummy pixel IO functions.
     ------------------------------------------------------------------------*/
   inline void Read (byte* Ptr, iColorRec* Color) {*Color = 0;}
   inline void Read (byte* Ptr, ColorRec*  Color) {*Color = 0.0f;}
   inline void Write(byte* Ptr, iColorRec* Color) {}
   inline void Write(byte* Ptr, ColorRec*  Color) {}

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif