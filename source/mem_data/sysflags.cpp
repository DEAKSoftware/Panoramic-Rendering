/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                         System Data Functions                              */
/*============================================================================*/

/*----------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ----------------------------------------------------------------------------*/
#ifndef __SYSFLAGS_CPP__
#define __SYSFLAGS_CPP__


/*----------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ----------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*----------------------------------------------------------------------------
   System and execution control class.
  ----------------------------------------------------------------------------*/
class SystemFlagsClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*-------------------------------------------------------------------------
      System and execution control data.  
     -------------------------------------------------------------------------*/
   bool   ShutDown;              //Flag to indicate for shutting down
   int    ArgCount;              //Command line argument data
   char** Argv;
   dword  Frame;                 //Frame counter
   dword  AccumPoints;           //Accumulated transformed points
   dword  AccumPolys;            //Accumulated polygon count
   float  AccumFPS;              //Accumulated frames per secod

   /*---- Constructor --------------------------------------------------------*/
   SystemFlagsClass(void) 
      {
      ShutDown    = false;
      ArgCount    = 0;
      Argv        = NULL;
      Frame       = 0;
      AccumPoints = 0;
      AccumPolys  = 0;
      AccumFPS    = 0.0f;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~SystemFlagsClass(void) {}

   /*==== End Class ==========================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
SystemFlagsClass SystemFlags;

/*==== End of file ===========================================================*/
#endif