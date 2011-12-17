/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                            Rendering Functions                             */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __RENDER_CPP__
#define __RENDER_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../mem_data/world.cpp"
#include "../video_io/video.cpp"


/*---------------------------------------------------------------------------
  Render flags.
  ---------------------------------------------------------------------------*/
#define RENDER_NULL     0x00000000
#define RENDER_DDRAW    0x00000001
#define RENDER_D3D      0x00000002
#define RENDER_GLIDE    0x00000003
#define RENDER_OPENGL   0x00000004
#define RENDER_RAY      0x00000005


/*---------------------------------------------------------------------------
  The Render class.
  ---------------------------------------------------------------------------*/
class RenderClass
   {
   /*==== Public Declarations ================================================*/
   public:

   bool     RenderValid;                     //If set true, then the class is ready for rendering
   PointRec CamApeture;                      //Camera apeture X, Y, and Z = field of view
   char*    ProfEqu;                         //Original profile curve equation
   byte*    ProfCode;                        //Compiled version of the profile curve equation
   float    ProfEquLimR;
   
   bool     ClearFlag;                       //If set true, the frame buffer is cleared with BackgndColor after refresh
   bool     ShadowFlag;                      //If set true, shadows are generated
   bool     RefractFlag;                     //If set true, refractions are rendered
   bool     ReflectFlag;                     //If set true, reflections are rendered
   bool     HotspotFlag;                     //Render in hotspot mode
   bool     AntiAliasFlag;
   bool     PCompFlag;                       //If set true, then projetor viewpoint compesation is enabled
   bool     RenderDone;                      //Flag to indicate that the rendering is done
   bool     WireFrame;
   
   float    POffset;                         //Projector z-offset in world coords
   ColorRec BackgndColor;                    //The backgroud color for clearing
   dword    MaxRayDepth;                     //Maximum ray-tracing depth
   float    AdaptDepthTresh;
   dword    Max_LOD;                         //Maximum polygon sub-div recursions
   dword    TabRes;
   float    SubdivTresh;
   dword    AA_Samples;
   float    AA_Jitter;
   float    AA_Treshold;
   dword    CurrentDevice;                   //Specifies the current device

   /*---- Constructor --------------------------------------------------------*/
   RenderClass(void) 
      {
      RenderValid       = false;

      CamApeture        = 1.0f;
      ProfEqu           = NULL;
      ProfCode          = NULL;
      ProfEquLimR       = 30.0f;

      ClearFlag         = false;
      ShadowFlag        = false;
      RefractFlag       = false;
      ReflectFlag       = false;
      HotspotFlag       = false;
      AntiAliasFlag     = false;
      PCompFlag         = false;
      RenderDone        = false;
      WireFrame         = false;
   
      POffset           = -1000000.0f;
      BackgndColor      = 0.0f;
      MaxRayDepth       = 4;
      AdaptDepthTresh   = 0.2f;
      Max_LOD           = 3;
      TabRes            = 20;
      SubdivTresh       = 0.2f;
      AA_Samples        = 8;
      AA_Jitter         = 0.0075f;
      AA_Treshold       = 0.01f;
      CurrentDevice     = RENDER_NULL;
      }
   
   /*---- Destructor ---------------------------------------------------------*/
   ~RenderClass(void) 
      {
      if (ProfEqu  != NULL) {delete[] ProfEqu;  ProfEqu = NULL;}
      if (ProfCode != NULL) {delete[] ProfCode; ProfCode = NULL;}
      }


   /*---- Render IO functions ------------------------------------------------*/
   bool         Interface (dword RenderDevice, RenderClass* &Render, VideoClass* &Video, int X_Res, int Y_Res, dword BitsPerPixel, dword RefreshRate, bool FullScreen, char* ProfCurve, float ProfEquLim, PointRec* CamApeture, dword NewTabRes);
   virtual bool Initialize(VideoClass* Video, char* ProfCurve, float ProfEquLim, dword NewTabRes) = NULL;
   virtual bool ShutDown  (void) = NULL;
   virtual bool DrawScene (ListRec* EntityList, ListRec* LightList, WorldRec* World) = NULL;

   /*==== End of Class =======================================================*/
   };


/*==== End of __RENDER_CPP__ =================================================*/
#endif



/*---------------------------------------------------------------------------
   Don't include the Interface code if it's already defined.
  ---------------------------------------------------------------------------*/
#ifndef __RENDER_CPP__InterfaceCode__
#define __RENDER_CPP__InterfaceCode__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this code section.
  ---------------------------------------------------------------------------*/
#include "../render/render_null.cpp"
#include "../render/render_ddraw.cpp"
#include "../render/render_opengl.cpp"
#include "../render/render_ray.cpp"
#include "../video_io/video.cpp"


/*----------------------------------------------------------------------------
   This function selects a new rendering interface. It also initializes the
   suitable video interface. Returns true on success.

   RenderDevice   : Select the new Render interface.
   Render         : Reference to the current Render class pointer. The new Render
                    class will be returned here.
   Video          : Reference to the Video interface class pointer. A new Video 
                    interface class will be returned if necessary.
   X_Res, Y_Res   : Force to a new screen resolution. (See details below).
   BitsPerPixel   : Colour depth of the new display. (See details below).
   RefreshRate    : The screen refresh rate in Hz.
   FullScreen     : If set true, the Video is initialized in full screen mode.
                    If the video is not forced into a new screen resolution, 
                    this flag is ignored.
   ProfCurve      : Pointer to the profile curve equation string for the 
                    panoramic display. If a standard view plane is desired,
                    ProfCurve can be set to NULL.
   ProfEquLim     : Profile curve X (or r) limit.
   CamApeture     : Camera apeture size and filed of view

   The Video device will be forced to change mode if the X_Res, Y_Res and
   BitsPerPixel is not 0. Otherwise, this function will preserve the old Video 
   mode settings after the Video->Interface() call. If there was no old Video 
   mode set, and no mode change is forced, the new Video mode must be set 
   externally, and the rendering interface must be also initialized externally.
   In this case, before any rendering is performed, the functions Video->SetMode() 
   and Render->Initialize() must be called first in that order!
         
   Calling protocol, no Video mode forcing:
      Render->Interface(RenderDevice, Render, Video, 0, 0, 0, false, Equation, CamApeture);

   Calling protocol, with Video mode forcing:
      Render->Interface(RenderDevice, Render, Video, 640, 480, 32, true, Equation, CamApeture);
  ----------------------------------------------------------------------------*/
bool RenderClass::Interface(dword RenderDevice, RenderClass* &Render, VideoClass* &Video, int X_Res, int Y_Res, dword BitsPerPixel, dword RefreshRate, bool FullScreen, char* ProfCurve, float ProfEquLim, PointRec* CamApeture, dword NewTabRes)
   {
   if (CamApeture == NULL) {return true;}

   //Do nothing if the new device is the same
   //if (CurrentDevice == RenderDevice) {return true;}

   //Shutdown the current renderer
   if (!ShutDown()) {return false;}


   //Setup the new render interface
   dword VideoDevice;
   switch (RenderDevice)
      {
      //-- NULL renderer --
      case RENDER_NULL: 
         {
         Render      = &RenderNULL;
         VideoDevice = Video->CurrentDevice;
         break;
         }

      //-- DDraw renderer --
      case RENDER_DDRAW: 
         {
         Render      = &RenderDDraw; 
         VideoDevice = VIDEO_DDRAW;
         break;
         }

      //-- OpenGL renderer --
      case RENDER_OPENGL: 
         {
         Render      = &RenderOpenGL; 
         VideoDevice = VIDEO_OPENGL;
         break;
         }

      //-- Ray renderer --
      case RENDER_RAY: 
         {
         //FullScreen  = false;
         Render      = &RenderRay; 
         VideoDevice = VIDEO_OPENGL;
         break;
         }

      //-- NULL renderer --
      default: 
         {
         Render       = &RenderNULL; 
         RenderDevice = RENDER_NULL;
         VideoDevice  = Video->CurrentDevice;
         break;
         }
      }

   //---- NOTE: From this point onwards, Render*& must be used to 
   //     access the new Render interface settings! ----
   Render->CamApeture = *CamApeture;
   if (Render->CamApeture.X == 0.0f) {Render->CamApeture.X = 1.0f;}
   if (Render->CamApeture.Y == 0.0f) {Render->CamApeture.Y = 1.0f;}
   if (Render->CamApeture.Z == 0.0f) {Render->CamApeture.Z = 1.0f;}


   //The ForceMode flag indicates that the Video mode will be forced to change.
   // If X_Res, Y_Res and BitsPerPixel is 0, no forcing is required, and 
   // ForceMode will be used to flag the preservation of the old Video mode
   // (if it's already in a valid mode).
   bool ForceMode = (X_Res != 0) && (Y_Res != 0) && (BitsPerPixel != 0);
   if (!ForceMode && Video->ModeValid)
      {
      //No forcing was necessay, but we have a valid video mode, therefore 
      // the current mode will be preserved.
      ForceMode    = Video->ModeValid;
      X_Res        = Video->X_Res;
      Y_Res        = Video->Y_Res;
      BitsPerPixel = Video->BitsPerPixel;
      RefreshRate  = Video->RefreshRate;
      FullScreen   = Video->FullScreen;
      }

   //Switch to the suitable Video interface
   if (!Video->Interface(VideoDevice, Video)) {return false;}
      
   //If necessary, set the Video mode, and at the same time 
   // initialize the new Render device.
   if (ForceMode)
      {
      if (!Video->SetMode(X_Res, Y_Res, BitsPerPixel, RefreshRate, FullScreen)) {return false;}
      if (!Render->Initialize(Video, ProfCurve, ProfEquLim, NewTabRes)) {return false;}
      }

   //Update the new device flag
   Render->CurrentDevice = RenderDevice;

   return true;
   }


/*----------------------------------------------------------------------------
  Global declaration of the Render class.
  ----------------------------------------------------------------------------*/
RenderClass* Render = &RenderNULL;



/*==== End of file ===========================================================*/
#endif