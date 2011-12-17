/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                         Video IO and Data Class                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.
  ---------------------------------------------------------------------------*/
#ifndef __VIDEO_CPP__
#define __VIDEO_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../mem_data/bitmap.cpp"


/*---------------------------------------------------------------------------
  Video flags.
  ---------------------------------------------------------------------------*/
#define VIDEO_NULL   0x00000000
#define VIDEO_DDRAW  0x00000001
#define VIDEO_D3D    0x00000002
#define VIDEO_GLIDE  0x00000003
#define VIDEO_OPENGL 0x00000004


/*---------------------------------------------------------------------------
  The Video class.
  ---------------------------------------------------------------------------*/
class VideoClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*---- Public Data --------------------------------------------------------*/
   bool  ModeValid;                       //If set true, then the video is ready for rendering
   bool  FullScreen;                      //If set true, then the display is in full screen
   bool  Minimized;                       //If set true, then the display window is minimized
   dword X_Res;                           //Resolution in X pixels of current video mode
   dword Y_Res;                           //Resolution in Y pixels of current video mode
   dword BitsPerPixel;                    //Bits per pixel
   dword BytesPerPixel;                   //Number of bytes aligned per pixel
   dword BytesPerLine;                    //Number of bytes in each scanline
   dword RefreshRate;                     //Video refresh rate in Hz
   float XY_Ratio;                        //X/Y Aspect ratio
   float YX_Ratio;                        //Y/x Aspect ratio
   char* DeviceInfo;                      //A zero terminated string, containing device information.
   dword CurrentDevice;                   //Specifies the current device


   /*---- Constructor --------------------------------------------------------*/
   VideoClass(void)
      {
      ModeValid        = false;
      FullScreen       = false;
      Minimized        = false;
      X_Res            = 0;    
      Y_Res            = 0;
      BitsPerPixel     = 0;
      BytesPerPixel    = 0;
      BytesPerLine     = 0;
      RefreshRate      = 0;
      XY_Ratio         = 0.0;
      YX_Ratio         = 0.0;
      DeviceInfo       = NULL;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~VideoClass(void) 
      {
      if (DeviceInfo != NULL) {delete[] DeviceInfo;}
      }


   /*-------------------------------------------------------------------------
     Clears all VideoClass data.
     ------------------------------------------------------------------------*/
   void inline Reset_VData(void)
      {
      ModeValid        = false;
      FullScreen       = false;
      Minimized        = false;
      X_Res            = 0;    
      Y_Res            = 0;
      BitsPerPixel     = 0;
      BytesPerPixel    = 0;
      BytesPerLine     = 0;
      XY_Ratio         = 0.0;
      YX_Ratio         = 0.0;
      if (DeviceInfo != NULL) {delete[] DeviceInfo; DeviceInfo = NULL;}
      }

   /*---- Video IO functions -------------------------------------------------*/
   bool         Interface     (dword VideoDevice, VideoClass* &Video);
   virtual bool Initialize    (void) = NULL;
   virtual bool Restore       (void) = NULL;
   virtual bool SetMode       (int X_Res_Set, int Y_Res_Set, int BitsPerPixel_Set, int RefreshRate_Set, bool FullScreen_Set) = NULL;
   virtual bool ClearFrame    (float R, float G, float B, float A) = NULL;
   virtual bool Refresh       (bool ClrFrame, float R, float G, float B, float A) = NULL;
   virtual bool Lock          (void) = NULL;
   virtual bool UnLock        (void) = NULL;
   virtual bool CaptureFrame  (BitmapRec* Bitmap) = NULL;


   /*==== End of Class =======================================================*/
   };


/*==== End of __VIDEO_CPP__ ==================================================*/
#endif



/*---------------------------------------------------------------------------
   Don't include the Interface code if it's already defined.
  ---------------------------------------------------------------------------*/
#ifndef __VIDEO_CPP__InterfaceCode__
#define __VIDEO_CPP__InterfaceCode__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this code section.
  ---------------------------------------------------------------------------*/
#include "../video_io/video_null.cpp"
#include "../video_io/video_ddraw.cpp"
#include "../video_io/video_opengl.cpp"


/*----------------------------------------------------------------------------
   This function selects a new video interface. Returns true on success.

   VideoDevice   : Select the new video interface.
   Video         : Reference to the current Video class pointer. The new Video
                   interface class will be returned here.

   Calling protocol:

   Video->Interface(VideoDevice, Video);
  ----------------------------------------------------------------------------*/
bool VideoClass::Interface(dword VideoDevice, VideoClass* &Video)
   {
   //Do nothing if the new device is the same
   if (CurrentDevice == VideoDevice) {return true;}

   //Restore the current video
   if (!Restore()) {return false;}
   
   //Setup the new video interface
   switch (VideoDevice)
      {
      case VIDEO_NULL   : Video = &VideoNULL; break;
      case VIDEO_DDRAW  : Video = &DDraw;     break;
      case VIDEO_OPENGL : Video = &OpenGL;    break;
      default           : Video = &VideoNULL; VideoDevice = VIDEO_NULL; break;
      }

   //---- NOTE: From this point onwards, Video*& must be used to 
   //     access the new interface settings! ----

   //Intialize the new video
   if (!Video->Initialize()) {return false;}
   
   //Update the new device flag
   Video->CurrentDevice = VideoDevice;

   return true;
   }

/*----------------------------------------------------------------------------
  Global declaration of the Video class.
  ----------------------------------------------------------------------------*/
VideoClass* Video = &VideoNULL;


/*==== End of File ===========================================================*/
#endif
