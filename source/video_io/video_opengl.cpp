/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                      OpenGL Interface Functions                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __VIDEO_OPENGL_CPP__
#define __VIDEO_OPENGL_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_str.cpp"
#include "../mem_data/sysflags.cpp"
#include "../mem_data/bitmap.cpp"
#include "../video_io/video.cpp"
#include "../ctrl_io/keyboard.cpp"
#include "../ctrl_io/mouse.cpp"


/*----------------------------------------------------------------------------
  Some pre-declared external functions.
  ----------------------------------------------------------------------------*/
void __cdecl MainLoop(void);
void __cdecl Reshape(int, int);
void __cdecl Visibility(int);
void __cdecl Entry(int);



/*---------------------------------------------------------------------------
  The OpenGL class.
  ---------------------------------------------------------------------------*/
class OpenGLClass : public VideoClass
   {
   /*==== Private Declarations ===============================================*/
   private:

   /*---- Private Data -------------------------------------------------------*/
   int          GLUT_WinHandle;                       //Main GLUT window handle
   bool         GLUT_Init;                            //This flag indicates that glutInit() was called once before
   bool         GLUT_GameMode;                        //If set, the OpenGL device uses the game interface

   /*-------------------------------------------------------------------------
     This function is based on the OpenGL function glGetString(). It will return
     an allocated version of the string obtained by glGetString(). Returns NULL
     on fail.
     ------------------------------------------------------------------------*/
   char* glGetAllocString(GLenum StringName)
      {
      char* StaticString = (char*)glGetString(StringName); //Get the pointer of the static string.
      if (StaticString == NULL) {return NULL;}        //Exit if NULL

      char* AllocatedStr = NULL;                      //Idicate empty pointer
      str_replace(AllocatedStr, StaticString);        //Duplicate the static string
      return AllocatedStr;                            //Return it's pointer
      }
   

   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Constructor --------------------------------------------------------*/
   OpenGLClass(void) 
      {
      GLUT_WinHandle = 0;
      GLUT_Init      = false;
      GLUT_GameMode  = false;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~OpenGLClass(void) {}


   /*-------------------------------------------------------------------------
     Initialize all the OpenGL specific stuff. Returns true if the initialisation 
     was successful.
     ------------------------------------------------------------------------*/
   bool Initialize(void)
      {
      //-- Restore the OpenGL if a video mode is already set up --
      if (ModeValid) 
         {
         if (!Restore()) {return false;}
         }
      
      //-- Call glutInit( ) only once --
      if (!GLUT_Init)
         {
         glutInit(&SystemFlags.ArgCount, SystemFlags.Argv); //Initialise GLUT with command line arguments
         GLUT_Init = true;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
     Restores the OpenGL device.
     ------------------------------------------------------------------------*/
   bool Restore(void)
      {
      if (!ModeValid) {return true;}
      
      if (GLUT_GameMode) {glutLeaveGameMode();}
      else {glutDestroyWindow(GLUT_WinHandle);}

   
      //-- Reset data --
      Reset_VData();
      GLUT_WinHandle = 0;
      GLUT_GameMode  = false;

      return true;
      }

   /*-------------------------------------------------------------------------
     Attempts to set a OpenGL video mode. Returns true if video mode was set.

      X_Res_Set, Y_Res_Set : X and Y resolution of the screen.
      BitsPerPixel_Set     : The desired bits per pixel.
      FullScreen_Set       : Setup the renderer in full screen.
     ------------------------------------------------------------------------*/
   bool SetMode(int X_Res_Set, int Y_Res_Set, int BitsPerPixel_Set, int RefreshRate_Set, bool FullScreen_Set)
      {
      if ((X_Res_Set == 0) || (Y_Res_Set == 0)) {return false;}
      
      //-- If the current video is valid, restore the video if necessary. --
      if (ModeValid) 
         {
         //If paramaters have not changed, return true
         if ((X_Res == (dword)X_Res_Set) && (Y_Res == (dword)Y_Res_Set) && 
             (BitsPerPixel == (dword)BitsPerPixel_Set) && 
             (FullScreen == FullScreen_Set)) {return true;}

         //Parameters are different, restore video mode
         if (!Restore()) {return false;}
         }

      //-- Setup the GL display flags --
      int GLUT_Flags = GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE;
      switch (BitsPerPixel_Set)
         {
         case 16 : GLUT_Flags |= GLUT_RGB;  break;
         case 24 : GLUT_Flags |= GLUT_RGB;  break;
         case 32 : GLUT_Flags |= GLUT_RGBA; break;
         default : GLUT_Flags |= GLUT_RGBA; BitsPerPixel_Set = 32; break;
         }
   
      //-- Initialize the GLUT interface --
      glutInitDisplayMode(GLUT_Flags);             //Set the display mode to true color, with a double buffer  
      glutInitWindowPosition(0, 0);                //Set the window postion

      if (FullScreen_Set)                          //Run in full screen mode if necessary
         {
         char ModeString[0x100]; *ModeString = 0;
         str_compose(ModeString, "%dx%d:%d@%d", X_Res_Set, Y_Res_Set, BitsPerPixel_Set, RefreshRate_Set);
         glutGameModeString(ModeString);
         
         //Determine if game mode is possible
         if (glutGameModeGet((GLenum)GLUT_GAME_MODE_POSSIBLE) != 0) 
            {
            glutEnterGameMode();
            GLUT_GameMode = true;
            }
         else
            {
            printf("WARNING: OpenGLClass::SetMode( ): Unable to set game mode.\n");
            GLUT_GameMode = false;
            }
         }
      
      //Use the standard GLUT setup if game mode failed or setup rendering in
      // windowed mode.
      if (!GLUT_GameMode)
         {
         glutInitWindowSize(X_Res_Set, Y_Res_Set); //Set the window size as specified
         GLUT_WinHandle = glutCreateWindow(Title); //Create the actual GLUT window  
         if (FullScreen_Set) {glutFullScreen();}
         }

      glutDisplayFunc((void(*)(void))MainLoop);    //Define the main looping function 
      glutReshapeFunc((void(*)(int, int))Reshape); //The reshape function
      glutVisibilityFunc((void(*)(int))Visibility);
      glutEntryFunc((void(*)(int))Entry);

      
      //---- GLUT user IO related setup ----
      glutKeyboardFunc((void(*)(byte, int, int))KeyboardHandler);
      glutSpecialFunc((void(*)(int, int, int))KeyboardSpecialHandler);
      glutMouseFunc((void(*)(int, int, int, int))MouseHandler);
      glutMotionFunc((void(*)(int, int))MouseMotionHandler);
      glutPassiveMotionFunc((void(*)(int, int))MouseMotionHandler);
      glutSetCursor(GLUT_CURSOR_NONE);

      
      //-- Setup the 3D projection --
      GLdouble Left     = -((GLdouble)X_Res_Set / (GLdouble)Y_Res_Set) * (GLdouble)0.5;
      GLdouble Right    = -Left;
      GLdouble Bottom   = -(GLdouble)0.5;
      GLdouble Top      = -Bottom;
      GLdouble Near     =  (GLdouble)1.0;
      GLdouble Far      =  (GLdouble)100.0;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(Left, Right, Bottom, Top, Near, Far);
      //glOrtho(Left, Right, Bottom, Top, -Far, Far); 
         
      //-- Setup the 3D model view matrix along with the OpenGL viewport area --
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glScalef(1.0f, 1.0f, -1.0f);
      glViewport((GLint)0, (GLint)0, (GLsizei)X_Res_Set, (GLsizei)Y_Res_Set); //Set the viewport size  

      //-- Set up other OpenGL stuff --
      glDepthRange((GLfloat)0.0f, (GLfloat)1.0f);
      glDepthFunc(GL_LESS);
      glEnable(GL_DEPTH_TEST);

      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CW);
      glCullFace(GL_BACK);

      glEnable(GL_DITHER);


      //-- Set up VideoClass data --
      ModeValid        = true;
      FullScreen       = FullScreen_Set;
      Minimized        = false;
      X_Res            = X_Res_Set;
      Y_Res            = Y_Res_Set;
      BitsPerPixel     = BitsPerPixel_Set;
      BytesPerPixel    = BitsPerPixel >> 3;
      BytesPerLine     = BytesPerPixel * X_Res;
      XY_Ratio         = (float)X_Res / (float)Y_Res;
      YX_Ratio         = (float)Y_Res / (float)X_Res;

      //-- Get OpenGL information. NOTE: DeviceInfo at this point should be NULL. --
      char* String;
      if ((String = glGetAllocString(GL_VENDOR)) != NULL)
         {
         str_append(DeviceInfo, "Vendor     : ");
         str_append(DeviceInfo, String);
         str_append(DeviceInfo, "\n");
         delete[] String;
         }

      if ((String = glGetAllocString(GL_RENDERER)) != NULL)
         {
         str_append(DeviceInfo, "Renderer   : ");
         str_append(DeviceInfo, String);
         str_append(DeviceInfo, "\n");
         delete[] String;
         }

      if ((String = glGetAllocString(GL_VERSION)) != NULL)
         {
         str_append(DeviceInfo, "Version    : ");
         str_append(DeviceInfo, String);
         str_append(DeviceInfo, "\n");
         delete[] String;
         }

      if ((String = glGetAllocString(GL_EXTENSIONS)) != NULL)
         {
         //Substitue spaces with newline
         char* Str = String;
         while (*Str != '\0') {if (*Str == ' ') {*Str = '\n';}   Str++;}
         str_append(DeviceInfo, "Extensions...\n");
         str_append(DeviceInfo, String);
         str_append(DeviceInfo, "\n");
         delete[] String;
         }

      printf("\n%s\n\n", DeviceInfo);

      return true;
      }

   /*-------------------------------------------------------------------------
     Clears the frame buffer.

     R, G, B, A : Red, Green, Blue and Alpha colors.
     ------------------------------------------------------------------------*/
   bool ClearFrame(float R, float G, float B, float A)
      {
      if (!ModeValid) {return false;}
      if (Minimized) {return true;}

      glClearDepth(1.0f);
      glClearColor(R, G, B, A);                 //Set the clear color 
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Do the clearing 
      
      return true;
      }

   /*-------------------------------------------------------------------------
     Refreshes the the video with the frame buffer. It simply swaps the VideoBuffer
     with the FrameBuffer. If ClrFrame is set, the function will clear the frame
     buffer to the desired Color. The function returns true on a successful exit.

     ClrFrame   : If set, the frame buffer will be cleared after the swap.
     R, G, B, A : Red, Green, Blue and Alpha colors.
     ------------------------------------------------------------------------*/
   bool Refresh(bool ClrFrame, float R, float G, float B, float A)
      {
      if (!ModeValid) {return false;}
      if (Minimized) {return true;}

      glFlush();                                   //High end machines may need this

      glutPostRedisplay();                         //Render to frame buffer 
      glutSwapBuffers();                           //Swap the frame buffer and video buffer 

      if (ClrFrame) 
         {
         ClearFrame(R, G, B, A);                   //Clear current frame buffer if necessary 
         }
      else
         {
         glClearDepth(1.0f);
         glClear(GL_DEPTH_BUFFER_BIT);             //Z buffer is always cleared
         }
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Locks the video device, and makes the device available for redering.
      For OpenGL applications, this function does nothing, it is only present 
      for compatibility reasons.
     ------------------------------------------------------------------------*/
   bool Lock(void) 
      {
      return true;
      }

   /*-------------------------------------------------------------------------
      Unlocks the video device which was locked previously.
      For OpenGL applications, this function does nothing, it is only present 
      for compatibility reasons.
     ------------------------------------------------------------------------*/
   bool UnLock(void) 
      {
      return true;
      }

   /*-------------------------------------------------------------------------
      Captures the image from the frame buffer. The function accepts a 
      GPBitmapRec data structure (which must be initalized correctly), and
      retruns the allocated the texture in this structure.
     ------------------------------------------------------------------------*/
   bool CaptureFrame(BitmapRec* Bitmap)
      {
      if (!ModeValid || (Bitmap == NULL)) {return false;}
      //if (Minimized) {return true;}

      //Delete the existing texture data
      Bitmap->DeleteData();

      //Re-allocate new texture data, mathcing the screen parameters      
      Bitmap->Flags = BMP_TYPE_RGB;
      Bitmap->U_Res = X_Res;
      Bitmap->V_Res = Y_Res;
      Bitmap->FrameCount = 1;
      if (!Bitmap->AllocateData()) {return false;}
      
      //Copy the frame buffer contents and flip the image at the same time
      glRasterPos2i(0, 0);
      byte* ScanLinePtr = Bitmap->BitmapPtr;
      for (int V = Bitmap->V_Res-1; V >= 0; V--)
         {
         glReadPixels((GLint)0, (GLint)V, (GLint)Bitmap->U_Res, (GLint)1, (GLenum)GL_RGB, GL_UNSIGNED_BYTE, ScanLinePtr);
         ScanLinePtr += Bitmap->BytesPerLine;
         }
      
      return true;
      }

   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
OpenGLClass OpenGL;


/*---------------------------------------------------------------------------
   The reshape function. It must be external of OpenGLClass.
  ---------------------------------------------------------------------------*/
void __cdecl Reshape(int X_Res_Set, int Y_Res_Set)
   {
   //If the new resolution is 0, the window is minimized
   if ((X_Res_Set == 0) || (Y_Res_Set == 0)) {OpenGL.Minimized = true; return;}
   
   //-- Setup the 3D projection --
   GLdouble Left     = -((GLdouble)X_Res_Set / (GLdouble)Y_Res_Set);
   GLdouble Right    = -Left;
   GLdouble Bottom   = -(GLdouble)1.0;
   GLdouble Top      = -Bottom;
   GLdouble Near     =  (GLdouble)1.0;
   GLdouble Far      =  (GLdouble)100.0;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(Left, Right, Bottom, Top, Near, Far);
   //glOrtho(Left, Right, Bottom, Top, -Far, Far); 
         
   //-- Setup the 3D model view matrix along with the OpenGL viewport area --
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glScalef(1.0f, 1.0f, -1.0f);
   glViewport((GLint)0, (GLint)0, (GLsizei)X_Res_Set, (GLsizei)Y_Res_Set); //Set the viewport size  


   //Reinitialize the VideoClass data
   OpenGL.Minimized    = false;
   OpenGL.X_Res        = X_Res_Set;
   OpenGL.Y_Res        = Y_Res_Set;
   OpenGL.BytesPerLine = OpenGL.BytesPerPixel * OpenGL.X_Res;
   OpenGL.XY_Ratio     = (float)OpenGL.X_Res / (float)OpenGL.Y_Res;
   OpenGL.YX_Ratio     = (float)OpenGL.Y_Res / (float)OpenGL.X_Res;

   //Clear the screen
   glClearDepth(1.0f);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);     //Set the clear color
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Do the clearing 
   }

/*---------------------------------------------------------------------------
   Monitors display is visibilty.
  ---------------------------------------------------------------------------*/
void __cdecl Visibility(int State)
   {
   OpenGL.Minimized = (State == GLUT_NOT_VISIBLE) ? true : false;
   }

/*---------------------------------------------------------------------------
   Monitors display is visibilty.
  ---------------------------------------------------------------------------*/
void __cdecl Entry(int State)
   {
   //Enable control IO
   if (State == GLUT_ENTERED)
      {
      glutKeyboardFunc((void(*)(byte, int, int))KeyboardHandler);
      glutSpecialFunc((void(*)(int, int, int))KeyboardSpecialHandler);
      glutMouseFunc((void(*)(int, int, int, int))MouseHandler);
      glutMotionFunc((void(*)(int, int))MouseMotionHandler);
      glutPassiveMotionFunc((void(*)(int, int))MouseMotionHandler);
      glutSetCursor(GLUT_CURSOR_NONE);
      }

   //Disable control IO
   else if (State == GLUT_LEFT)
      {
      glutKeyboardFunc(NULL);
      glutSpecialFunc(NULL);
      glutMouseFunc(NULL);
      glutMotionFunc(NULL);
      glutPassiveMotionFunc(NULL);
      }
   }

/*==== End of File ===========================================================*/
#endif
