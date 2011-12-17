/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                            NULL Rendering Functions                        */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __RENDER_NULL_CPP__
#define __RENDER_NULL_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../render/render.cpp"
#include "../video_io/video.cpp"


/*---------------------------------------------------------------------------
  The Renering NULL class.
  ---------------------------------------------------------------------------*/
class RenderNULLClass : public RenderClass
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Constructor --------------------------------------------------------*/
   RenderNULLClass(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~RenderNULLClass(void) {}

   /*---- Dummy functions ----------------------------------------------------*/
   bool Initialize(VideoClass* Video, char* ProfCurve, float ProfEquLim, dword NewTabRes) {return false;}
   bool ShutDown(void) {return true;}
   bool DrawScene(ListRec* EntityList, ListRec* LightList, WorldRec* World) {return false;}


   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global declaration of the Video class.
  ----------------------------------------------------------------------------*/
RenderNULLClass RenderNULL;


/*==== End of file ===========================================================*/
#endif
