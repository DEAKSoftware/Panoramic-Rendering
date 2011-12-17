/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                               Keyboard Interface                           */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __KEYBOARD_CPP__
#define __KEYBOARD_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*---------------------------------------------------------------------------
   Definitions.
  ---------------------------------------------------------------------------*/
#define KEYB_BUFLEN     0x00000001

#define KEYB_NULL       0x00000000
#define KEYB_KEYMASK    0x000000FF
#define KEYB_F1         0x00000001
#define KEYB_F2         0x00000002
#define KEYB_F3         0x00000003
#define KEYB_F4         0x00000004
#define KEYB_F5         0x00000005
#define KEYB_F6         0x00000006
#define KEYB_F7         0x00000007
#define KEYB_F8         0x00000008
#define KEYB_F9         0x00000009
#define KEYB_F10        0x0000000A
#define KEYB_F11        0x0000000B
#define KEYB_F12        0x0000000C
#define KEYB_LEFT       0x0000000D
#define KEYB_RIGHT      0x0000000E
#define KEYB_UP         0x0000000F
#define KEYB_DOWN       0x00000010
#define KEYB_PGUP       0x00000011
#define KEYB_PGDN       0x00000012
#define KEYB_INS        0x00000013
#define KEYB_HOME       0x00000014
#define KEYB_END        0x00000015


/*---------------------------------------------------------------------------
  The Keyboard IO class.
  ---------------------------------------------------------------------------*/
class Keyboard_Class
   {
   /*==== Public Declarations ================================================*/
   public:

   byte  Buffer[KEYB_BUFLEN];
   byte* LastChar;

   /*---- Constructor --------------------------------------------------------*/
   Keyboard_Class(void)
      {
      LastChar = Buffer;
      for (int I = 0; I < KEYB_BUFLEN; I++) {Buffer[I] = 0;}
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~Keyboard_Class(void) {}

   
   /*------------------------------------------------------------------------- 
      Initialise the keyboard parameters
     -------------------------------------------------------------------------*/ 
   void Initialize(void)
      {}

   /*-------------------------------------------------------------------------
     The keyboard handler.
     -------------------------------------------------------------------------*/
   void __cdecl Handler(byte Char, int X, int Y)
      {
      if (LastChar >= (Buffer + KEYB_BUFLEN)) {return;}
      *LastChar = Char;
      LastChar++;
      }

   /*-------------------------------------------------------------------------
     The special function keyboard handler.
     -------------------------------------------------------------------------*/
   void __cdecl SpecialHandler(int Key, int X, int Y)
      {
      if (LastChar >= (Buffer + KEYB_BUFLEN)) {return;}

      switch (Key)
         {
         case GLUT_KEY_F1        : *LastChar = KEYB_F1;  break;
         case GLUT_KEY_F2        : *LastChar = KEYB_F2;  break;
         case GLUT_KEY_F3        : *LastChar = KEYB_F3;  break;
         case GLUT_KEY_F4        : *LastChar = KEYB_F4;  break;
         case GLUT_KEY_F5        : *LastChar = KEYB_F5;  break;
         case GLUT_KEY_F6        : *LastChar = KEYB_F6;  break;
         case GLUT_KEY_F7        : *LastChar = KEYB_F7;  break;
         case GLUT_KEY_F8        : *LastChar = KEYB_F8;  break;
         case GLUT_KEY_F9        : *LastChar = KEYB_F9;  break;
         case GLUT_KEY_F10       : *LastChar = KEYB_F10; break;
         case GLUT_KEY_F11       : *LastChar = KEYB_F11; break;
         case GLUT_KEY_F12       : *LastChar = KEYB_F12; break;
         case GLUT_KEY_LEFT      : *LastChar = KEYB_LEFT; break;
         case GLUT_KEY_RIGHT     : *LastChar = KEYB_RIGHT; break;
         case GLUT_KEY_UP        : *LastChar = KEYB_UP; break;
         case GLUT_KEY_DOWN      : *LastChar = KEYB_DOWN; break;
         case GLUT_KEY_PAGE_UP   : *LastChar = KEYB_PGUP; break;
         case GLUT_KEY_PAGE_DOWN : *LastChar = KEYB_PGDN; break;
         case GLUT_KEY_HOME      : *LastChar = KEYB_HOME; break;
         case GLUT_KEY_END       : *LastChar = KEYB_END; break;
         case GLUT_KEY_INSERT    : *LastChar = KEYB_INS; break;
         default : return;
         }

      //*LastChar = Char;
      LastChar++;
      }

   /*-------------------------------------------------------------------------
     Reads a character from the buffer.
     -------------------------------------------------------------------------*/
   inline byte GetChar(void)
      {
      if (LastChar == Buffer) {return 0;}
      LastChar--;
      return *LastChar;
      }

   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
Keyboard_Class Keyboard;

void __cdecl KeyboardHandler(byte Char, int X, int Y) {Keyboard.Handler(Char, X, Y);}
void __cdecl KeyboardSpecialHandler(int Key, int X, int Y) {Keyboard.SpecialHandler(Key, X, Y);}


/*==== End of file ===========================================================*/
#endif
