/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                         Configuration Data                                 */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __CFG_DATA_CPP__
#define __CFG_DATA_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/pointrec.h"


//-- Video Configuration --
struct ConfigVideo
   {
   bool  FullScreen;                         //If set true, then the display is in full screen
   dword X_Res;                              //Resolution in X pixels of current video mode
   dword Y_Res;                              //Resolution in Y pixels of current video mode
   dword BitsPerPixel;                       //Bits per pixel
   dword RefreshRate;
   };

//-- Rendering Configuration --
struct ConfigRender
   {
   PointRec CamApeture;                      //Camera apeture
   char*    ProfEqu;                         //Original profile curve equation
   float    ProfEquLimX;                     //

   bool     ClearFlag;                       //If set true, the frame buffer is cleared with BackgndColor after refresh
   bool     ShadowFlag;                      //If set true, shadows are generated
   bool     RefractFlag;                     //If set true, refractions are rendered
   bool     ReflectFlag;                     //If set true, reflections are rendered
   bool     HotspotFlag;                     //Render in hotspot mode
   bool     AntiAliasFlag;
   bool     PCompFlag;                       //If set true, then projetor viewpoint compesation is enabled
   
   float    POffset;                         //Projector z-offset in world coords
   ColorRec BackgndColor;
   dword    MaxRayDepth;                     //Maximum ray-tracing depth
   dword    Max_LOD;                         //Maximum polygon sub-div recursions
   dword    TabRes;
   float    SubdivTresh;
   dword    AA_Samples;
   float    AA_Jitter;
   float    AA_Treshold;
   float    AdaptDepthTresh;
   dword    CurrentDevice;                   //Specifies the current rendering device
   };

//-- Rendering Configuration --
struct ConfigWorld
   {
   PointRec VOrigin;                         //View origin in the world
   PointRec VVelocity;                       //View velocity
   PointRec VRotation;                       //View initial rotation
   PointRec VOrientation;                    //View orientation
   ColorRec AmbLight;
   };


/*---------------------------------------------------------------------------
  The ConfigRec class.
  ---------------------------------------------------------------------------*/
class ConfigRec
   {
   /*==== Public Declarations ================================================*/
   public:

   ConfigVideo  Video;
   ConfigRender Render;
   ConfigWorld  World;

   /*---- Constructor --------------------------------------------------------*/
   ConfigRec(void)
      {
      //-- Reset the Video config structure --
      this->Video.FullScreen        = false;
      this->Video.X_Res             = 0;
      this->Video.Y_Res             = 0;
      this->Video.BitsPerPixel      = 0;
      this->Video.RefreshRate       = 0;

      //-- Reset the Render config structure --
      this->Render.CamApeture       = 1.0f;
      this->Render.ProfEqu          = NULL;
      this->Render.ProfEquLimX      = 30.0f;
   
      this->Render.ClearFlag        = false;
      this->Render.ShadowFlag       = false;
      this->Render.RefractFlag      = false;
      this->Render.ReflectFlag      = false;
      this->Render.HotspotFlag      = false;
      this->Render.AntiAliasFlag    = false;
      this->Render.PCompFlag        = false;
   
      this->Render.POffset          = -1000000.0f;
      this->Render.BackgndColor     = 0.0f;
      this->Render.MaxRayDepth      = false;
      this->Render.Max_LOD          = 3;
      this->Render.TabRes           = 20;
      this->Render.SubdivTresh      = 0.2f;
      this->Render.CurrentDevice    = 0;
      this->Render.AA_Samples       = 8;
      this->Render.AA_Jitter        = 0.0075f;
      this->Render.AA_Treshold      = 0.01f;
      this->Render.AdaptDepthTresh  = 0.2f;

      //-- Reset the World config structure --
      this->World.VOrigin           = 0.0f;
      this->World.VVelocity         = 0.0f;
      this->World.VRotation         = 0.0f;
      this->World.VOrientation      = 0.0f;
      this->World.AmbLight          = 0.0f;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~ConfigRec(void)
      {
      if (this->Render.ProfEqu != 0) {delete[] this->Render.ProfEqu; this->Render.ProfEqu = NULL;}
      }

   /*-------------------------------------------------------------------------

     -------------------------------------------------------------------------*/


   /*==== End of Class =======================================================*/
   };


/*---------------------------------------------------------------------------
   Global declaration.
  ---------------------------------------------------------------------------*/
ConfigRec Config;

/*==== End of File ===========================================================*/
#endif
