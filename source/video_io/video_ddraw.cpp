/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                      OpenGL Interface Functions                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __VIDEO_DDRAW_CPP__
#define __VIDEO_DDRAW_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../video_io/video.cpp"
#include "../mem_data/bitmap.cpp"


/*---------------------------------------------------------------------------
  The DDraw class.
  ---------------------------------------------------------------------------*/
class DDrawClass : public VideoClass
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Constructor --------------------------------------------------------*/
   DDrawClass(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~DDrawClass(void) {}

   /*---- Video IO functions -------------------------------------------------*/
   bool Initialize    (void) {return false;}
   bool Restore       (void) {return true;}
   bool SetMode       (int X_Res_Set, int Y_Res_Set, int BitsPerPixel_Set, int RefreshRate_Set, bool FullScreen_Set) {return false;}
   bool ClearFrame    (float R, float G, float B, float A) {return false;}
   bool Refresh       (bool ClrFrame, float R, float G, float B, float A) {return false;}
   bool CaptureFrame  (BitmapRec* Bitmap) {return false;}
   bool Lock          (void) {return false;}
   bool UnLock        (void) {return false;}

   /*==== End of Class =======================================================*/
   };

/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
DDrawClass DDraw;


/*==== End of File ===========================================================*/
#endif
