/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                              Mouse Interface                               */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __MOUSE_CPP__
#define __MOUSE_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"


/*---------------------------------------------------------------------------
   Defs.
  ---------------------------------------------------------------------------*/
#define MOUSE_MAX_BUTTONS  0x00000004

#define MOUSE_NULL         0x00000000

#define MOUSE_BUTTON_MASK  0x00000003
#define MOUSE_BUTTON_LEFT  0x00000000
#define MOUSE_BUTTON_MID   0x00000001
#define MOUSE_BUTTON_RIGHT 0x00000002
#define MOUSE_BUTTON_ROLL  0x00000003

#define MOUSE_COORD_MASK   0x00000030
#define MOUSE_COORD        0x00000010
#define MOUSE_REL_COORD    0x00000020


/*---------------------------------------------------------------------------
  The Mouse IO class.
  ---------------------------------------------------------------------------*/
class Mouse_Class
   {
   /*==== Pirvate Declarations ===============================================*/
   private:

   bool      FirstCall;                   //If true the mouse handlers recieve the firts call
   iPointRec Coord;                       //Current mouse screen coordinates
   iPointRec RelCoord;                    //Current difference mouse coordinates
   bool      ButtonFlags[MOUSE_MAX_BUTTONS]; //Button status flags


   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   Mouse_Class(void)
      {
      FirstCall   = true;
      Coord       = 0;
      RelCoord    = 0;
      for (int I = 0; I < MOUSE_MAX_BUTTONS; I++) {ButtonFlags[I] = false;}
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~Mouse_Class(void) {}

   
   /*------------------------------------------------------------------------- 
      Initialise the mouse parameters
     -------------------------------------------------------------------------*/ 
   void Initialize(void)
      {}

   /*------------------------------------------------------------------------- 
     Mouse handler.
     -------------------------------------------------------------------------*/ 
   void __cdecl Handler(int Button, int State, int X, int Y)
      {
      MotionHandler(X, Y);

      ButtonFlags[MOUSE_BUTTON_LEFT]  = ((Button == GLUT_LEFT_BUTTON)   && (State == GLUT_DOWN));
      ButtonFlags[MOUSE_BUTTON_MID]   = ((Button == GLUT_MIDDLE_BUTTON) && (State == GLUT_DOWN));
      ButtonFlags[MOUSE_BUTTON_RIGHT] = ((Button == GLUT_RIGHT_BUTTON)  && (State == GLUT_DOWN));
      }

   /*------------------------------------------------------------------------- 
      Mouse motion handler.
     -------------------------------------------------------------------------*/ 
   void __cdecl MotionHandler(int X, int Y)
      {
      //Determine the amount the mouse moved
      RelCoord = iPointRec(X, Y, 0, 0) - Coord;

      //Update the mouse position.
      Coord.X = X;
      Coord.Y = Y;

      //After the first call, keep the initial relative coordinate as 0. This 
      // prevents a sudden RelCoord jump, due to inital Coord = 0 and 
      // new [X, Y] != 0.
      if (FirstCall) {RelCoord = 0; FirstCall = false;}
      }

   /*------------------------------------------------------------------------- 
      Returns the button status, where Button can be MOUSE_BUTTON_LEFT, 
      MOUSE_BUTTON_MID, or MOUSE_BUTTON_RIGHT. Returns true if the correspoding
      button was pressed.
     -------------------------------------------------------------------------*/ 
   inline bool GetButtonStatus(dword ButtonType)
      {
      return ButtonFlags[ButtonType & MOUSE_BUTTON_MASK];
      }

   /*------------------------------------------------------------------------- 
      Returns the current mouse postion or relative postiton. CoordType can be
      MOUSE_COORD or MOUSE_REL_COORD (for anyting else 0 is returned).
     -------------------------------------------------------------------------*/ 
   inline iPointRec GetCoord(dword CoordType)
      {
      if ((CoordType & MOUSE_COORD_MASK) == MOUSE_COORD)     {return Coord;}
      if ((CoordType & MOUSE_COORD_MASK) == MOUSE_REL_COORD) {return RelCoord;}
      return 0;
      }



   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
Mouse_Class Mouse;

void __cdecl MouseHandler(int Button, int State, int X, int Y) {Mouse.Handler(Button, State, X, Y);}
void __cdecl MouseMotionHandler(int X, int Y) {Mouse.MotionHandler(X, Y);}



/*==== End of file ===========================================================*/
#endif
