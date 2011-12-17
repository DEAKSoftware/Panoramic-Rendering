/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __COSMIC_RAY_TAU_CPP__
#define __COSMIC_RAY_TAU_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "cosmic_ray.h"


/*----------------------------------------------------------------------------
   This function should be called once at startup. It calls detection routines, 
   initialises data and sets up video.
  ----------------------------------------------------------------------------*/
bool CosmosInit(void)
   {
   srand((unsigned)time(NULL));

   //Process command line arguments
   if (SystemFlags.ArgCount < 2)
      {printf("\n%s\n\nUsage: %s [script_file.scr]\n", Title, SystemFlags.Argv[0]); return false;}
   
   //Read the script file
   if (!SCR.Read(SystemFlags.Argv[1], &Config, &World))
      {printf("CosmosInit( ): SCR.Read( ) failed.\n"); return false;}

   //Check if we have the decent video config
   if ((Config.Video.X_Res == 0) || 
       (Config.Video.Y_Res == 0) || 
       (Config.Video.BitsPerPixel == 0))
      {printf("CosmosInit( ): A shifty video mode was specified in Config.\n"); return false;}


   //-- Setup the rendering and video interface --           
   if (!Render->Interface(Config.Render.CurrentDevice, 
                          Render, Video, Config.Video.X_Res, Config.Video.Y_Res, 
                          Config.Video.BitsPerPixel, Config.Video.RefreshRate, 
                          Config.Video.FullScreen, 
                          Config.Render.ProfEqu, Config.Render.ProfEquLimX, 
                          &Config.Render.CamApeture,
                          Config.Render.TabRes))
      {printf("CosmosInit( ): Render.Interface( ) failed.\n"); return false;}

   //Setup the render data
   Render->ClearFlag       = Config.Render.ClearFlag;
   Render->ShadowFlag      = Config.Render.ShadowFlag;
   Render->RefractFlag     = Config.Render.RefractFlag;
   Render->ReflectFlag     = Config.Render.ReflectFlag;
   Render->HotspotFlag     = Config.Render.HotspotFlag;
   Render->AntiAliasFlag   = Config.Render.AntiAliasFlag;
   Render->BackgndColor    = Config.Render.BackgndColor;
   Render->MaxRayDepth     = Config.Render.MaxRayDepth;
   Render->AdaptDepthTresh = Config.Render.AdaptDepthTresh;
   Render->Max_LOD         = Config.Render.Max_LOD;
   Render->SubdivTresh     = Config.Render.SubdivTresh;
   Render->AA_Samples      = Config.Render.AA_Samples;
   Render->AA_Jitter       = Config.Render.AA_Jitter;
   Render->AA_Treshold     = Config.Render.AA_Treshold;
   Render->PCompFlag       = Config.Render.PCompFlag;
   Render->POffset         = Config.Render.POffset;

   //Setup the world data
   World.VOrigin           = Config.World.VOrigin;
   World.VVelocity         = Config.World.VVelocity;
   World.VRotation         = Config.World.VRotation;
   World.VOrientation      = Config.World.VOrientation;
   World.AmbLight          = Config.World.AmbLight;

   return true;
   }

/*----------------------------------------------------------------------------

  ----------------------------------------------------------------------------*/
bool RenderModeSwitch(dword RenderDevice)
   {
   //Toggle between rendering devices
   Config.Render.CurrentDevice = RenderDevice;

   //Print debug info        
   printf("\n\n-------- %s Renderer Setup --------\n\n", (Config.Render.CurrentDevice == RENDER_OPENGL) ? "OpenGL" : "Ray-Tracer");

   //***** Temp hack fix: prevents a crash during render change if window res has changed *****
   #if defined (WIN32) || defined (WIN32_NT)
   Config.Video.X_Res = Video->X_Res;
   Config.Video.Y_Res = Video->Y_Res;
   #endif

   //-- Setup the rendering and video interface --           
   if (!Render->Interface(Config.Render.CurrentDevice, 
                           Render, Video, Config.Video.X_Res, Config.Video.Y_Res, 
                           Config.Video.BitsPerPixel, Config.Video.RefreshRate, 
                           Config.Video.FullScreen, 
                           Config.Render.ProfEqu, Config.Render.ProfEquLimX, 
                           &Config.Render.CamApeture,
                           Config.Render.TabRes))
      {printf("RenderModeSwitch( ): Render.Interface( ) failed.\n"); return false;}

   Render->ClearFlag       = Config.Render.ClearFlag;
   Render->ShadowFlag      = Config.Render.ShadowFlag;
   Render->RefractFlag     = Config.Render.RefractFlag;
   Render->ReflectFlag     = Config.Render.ReflectFlag;
   Render->HotspotFlag     = Config.Render.HotspotFlag;
   Render->AntiAliasFlag   = Config.Render.AntiAliasFlag;
   Render->BackgndColor    = Config.Render.BackgndColor;
   Render->MaxRayDepth     = Config.Render.MaxRayDepth;
   Render->AdaptDepthTresh = Config.Render.AdaptDepthTresh;
   Render->Max_LOD         = Config.Render.Max_LOD;
   Render->SubdivTresh     = Config.Render.SubdivTresh;
   Render->AA_Samples      = Config.Render.AA_Samples;
   Render->AA_Jitter       = Config.Render.AA_Jitter;
   Render->AA_Treshold     = Config.Render.AA_Treshold;
   Render->PCompFlag       = Config.Render.PCompFlag;
   Render->POffset         = Config.Render.POffset;

   return true;
   }


/*----------------------------------------------------------------------------
   This function should be called once before exit. It will reset and restore
   the system to it's former settings. Returns true if successful. If a fail has
   occured, the function will not exit immediately, as other systems still needs
   resetting.
  ----------------------------------------------------------------------------*/
bool CosmosShutDown(void)
   {
   bool StatusFlag = true;

   //-- Save benchmark data --
   FILE* TXT_File = fopen("debug.txt", "wt");
   if (TXT_File != NULL)
      {
      fprintf(TXT_File, "---- Benchmark for \"%s\" ----\n\n"
                        #if defined (TRANSFORM_SLOW)
                        "Transfomation             : Direct. \n"
                        #else
                        "Transfomation             : Lookup Table. \n"
                        #endif
                        "Profile Curve             : %s \n"
                        "Total Frames              : %u \n"
                        "Average FPS               : %.3f \n"
                        "Average Polygons per Frame: %.3f \n"
                        "Average Vertices per Frame: %.3f \n",
                        SystemFlags.Argv[1],
                        Config.Render.ProfEqu,
                        SystemFlags.Frame,
                        SystemFlags.AccumFPS           / (float)SystemFlags.Frame,
                        (float)SystemFlags.AccumPolys  / (float)SystemFlags.Frame,
                        (float)SystemFlags.AccumPoints / (float)SystemFlags.Frame);
      fclose(TXT_File);
      }


   //-- Delete world data --
   World.Nuke();

   //-- Shutdown renderer --
   if (!Render->ShutDown())
      {printf("Render->ShutDown( ) failed.\n"); return false;}

   //-- Shutdown video interface --
   if (!Video->Restore())
      {printf("Video->Restore( ) failed.\n"); StatusFlag = false;}

   return StatusFlag;
   }

/*----------------------------------------------------------------------------
   Manage user controled interface.
  ----------------------------------------------------------------------------*/
bool CTRL_Interface(void)
   {
   static bool RecorderFlag = false;
   static bool MotionFlag;
   MotionFlag = false;
   
   
   //---- Handle the mouse interface ----
   iPointRec Coord = Mouse.GetCoord(MOUSE_REL_COORD);

   //Set the rotation
   //World.VRotation.X = -deg2rad((float)Coord.Y * 0.125);
   //World.VRotation.Y =  deg2rad((float)Coord.X * 0.125);

   //This resets the internal relative coords to 0. This is useful, coz GLUT
   // doesn't call Mouse.MotionHandler( ) when the mouse is not moving. This
   // leads to problems, because the internal relative coords will not be
   // reset and the view rotation and velocity will drift even though the mouse
   // is stationary. NOTE: The relative coords beyond this point will have no 
   // usefulness.
   Coord = Mouse.GetCoord(MOUSE_COORD);
   Mouse.MotionHandler(Coord.X, Coord.Y);

   //Reset motion data
   World.VVelocity = 0.0f;
   World.VRotation = 0.0f;

   //---- Handle the keyboard interface ----
   bool RotateVelocity = false;
   byte Char = Keyboard.GetChar();
   switch (Char)
      {
      //Exit 
      case 'q' : 
      case 'Q' : 
      case 27  : printf("\nExit.\n"); SystemFlags.ShutDown = true; break;

      //Navigation
      case KEYB_UP    : World.VVelocity.Z =  0.125; RotateVelocity = true; MotionFlag = true; break;
      case KEYB_DOWN  : World.VVelocity.Z = -0.125; RotateVelocity = true; MotionFlag = true; break;
      case KEYB_LEFT  : World.VRotation.Y = -deg2rad(1.0f); MotionFlag = true; break;
      case KEYB_RIGHT : World.VRotation.Y =  deg2rad(1.0f); MotionFlag = true; break;
      case '<'        :
      case ','        : World.VVelocity.X = -0.125; RotateVelocity = true; MotionFlag = true; break;
      case '>'        :
      case '.'        : World.VVelocity.X =  0.125; RotateVelocity = true; MotionFlag = true; break;
      case 'a'        :
      case 'A'        : World.VVelocity.Y =  0.125; RotateVelocity = true; MotionFlag = true; break;
      case 'z'        : 
      case 'Z'        : World.VVelocity.Y = -0.125; RotateVelocity = true; MotionFlag = true; break;
      case 'w'        : 
      case 'W'        : Render->WireFrame = !Render->WireFrame; break;

      case KEYB_F10   :
         {
         printf("\nSaving Surface.\n");
         EntityRec* Surface = Primitive.Surface(Render->ProfEqu, Render->ProfEquLimR, Render->PCompFlag, Render->POffset, Video->XY_Ratio);
         if (Surface == NULL) {printf("CTRL_Interface( ): Primitive.Surface( ) failed.\n"); return false;}

         if (!ASC.Save("surface.asc", Surface))
            {printf("CTRL_Interface( ): ASC.Save( ) failed.\n"); return false;}

         delete Surface; Surface = NULL;
         break;
         }

      case 'c'        : 
      case 'C'        :
         {
         //Switch to OpenGL if not in recording mode
         if ((!RecorderFlag) && (Render->CurrentDevice == RENDER_RAY))
            {
            if (!RenderModeSwitch(RENDER_OPENGL))
               {printf("RenderModeSwitch( ) failed.\n"); return false;}
            }

         //Setup recorder if not in recording mode
         if (!RecorderFlag)
            {
            RecorderFlag = true;
            if (!Recorder.Initialize(1800))
               {printf("Recorder.Initialize( ) failed.\n"); return false;}

            if (!Recorder.InitRec())
               {printf("Recorder.InitRec( ) failed.\n"); return false;}

            printf("Motion Record Enabled.\n");
            break;
            }

         //Setup the ray-tracer, and stop recording
         if (Recorder.RecMode)
            {
            if (!RenderModeSwitch(RENDER_RAY))
               {printf("RenderModeSwitch( ) failed.\n"); return false;}

            if (!Recorder.InitPlay())
               {printf("Recorder.InitPlay( ) failed.\n"); return false;}

            printf("Motion Record Disabled.\nMotion Playback Enabled.\n");
            }
         
         //Else, stop replaying
         else
            {
            RecorderFlag = false;
            Recorder.ShutDown();
            printf("Motion Record\\Playback Disabled.\n");
            }

         break;
         }

      //Screen capture
      case KEYB_F11   :
      case KEYB_F12   :
         {
         if (RecorderFlag) {break;}

         BitmapRec Bitmap;
         if (!Video->CaptureFrame(&Bitmap)) {printf("Video->CaptureFrame( ) failed.\n"); return false;}

         char FileName[64]; *FileName = 0;
         if (Char == KEYB_F11)
            {
            str_compose(FileName, "screen-frame-%.8d.ppm", SystemFlags.Frame);
            if (!PPM.Save(FileName, &Bitmap, false)) {printf("PPM.Save( ) failed.\n"); return false;}
            }
         else
            {
            str_compose(FileName, "screen-frame-%.8d.tga", SystemFlags.Frame);
            if (!TGA.Save(FileName, &Bitmap, true)) {printf("TGA.Save( ) failed.\n"); return false;}
            }

         printf("\nFrame captured to \"%s\".\n", FileName);

         Bitmap.DeleteData();
         break;
         }

      //Toggle between ray-tracer and OpenGL renderer
      case 'r':
      case 'R':
         {
         if (RecorderFlag) {break;}

         if (!RenderModeSwitch((Render->CurrentDevice == RENDER_OPENGL) ? RENDER_RAY : RENDER_OPENGL))
            {printf("RenderModeSwitch( ) failed.\n"); return false;}
         break;
         }
      }

   //Rotate the velocity vector to align with the current view orientation
   if (RotateVelocity) {World.VVelocity = World.VVelocity.Rotate(World.VOrientation);}


   //---- Save each redered frame if necessary ----
   if (RecorderFlag)
      {
      bool SequenceEnd = false;
      if (!Recorder.RecMode)
         {
         Render->RenderDone = false;

         BitmapRec Bitmap;
         if (!Video->CaptureFrame(&Bitmap)) {printf("Video->CaptureFrame( ) failed.\n"); return false;}

         char FileName[64]; *FileName = 0;
         str_compose(FileName, "screen-frame-%.8d.tga", SystemFlags.Frame);
         if (!TGA.Save(FileName, &Bitmap, true)) {printf("TGA.Save( ) failed.\n"); return false;}

         printf("\nFrame captured to \"%s\".\n", FileName);
         Bitmap.DeleteData();
      
         if (!Recorder.PlayFrame(World.VOrientation, World.VOrigin, SequenceEnd))
            {printf("Recorder.PlayFrame( ) failed.\n"); return false;}
         
         World.VRotation = World.VVelocity = 0.0f;

         //Shutdown if necessary
         if (SequenceEnd)
            {
            RecorderFlag = false;
            Recorder.ShutDown();
            printf("Motion Recorder\\Playback Disabled.\n");
            }
         }
      
      //Record only motion
      else if (MotionFlag)
         {
         dword CurrentFrame;
         if (!Recorder.RecFrame(World.VOrientation, World.VOrigin, SequenceEnd, CurrentFrame))
            {printf("Recorder.RecFrame( ) failed.\n"); return false;}

         printf("\nRecorded frame %+8u\n", CurrentFrame);

         if (SequenceEnd)
            {
            //Switch to Ray 
            if (Render->CurrentDevice == RENDER_OPENGL)
               {
               if (!RenderModeSwitch(RENDER_RAY))
                  {printf("RenderModeSwitch( ) failed.\n"); return false;}
               }

            if (!Recorder.InitPlay())
               {printf("Recorder.InitPlay( ) failed.\n"); return false;}
            }
         }
      }

   return true;
   }

/*----------------------------------------------------------------------------
   Main loop.
  ----------------------------------------------------------------------------*/
void __cdecl MainLoop(void)
   {
   //-- Render the scene --
   if (!Render->DrawScene(World.EntityList, World.LightList, &World))
      {printf("Render->DrawScene( ) failed.\n"); SystemFlags.ShutDown = true;}
        
   //-- Handle user Interface --
   if (!CTRL_Interface())
      {printf("CTRL_Interface( ) failed.\n"); SystemFlags.ShutDown = true;}

   //-- Refresh display --
   if (!Video->Refresh(Render->ClearFlag, Render->BackgndColor.R, Render->BackgndColor.G, Render->BackgndColor.B, 0.0f))
      {printf("Video->Refresh( ) failed.\n"); SystemFlags.ShutDown = true;}
        
   //-- Batch process the entire world Entity list --
   if (!World.BatchProcess())
      {printf("World.BatchProcess( ) failed.\n"); SystemFlags.ShutDown = true;}

   //Display frame rate  
   float FPS = 1.0f / SystemTimer.ReadTS_DiffSec();
   SystemFlags.AccumFPS += FPS;
   printf("Frame: %+6u, FPS : %.3f\t%c", SystemFlags.Frame, FPS, 0x0D);
   SystemTimer.TS_DiffStart();

   //Increment frame counter
   SystemFlags.Frame++;

   //-- Exit on shutdown signal --
   if (SystemFlags.ShutDown)
      {
      //== Restore all system settings ==
      if (!CosmosShutDown()) {printf("CosmosShutDown( ) failed.\n");}
      exit(0);
      }
   }


/*----------------------------------------------------------------------------
   Program entry point.
  ----------------------------------------------------------------------------*/
int __cdecl main(int ArgCount, char* *Argv)
   {
   //-- Get command line arguments --
   SystemFlags.ArgCount = ArgCount;
   SystemFlags.Argv     = Argv;

   
   //== Set up the Cosmos 3D engine ==
   if (!CosmosInit()) 
      {
      printf("CosmosInit( ) failed.\n");
      return dword_MAX;
      }

   //== Execute the main loop ==
   if (Video->CurrentDevice == VIDEO_OPENGL) {glutMainLoop();}
   else {while(true) {MainLoop();}}

   return 0;
   }


/*==== End of file ===========================================================*/
#endif