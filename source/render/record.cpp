/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                       Functions                         */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __RECORD_CPP__
#define __RECORD_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"


/*---------------------------------------------------------------------------
   The Recorder Class
  ---------------------------------------------------------------------------*/
class RecorderClass
   {
   /*==== Private Declarations ===============================================*/
   private:

   PointRec* RotateBuffer;
   PointRec* VelocityBuffer;
   dword     FrameCount;
   dword     CurrentFrame;
   dword     RecodedFrames;


   /*==== Public Declarations ================================================*/
   public:

   bool      BufferFull;
   bool      RecMode;

   /*---- Constructor --------------------------------------------------------*/
   RecorderClass(void)
      {
      RotateBuffer   = NULL;
      VelocityBuffer = NULL;
      FrameCount     = 0;
      CurrentFrame   = 0;
      RecodedFrames  = 0;
      BufferFull     = false;
      RecMode        = true;
      }
   
   /*---- Destructor ---------------------------------------------------------*/
   ~RecorderClass(void)
      {
      if (RotateBuffer   != NULL) {free(RotateBuffer);   RotateBuffer = NULL;}
      if (VelocityBuffer != NULL) {free(VelocityBuffer); VelocityBuffer = NULL;}
      }

   /*-------------------------------------------------------------------------
      Setup a new frame recording buffer.
     ------------------------------------------------------------------------*/
   bool Initialize(dword TotalFrameCount)
      {
      if (TotalFrameCount == 0) {return false;}
     
      PointRec* TempPtr;
      dword     BufferSize = TotalFrameCount * sizeof(PointRec);

      //Allocate the arrays.
      TempPtr = (PointRec*)realloc(RotateBuffer, BufferSize);
      if (TempPtr == NULL) {return false;}
      RotateBuffer = TempPtr;

      TempPtr = (PointRec*)realloc(VelocityBuffer, BufferSize);
      if (TempPtr == NULL) {return false;}
      VelocityBuffer = TempPtr;

      FrameCount    = TotalFrameCount;
      CurrentFrame  = 0;
      RecodedFrames = 0;
      BufferFull    = false;
      RecMode       = true;

      return true;
      }
   
   /*-------------------------------------------------------------------------
      Setup a new frame recording buffer.
     ------------------------------------------------------------------------*/
   void ShutDown(void)
      {
      this->~RecorderClass();
      *this = RecorderClass();
      }

   /*-------------------------------------------------------------------------
      Setup recording mode.
     ------------------------------------------------------------------------*/
   bool InitRec(void)
      {
      if ((RotateBuffer == NULL) || (VelocityBuffer == NULL)) {return false;}
      
      RecodedFrames = 0;
      CurrentFrame  = 0; 
      RecMode       = true;

      return true;      
      }

   /*-------------------------------------------------------------------------
      Record a frame.
     ------------------------------------------------------------------------*/
   bool RecFrame(PointRec &Rotation, PointRec &Velocity, bool &SequenceEnd, dword &CurrentFrameOut)
      {
      if ((RotateBuffer == NULL) || (VelocityBuffer == NULL)) {return false;}
      if (BufferFull || !RecMode) {SequenceEnd = true; return true;}
      
      RotateBuffer[CurrentFrame]   = Rotation;
      VelocityBuffer[CurrentFrame] = Velocity;
      CurrentFrame++;
      RecodedFrames++;

      CurrentFrameOut = CurrentFrame;

      SequenceEnd = BufferFull = (CurrentFrame >= FrameCount);
      if (BufferFull) {RecMode = false;}

      return true;      
      }
   
   /*-------------------------------------------------------------------------
      Setup playback mode.
     ------------------------------------------------------------------------*/
   bool InitPlay(void)
      {
      if ((RotateBuffer == NULL) || (VelocityBuffer == NULL)) {return false;}
      
      CurrentFrame  = 0; 
      RecMode       = false;

      return true;      
      }

   /*-------------------------------------------------------------------------
      Plays a frame.
     ------------------------------------------------------------------------*/
   bool PlayFrame(PointRec &Rotation, PointRec &Velocity, bool &SequenceEnd)
      {
      if ((RotateBuffer == NULL) || (VelocityBuffer == NULL)) {return false;}
      if ((CurrentFrame >= RecodedFrames) || RecMode) {SequenceEnd = true; return true;}

      Rotation = RotateBuffer[CurrentFrame];
      Velocity = VelocityBuffer[CurrentFrame];
      CurrentFrame++;

      SequenceEnd = (CurrentFrame >= RecodedFrames);
      return true;      
      }
   };


/*----------------------------------------------------------------------------
  Global declaration of the class.
  ----------------------------------------------------------------------------*/
RecorderClass Recorder;


/*==== End of file ===========================================================*/
#endif
