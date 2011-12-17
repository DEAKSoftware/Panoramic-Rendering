/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*         System Timer Functions (at this stage Windows 9x and NT only)      */
/*============================================================================*/

/*----------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ----------------------------------------------------------------------------*/
#ifndef __SYSTIMER_CPP__
#define __SYSTIMER_CPP__


/*----------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ----------------------------------------------------------------------------*/
#include "../_common/std_inc.h"



/*----------------------------------------------------------------------------
   System timer class.
  ----------------------------------------------------------------------------*/
class SystemTimerClass
   {
   /*==== Private Declarations ===============================================*/
   private:

   bool  TS_Valid;
   qword LastCount;
   qword Frequency;
   float FreqInv;

   
   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   SystemTimerClass(void) 
      {
      //==== Win32 specific ====
      #if defined (WIN32) || defined (WIN32_NT)
         if ((QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency) != 0) &&
             (QueryPerformanceCounter((LARGE_INTEGER*)&LastCount) != 0))
            {
            FreqInv  = 1.0f / (float)Frequency;
            TS_Valid = true;
            }
         else 
            {
            TS_Valid  = false;
            LastCount = 0;
            Frequency = 0;
            FreqInv   = 0.0f;
            }

      //==== Other OS ====
      #else
         TS_Valid  = false;
         LastCount = 0;
         Frequency = 0;
         FreqInv   = 0.0f;

      #endif
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~SystemTimerClass(void) {}

   /*-------------------------------------------------------------------------
      Returns the current time stamp.
     -------------------------------------------------------------------------*/
   inline qword ReadTS(void)
      {
      if (!TS_Valid) {return 0;}

      qword Count;
      
      //==== Win32 specific ====
      #if defined (WIN32) || defined (WIN32_NT)
         if (QueryPerformanceCounter((LARGE_INTEGER*)&Count) == 0) {return 0;}

      //==== Other OS ====
      #else
         Count = 0;
      #endif
      
      return Count;
      }
  
   /*-------------------------------------------------------------------------
      Sets up the delay measurement by initializing the LastCount register.
     -------------------------------------------------------------------------*/
   inline void TS_DiffStart(void)
      {
      if (!TS_Valid) {return;}
      LastCount = ReadTS(); 
      }

   /*-------------------------------------------------------------------------
      Returns the delay in time stamp counts.
     -------------------------------------------------------------------------*/
   inline qword ReadTS_Diff(void)
      {
      if (!TS_Valid) {return 0;}

      qword Count = ReadTS(); 
      qword Diff  = Count - LastCount;
      LastCount   = Count;
      
      return Diff;
      }
  
   /*-------------------------------------------------------------------------
      Returns the delay in seconds.
     -------------------------------------------------------------------------*/
   inline float ReadTS_DiffSec(void)
      {
      if (!TS_Valid) {return 0.0f;}

      qword Diff = ReadTS_Diff();

      return (float)Diff * FreqInv;
      }
  
   
   /*==== End Class ==========================================================*/
   };



/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
SystemTimerClass SystemTimer;

/*==== End of file ===========================================================*/
#endif