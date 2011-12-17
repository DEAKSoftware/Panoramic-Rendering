/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                           DDraw Rendering Functions                        */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __RENDER_DDRAW_CPP__
#define __RENDER_DDRAW_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../render/render.cpp"
#include "../video_io/video.cpp"


/*---------------------------------------------------------------------------
  The Renering NULL class.
  ---------------------------------------------------------------------------*/
class RenderDDrawClass : public RenderClass
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Constructor --------------------------------------------------------*/
   RenderDDrawClass(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~RenderDDrawClass(void) {}

   /*-------------------------------------------------------------------------
      Setup the renderer.
     ------------------------------------------------------------------------*/
   bool Initialize(VideoClass* Video, char* ProfCurve, float ProfEquLim, dword NewTabRes) 
      {
      //No NULL pointers please
      if (Video == NULL) {return false;}

      //Ensure that the Video uses the correct interface,
      // and the Video mode is valid.
      if ((Video->CurrentDevice != VIDEO_DDRAW) || !Video->ModeValid) {return false;}

      //Indicate that rendering is allowed
      //RenderValid = true;

      return true;
      }

   /*-------------------------------------------------------------------------
      Shutdown the renderer.
     ------------------------------------------------------------------------*/
   bool ShutDown(void) 
      {
      if (ProfEqu  != NULL) {delete[] ProfEqu;  ProfEqu  = NULL;}
      if (ProfCode != NULL) {delete[] ProfCode; ProfCode = NULL;}

      //Indicate that rendering is not allowed
      RenderValid = false;

      return true;
      }

   /*-------------------------------------------------------------------------
      This function will render the entire world.

      EntityList  : The list of Entities and sub-Entities to render.
      LightList   : The list of Lights to use for shading.
      World       : Access to the world data such as view origin and 
                    orientation, etc.
     ------------------------------------------------------------------------*/
   bool DrawScene(ListRec* EntityList, ListRec* LightList, WorldRec* World) 
      {
      //Ensure that the Video uses the correct interface,
      // and the Video mode is valid.
      if ((Video->CurrentDevice != VIDEO_DDRAW) || !Video->ModeValid) {return false;}

      //If the display area is minimized, don't do any rendering
      if (Video->Minimized) {return true;}

      //Lock the renderer
      if (!Video->Lock()) {return false;}

      //Unlock the renderer
      if (!Video->UnLock()) {return false;}

      return true;
      }



   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global declaration of the Video class.
  ----------------------------------------------------------------------------*/
RenderDDrawClass RenderDDraw;


/*==== End of file ===========================================================*/
#endif
