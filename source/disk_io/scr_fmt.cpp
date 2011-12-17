/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                       Custom Script File Reader                            */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __SCR_FMT_CPP__
#define __SCR_FMT_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_str.cpp"
#include "../_common/list.cpp"
#include "../math/primitive.cpp"
#include "../mem_data/cfg_data.cpp"
#include "../mem_data/entity.cpp"
#include "../mem_data/world.cpp"
#include "../disk_io/cob_fmt.cpp"


/*---------------------------------------------------------------------------

  ---------------------------------------------------------------------------*/
#define SCR_SIZE_LIMIT  0x00100000           //1 MB limit


/*---------------------------------------------------------------------------

  ---------------------------------------------------------------------------*/
//-- Video related keywords --
#define SCR_VIDEO         "VIDEO"
#define SCR_FULLSCREEN    "FULLSCREEN"
#define SCR_RES           "RES"
#define SCR_BITSPERPIXEL  "BITSPERPIXEL"
#define SCR_REFRESH       "REFRESH"

//-- Render related keywords --
#define SCR_RENDER        "RENDER"
#define SCR_CAMAPETURE    "CAMAPETURE"
#define SCR_PROFEQU       "PROFEQU"
#define SCR_PROFEQULIMX   "PROFEQULIMX"
#define SCR_NULL          "NULL"
#define SCR_CLEARFLAG     "CLEARFLAG"
#define SCR_SHADOWFLAG    "SHADOWFLAG"
#define SCR_REFRACTFLAG   "REFRACTFLAG"
#define SCR_REFLECTFLAG   "REFLECTFLAG"
#define SCR_HOTSPOTFLAG   "HOTSPOTFLAG"
#define SCR_ANTIALIASFLAG "ANTIALIASFLAG"
#define SCR_BACKGNDCOLOR  "BACKGNDCOLOR"
#define SCR_MAXRAYDEPTH   "MAXRAYDEPTH"
#define SCR_MAX_LOD       "MAX_LOD"
#define SCR_SUBDIVTRESH   "SUBDIVTRESH"
#define SCR_TABRES        "TABRES"
#define SCR_CURRENTDEVICE "CURRENTDEVICE"
#define SCR_AA_TRESHOLD   "AA_TRESHOLD"
#define SCR_AA_SAMPLES    "AA_SAMPLES"
#define SCR_AA_JITTER     "AA_JITTER"
#define SCR_ADAPTDEPTHTRESH "ADAPTDEPTHTRESH"
#define SCR_PCOMPFLAG     "PCOMPFLAG"
#define SCR_POFFSET       "POFFSET"
//-- World related keywords --
#define SCR_WORLD         "WORLD"
#define SCR_VORIGIN       "VORIGIN"
#define SCR_VVELOCITY     "VVELOCITY"
#define SCR_VROTATION     "VROTATION"
#define SCR_VORIENTATION  "VORIENTATION"

//-- Entity related keywords --
#define SCR_ENTITY        "ENTITY"
#define SCR_PLANE         "PLANE"
#define SCR_CUBE          "CUBE"

//-- Light related keywords --
#define SCR_LIGHT         "LIGHT"
#define SCR_AMBCOLOR      "AMBCOLOR"

//-- Other common keywords --
#define SCR_CLASS         "CLASS"
#define SCR_GRIDRES       "GRIDRES"
#define SCR_COLOR         "COLOR"
#define SCR_COORD         "COORD"
#define SCR_ORIENTATION   "ORIENTATION"
#define SCR_SCALECONST    "SCALECONST"
#define SCR_VELOCITY      "VELOCITY"
#define SCR_ROTATION      "ROTATION"



/*---------------------------------------------------------------------------
  The SCR file IO class.
  ---------------------------------------------------------------------------*/
class SCR_Class
   {
   /*==== Private Declarations ===============================================*/
   private:
   
   /*-------------------------------------------------------------------------
      Skips the comment field, including the newline characters, and returns 
      the string pointer after the new line. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   inline char* SkipComments(char* StrPtr)
      {
      if (StrPtr == NULL) {return NULL;}

      //Search for newline and skip past new line
      if      ((StrPtr[0] == '/') && (StrPtr[1] == '/')) {return str_skip(StrPtr, "\n", false) + 1;}
      else if ((StrPtr[0] == '/') && (StrPtr[1] == '*')) 
         {
         StrPtr += 2;
         while ((StrPtr[0] != '*') || (StrPtr[1] != '/')) 
            {if (StrPtr[0] == 0) {return StrPtr;} StrPtr++;}
         
         return StrPtr + 2;
         }
      
      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Returns the pointer of the next symbol in the text that is not a space, 
      tab, newline, or part of a comment field.
     -------------------------------------------------------------------------*/
   inline char* FindSymbol(char* StrPtr)
      {
      if (StrPtr == NULL) {return NULL;}

      char* LastStrPtr = NULL;
      while (StrPtr != LastStrPtr)
         {
         LastStrPtr = StrPtr;
         StrPtr     = str_skip(StrPtr, " \t\n", true);
         StrPtr     = SkipComments(StrPtr);
         }
      if (StrPtr == NULL) {printf("SCR_Class::FindSymbol( ): StrPtr == NULL.\n"); return NULL;}
      
      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Converts a float symbol to a float numerical value. Returns the pointer
      immediately after the numerical value. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   inline char* ReadFloat(char* StrPtr, float &Data)
      {
      if (StrPtr == NULL) {return NULL;}

      StrPtr = FindSymbol(StrPtr);
      if (StrPtr == NULL) {printf("SCR_Class::ReadFloat( ): StrPtr == NULL.\n"); return NULL;}

      //Do conversion
      Data = atof(StrPtr);
      StrPtr = str_skip(StrPtr, "-+.0123456789eEdD", true);

      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Converts a integer symbol to a integer numerical value. Returns the 
      pointer immediately after the numerical value. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   inline char* ReadInt(char* StrPtr, int &Data)
      {
      if (StrPtr == NULL) {return NULL;}

      StrPtr = FindSymbol(StrPtr);
      if (StrPtr == NULL) {printf("SCR_Class::ReadInt( ): StrPtr == NULL.\n"); return NULL;}

      //Do conversion
      Data = atoi(StrPtr);
      StrPtr = str_skip(StrPtr, "-+0123456789", true);

      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Converts a boolean symbol to a boolean value. Returns the pointer 
      immediately after the numerical value. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   inline char* ReadBool(char* StrPtr, bool &Data)
      {
      if (StrPtr == NULL) {return NULL;}

      StrPtr = FindSymbol(StrPtr);
      if (StrPtr == NULL) {printf("SCR_Class::ReadBool( ): StrPtr == NULL.\n"); return NULL;}

      //Do conversion
      if (atoi(StrPtr) != 0) {Data = true;} else {Data = false;}
      StrPtr = str_skip(StrPtr, "-+0123456789", true);

      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Reads an embedded string between "". Returns the pointer immediately 
      after the string value. Returns NULL on fail. The Data will return an 
      allocated string if AllocStr is set. Otherwise, Data must point to an 
      existing string of sufficient size.
     -------------------------------------------------------------------------*/
   inline char* ReadSubStr(char* StrPtr, char* &Data, bool AllocStr)
      {
      if (StrPtr == NULL) {return NULL;}
      if (!AllocStr && (Data == NULL)) {return NULL;}

      StrPtr = FindSymbol(StrPtr);
      if (StrPtr == NULL) {printf("SCR_Class::ReadBool( ): StrPtr == NULL.\n"); return NULL;}

      //Find opening "       
      if (*StrPtr != '"') {printf("SCR_Class::SubStr( ): The symbol \" is expected in front of string constant.\n"); return NULL;}
      StrPtr++;

      //Find the length of the string
      char* EndPtr = str_skip(StrPtr, "\n\"", false);
      if (*EndPtr != '"') {printf("SCR_Class::SubStr( ): The symbol \" is expected at the end of string constant.\n"); return NULL;}
      dword StrLength = EndPtr - StrPtr;

      //Allocate string if required
      if (AllocStr)
         {
         char* TempPtr = (char*)realloc(Data, StrLength + 1);
         if (TempPtr == NULL) {return NULL;}
         Data = TempPtr;
         }
      else {*Data = 0;}

      //Copy the data      
      strncpy(Data, StrPtr, StrLength);
      Data[StrLength] = 0;

      //Skip over the sub string and the closing "
      return EndPtr + 1;
      }

   /*-------------------------------------------------------------------------
      Reads a keyword, returns the value returned by fscanf( ). Returns EOF 
      if end of file was reached or on fail.
     -------------------------------------------------------------------------*/
   char* ReadKeyword(char* KeyWord, char* StrPtr)
      {
      if ((KeyWord == NULL) || (StrPtr == NULL)) {return NULL;}

      StrPtr = FindSymbol(StrPtr);
      if (StrPtr == NULL) {printf("SCR_Class::ReadKeyword( ): StrPtr == NULL.\n"); return NULL;}
      
      //Get the end pointer and the length of the keyword
      char* EndPtr = str_skip(StrPtr, " \t\n/", false);
      dword KeyWordLen = EndPtr - StrPtr;
      //if (KeyWordLen == 0) {printf("SCR_Class::ReadKeyword( ): KeyWordLen == 0.\n"); return NULL;}

      //Copy the keyword
      strncpy(KeyWord, StrPtr, KeyWordLen);
      KeyWord[KeyWordLen] = 0;

      return EndPtr;
      }

   /*-------------------------------------------------------------------------
      Setup all the Video config data.
     -------------------------------------------------------------------------*/
   char* SetupVideoCfg(char* KeyWord, char* StrPtr, ConfigRec* Config)
      {
      if ((KeyWord == NULL) || (StrPtr == NULL) || (Config == NULL)) {return NULL;}

      //-- Read local keywords --
      bool ExitLoop = false;
      while (!ExitLoop && (*StrPtr != 0))
         {
         StrPtr = ReadKeyword(KeyWord, StrPtr);
         if (StrPtr == NULL) {printf("SCR_Class::SetupVideoCfg( ): StrPtr == NULL.\n"); return NULL;}

         //Read the full screen flag
         if (stricmp(SCR_FULLSCREEN, KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Video.FullScreen);}

         //Video resolution
         else if (stricmp(SCR_RES, KeyWord) == 0) 
            {
            StrPtr = ReadInt(StrPtr, (int &)Config->Video.X_Res);
            StrPtr = ReadInt(StrPtr, (int &)Config->Video.Y_Res);
            }

         //Bits per pixel
         else if (stricmp(SCR_BITSPERPIXEL,  KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Video.BitsPerPixel);}

         //Bits per refresh rate
         else if (stricmp(SCR_REFRESH,       KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Video.RefreshRate);}

         //Handle unknown keywords
         else if (KeyWord[0] != 0) {StrPtr -= strlen(KeyWord); ExitLoop = true;}
         }

      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Setup all the Render config data.
     -------------------------------------------------------------------------*/
   char* SetupRenderCfg(char* KeyWord, char* StrPtr, ConfigRec* Config)
      {
      if ((KeyWord == NULL) || (StrPtr == NULL) || (Config == NULL)) {return NULL;}

      //-- Read local keywords --
      bool ExitLoop = false;
      while (!ExitLoop && (*StrPtr != 0))
         {
         StrPtr = ReadKeyword(KeyWord, StrPtr);
         if (StrPtr == NULL) {printf("SCR_Class::SetupRenderCfg( ): StrPtr == NULL.\n"); return NULL;}

         //Camera apeture and field of view
         if (stricmp(SCR_CAMAPETURE, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->Render.CamApeture.X);
            StrPtr = ReadFloat(StrPtr, Config->Render.CamApeture.Y);
            StrPtr = ReadFloat(StrPtr, Config->Render.CamApeture.Z);
            }
         
         //Read the profile curve
         else if (stricmp(SCR_PROFEQU,     KeyWord) == 0) 
            {
            StrPtr = ReadKeyword(KeyWord, StrPtr);
            if (StrPtr == NULL) {printf("SCR_Class::SetupRenderCfg( ): StrPtr == NULL.\n"); return NULL;}
            if (stricmp(SCR_NULL, KeyWord) == 0) {Config->Render.ProfEqu = NULL;}
            else {StrPtr = ReadSubStr(StrPtr-strlen(KeyWord), Config->Render.ProfEqu, true);}
            }
         
         //Read the profile curve X upper limit
         else if (stricmp(SCR_PROFEQULIMX, KeyWord) == 0) {StrPtr = ReadFloat(StrPtr, Config->Render.ProfEquLimX);Config->Render.ProfEquLimX = fabs(Config->Render.ProfEquLimX);}  
        
         //Read the clear flag
         else if (stricmp(SCR_CLEARFLAG,   KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.ClearFlag);}
         
         //Read the shadow flag
         else if (stricmp(SCR_SHADOWFLAG,  KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.ShadowFlag);}
         
         //Read the refract flag
         else if (stricmp(SCR_REFRACTFLAG, KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.RefractFlag);}
         
         //Read the reflect flag
         else if (stricmp(SCR_REFLECTFLAG, KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.ReflectFlag);}
         
         //Read the hotspot flag
         else if (stricmp(SCR_HOTSPOTFLAG, KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.HotspotFlag);}

         //Read the anti-alias flag
         else if (stricmp(SCR_ANTIALIASFLAG, KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.AntiAliasFlag);}

         //Read the projector compensation flag
         else if (stricmp(SCR_PCOMPFLAG,   KeyWord) == 0) {StrPtr = ReadBool(StrPtr, Config->Render.PCompFlag);}

         //Read the projector offset
         else if (stricmp(SCR_POFFSET,     KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->Render.POffset);
            
            //Must be always negative!
            Config->Render.POffset = (Config->Render.POffset != 0.0f) ? -fabs(Config->Render.POffset) : -1.0f;
            }

         //Background color
         else if (stricmp(SCR_BACKGNDCOLOR, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->Render.BackgndColor.R);
            StrPtr = ReadFloat(StrPtr, Config->Render.BackgndColor.G);
            StrPtr = ReadFloat(StrPtr, Config->Render.BackgndColor.B);
            }

         //Read the AA_Samples
         else if (stricmp(SCR_AA_SAMPLES, KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Render.AA_Samples);}
         
         //Read the AA_Jitter
         else if (stricmp(SCR_AA_JITTER,  KeyWord) == 0) {StrPtr = ReadFloat(StrPtr, Config->Render.AA_Jitter);}

         //Read the AA_Treshold
         else if (stricmp(SCR_AA_TRESHOLD, KeyWord) == 0) {StrPtr = ReadFloat(StrPtr, Config->Render.AA_Treshold);}
         
         //Read the maximum ray depth
         else if (stricmp(SCR_MAXRAYDEPTH, KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Render.MaxRayDepth);}

         //Read the adaptive depth treshold
         else if (stricmp(SCR_ADAPTDEPTHTRESH, KeyWord) == 0) {StrPtr = ReadFloat(StrPtr, Config->Render.AdaptDepthTresh);}
         
         //Read the maximum LOD
         else if (stricmp(SCR_MAX_LOD, KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Render.Max_LOD);}

         //Read the lookup table resolution
         else if (stricmp(SCR_TABRES, KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Render.TabRes);}

         //Read the maximum LOD
         else if (stricmp(SCR_SUBDIVTRESH, KeyWord) == 0) {StrPtr = ReadFloat(StrPtr, Config->Render.SubdivTresh);}

         //Rendering mode
         else if (stricmp(SCR_CURRENTDEVICE, KeyWord) == 0) {StrPtr = ReadInt(StrPtr, (int &)Config->Render.CurrentDevice);}

         //Handle unknown keywords
         else if (KeyWord[0] != 0) {StrPtr -= strlen(KeyWord); ExitLoop = true;}
         }

      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Setup the World data.
     -------------------------------------------------------------------------*/
   char* SetupWorld(char* KeyWord, char* StrPtr, ConfigRec* Config, WorldRec* World)
      {
      if ((KeyWord == NULL) || (StrPtr == NULL) || (Config == NULL) || (World == NULL)) {return NULL;}
     
      //-- Read local keywords --
      bool ExitLoop = false;
      while (!ExitLoop && (*StrPtr != 0))
         {
         StrPtr = ReadKeyword(KeyWord, StrPtr);
         if (StrPtr == NULL) {printf("SCR_Class::SetupWorld( ): StrPtr == NULL.\n"); return NULL;}

         //View origin
         if (stricmp(SCR_VORIGIN, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->World.VOrigin.X);
            StrPtr = ReadFloat(StrPtr, Config->World.VOrigin.Y);
            StrPtr = ReadFloat(StrPtr, Config->World.VOrigin.Z);
            }

         //View velocity
         else if (stricmp(SCR_VVELOCITY, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->World.VVelocity.X);
            StrPtr = ReadFloat(StrPtr, Config->World.VVelocity.Y);
            StrPtr = ReadFloat(StrPtr, Config->World.VVelocity.Z);
            }

         //View rotation
         else if (stricmp(SCR_VROTATION, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->World.VRotation.X);
            StrPtr = ReadFloat(StrPtr, Config->World.VRotation.Y);
            StrPtr = ReadFloat(StrPtr, Config->World.VRotation.Z);
            Config->World.VRotation.X = deg2rad(Config->World.VRotation.X);
            Config->World.VRotation.Y = deg2rad(Config->World.VRotation.Y);
            Config->World.VRotation.Z = deg2rad(Config->World.VRotation.Z);
            }

         //View orientation
         else if (stricmp(SCR_VORIENTATION, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->World.VOrientation.X);
            StrPtr = ReadFloat(StrPtr, Config->World.VOrientation.Y);
            StrPtr = ReadFloat(StrPtr, Config->World.VOrientation.Z);
            Config->World.VOrientation.X = deg2rad(Config->World.VOrientation.X);
            Config->World.VOrientation.Y = deg2rad(Config->World.VOrientation.Y);
            Config->World.VOrientation.Z = deg2rad(Config->World.VOrientation.Z);
            }

         //Handle unknown keywords
         else if (KeyWord[0] != 0) {StrPtr -= strlen(KeyWord); ExitLoop = true;}
         }

      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Setup an Entity.
     -------------------------------------------------------------------------*/
   char* SetupEntity(char* KeyWord, char* StrPtr, ConfigRec* Config, WorldRec* World)
      {
      if ((KeyWord == NULL) || (StrPtr == NULL) || (Config == NULL) || (World == NULL)) {return NULL;}
     
      //Local data      
      char  TempStr[0x100]; 
      char* EntityClass = TempStr;
      *EntityClass = 0;
      
      iTexPointRec GridRes     = 1;
      ColorRec     Color       = 1;
      PointRec     Coord       = 0;
      PointRec     Orientation = 0;
      PointRec     ScaleConst  = 1;
      PointRec     Velocity    = 0;
      PointRec     Rotation    = 0;

      //-- Read local keywords --
      bool ExitLoop = false;
      while (!ExitLoop && (*StrPtr != 0))
         {
         StrPtr = ReadKeyword(KeyWord, StrPtr);
         if (StrPtr == NULL) {printf("SCR_Class::SetupEntity( ): StrPtr == NULL.\n"); return NULL;}

         //Determine the entity class
         if (stricmp(SCR_CLASS, KeyWord) == 0) 
            {
            //Read the object type or file name
            StrPtr = ReadKeyword(KeyWord, StrPtr);
            if (StrPtr == NULL) {printf("SCR_Class::SetupEntity( ): StrPtr == NULL.\n"); return NULL;}

            //Determine the object type
            if ((stricmp(SCR_PLANE, KeyWord) == 0) || (stricmp(SCR_CUBE, KeyWord) == 0))
               {strcpy(EntityClass, KeyWord);}
            else {StrPtr = ReadSubStr(StrPtr-strlen(KeyWord), EntityClass, false);}
            }

         //Diffuse color
         else if (stricmp(SCR_COLOR, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Color.R);
            StrPtr = ReadFloat(StrPtr, Color.G);
            StrPtr = ReadFloat(StrPtr, Color.B);
            }
         
         //Grid resolution
         else if (stricmp(SCR_GRIDRES, KeyWord) == 0) 
            {
            StrPtr = ReadInt(StrPtr, GridRes.U);
            StrPtr = ReadInt(StrPtr, GridRes.V);
            }

         //Position
         else if (stricmp(SCR_COORD, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Coord.X);
            StrPtr = ReadFloat(StrPtr, Coord.Y);
            StrPtr = ReadFloat(StrPtr, Coord.Z);
            }

         //Orientation
         else if (stricmp(SCR_ORIENTATION, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Orientation.X);
            StrPtr = ReadFloat(StrPtr, Orientation.Y);
            StrPtr = ReadFloat(StrPtr, Orientation.Z);
            Orientation.X = deg2rad(Orientation.X);
            Orientation.Y = deg2rad(Orientation.Y);
            Orientation.Z = deg2rad(Orientation.Z);
            }

         //ScaleConst
         else if (stricmp(SCR_SCALECONST, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, ScaleConst.X);
            StrPtr = ReadFloat(StrPtr, ScaleConst.Y);
            StrPtr = ReadFloat(StrPtr, ScaleConst.Z);
            }

         //Velocity
         else if (stricmp(SCR_VELOCITY, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Velocity.X);
            StrPtr = ReadFloat(StrPtr, Velocity.Y);
            StrPtr = ReadFloat(StrPtr, Velocity.Z);
            }

         //Rotation
         else if (stricmp(SCR_ROTATION, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Rotation.X);
            StrPtr = ReadFloat(StrPtr, Rotation.Y);
            StrPtr = ReadFloat(StrPtr, Rotation.Z);
            Rotation.X = deg2rad(Rotation.X);
            Rotation.Y = deg2rad(Rotation.Y);
            Rotation.Z = deg2rad(Rotation.Z);
            }

         //Handle unknown keywords
         else if (KeyWord[0] != 0) {StrPtr -= strlen(KeyWord); ExitLoop = true;}
         }


      //-- Entity allocation --
      EntityRec* Entity = NULL;
      if (*EntityClass == 0) {printf("SCR_Class::SetupEntity( ): No entity class was defined.\n"); return NULL;}
      else if (stricmp(SCR_PLANE, EntityClass) == 0) 
         {
         Entity = Primitive.Plane(&GridRes, &ScaleConst, &Coord, &Color);
         if (Entity == NULL)
            {printf("SCR_Class::SetupEntity( ): Primitive.Plane( ) failed.\n"); return NULL;}
         }
      else if (stricmp(SCR_CUBE, EntityClass) == 0) 
         {
         Entity = Primitive.Cube(&ScaleConst, &Coord, &Color);
         if (Entity == NULL)
            {printf("SCR_Class::SetupEntity( ): Primitive.Cube( ) failed.\n"); return NULL;}
         }                         
      else
         {
         if (!COB.Read(EntityClass, Entity))
            {printf("SCR_Class::SetupEntity( ): COB.Read( ) failed.\n"); return NULL;}

         if (!Entity->Scale(&ScaleConst, &Entity->Centroid)) {if (Entity != NULL) {delete Entity;} return NULL;}
         if (!Entity->Translate(&Coord)) {if (Entity != NULL) {delete Entity;} return NULL;}
         } 

      //Do the initial orientation
      if (!Entity->Rotate(&Orientation, &Entity->Centroid)) {if (Entity != NULL) {delete Entity;} return NULL;}

      //Set the Entity's velocity and angular velocity
      Entity->Velocity = Velocity;
      Entity->Rotation = Rotation;


      //Insert the entity into the world
      if (!LinkedList.Insert(World->EntityList, Entity))
         {
         printf("SCR_Class::SetupEntity( ): LinkedList.Insert( ) failed.\n"); 
         if (Entity != NULL) {delete Entity;}
         return NULL;
         }
      
      return StrPtr;
      }

   /*-------------------------------------------------------------------------
      Setup a Light.
     -------------------------------------------------------------------------*/
   char* SetupLight(char* KeyWord, char* StrPtr, ConfigRec* Config, WorldRec* World)
      {
      if ((KeyWord == NULL) || (StrPtr == NULL) || (Config == NULL) || (World == NULL)) {return NULL;}
     
      //Local data      
      ColorRec Color = 1;
      PointRec Coord = 0;
      bool     AllocLight = true;


      //-- Read local keywords --
      bool ExitLoop = false;
      while (!ExitLoop && (*StrPtr != 0))
         {
         StrPtr = ReadKeyword(KeyWord, StrPtr);
         if (StrPtr == NULL) {printf("SCR_Class::SetupLight( ): StrPtr == NULL.\n"); return NULL;}

         //Setup the light color
         if (stricmp(SCR_COLOR, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Color.R);
            StrPtr = ReadFloat(StrPtr, Color.G);
            StrPtr = ReadFloat(StrPtr, Color.B);
            }
         
         //Setup the ambient light color
         else if (stricmp(SCR_AMBCOLOR, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Config->World.AmbLight.R);
            StrPtr = ReadFloat(StrPtr, Config->World.AmbLight.G);
            StrPtr = ReadFloat(StrPtr, Config->World.AmbLight.B);
            AllocLight = false;
            }
         
         //Position
         else if (stricmp(SCR_COORD, KeyWord) == 0) 
            {
            StrPtr = ReadFloat(StrPtr, Coord.X);
            StrPtr = ReadFloat(StrPtr, Coord.Y);
            StrPtr = ReadFloat(StrPtr, Coord.Z);
            }

         //Handle unknown keywords
         else if (KeyWord[0] != 0) {StrPtr -= strlen(KeyWord); ExitLoop = true;}
         }


      //-- Light allocation --
      if (AllocLight)
         {
         LightRec* Light = new LightRec;
         if (Light == NULL) {printf("SCR_Class::SetupLight( ): Light allocation failed.\n"); return NULL;}

         Light->Color = Color;
         Light->Coord = Coord;

         //Insert the Light into the world
         if (!LinkedList.Insert(World->LightList, Light))
            {
            printf("SCR_Class::SetupLight( ): LinkedList.Insert( ) failed.\n"); 
            if (Light != NULL) {delete Light;}
            return NULL;
            }
         }

      return StrPtr;
      }


   /*==== Public Declarations ================================================*/
   public:

   /*-------------------------------------------------------------------------
      Read is not supported.
     -------------------------------------------------------------------------*/
   bool Read(char* FileName, ConfigRec* Config, WorldRec* World)
      {
      if ((FileName == NULL) || (Config == NULL) || (World == NULL)) {return false;}


      //Local data
      char  KeyWord[0x100];
      int   ErrorCode = 0;
      dword ScriptLen = 0;
      dword ScriptRead;
      char* ScriptStr = NULL;
      char* StrPtr    = NULL;


      //-- Open the script file --
      FILE* SCR_File = fopen(FileName, "rt");
      if (SCR_File == NULL) {goto _ExitError;}

      //Find the length of the entire script
      fseek(SCR_File, 0, SEEK_END);
      ScriptLen = ftell(SCR_File);
      fseek(SCR_File, 0, SEEK_SET);

      //Filesize check
      if (ScriptLen > SCR_SIZE_LIMIT) 
         {printf("SCR_Class::Read( ): Script file \"%s\" is over the %u byte limit.\n", FileName, SCR_SIZE_LIMIT); goto _ExitError;}

      //Allocate the string buffer
      StrPtr = ScriptStr = new char[ScriptLen + 1];
      if (ScriptStr == NULL) {printf("SCR_Class::Read( ): Script string allocation error.\n"); goto _ExitError;}

      //Read the file into the buffer
      ScriptRead = fread(ScriptStr, 1, ScriptLen, SCR_File);
      if ((ScriptRead != ScriptLen) && (feof(SCR_File) == 0))
         {printf("SCR_Class::Read( ): File IO error.\n"); goto _ExitError;}

      //Get rid of linefeed chars
      while (StrPtr < ScriptStr + ScriptLen) {if (*StrPtr == 0x0D) {*StrPtr = ' ';} StrPtr++;}
      StrPtr = ScriptStr;
      ScriptStr[ScriptRead] = 0;


      //-- Read the script --
      while (*StrPtr != 0)
         {
         StrPtr = ReadKeyword(KeyWord, StrPtr);
         if (StrPtr == NULL) {printf("SCR_Class::Read( ): StrPtr == NULL.\n"); goto _ExitError;}

         if      (stricmp(SCR_RENDER, KeyWord) == 0) {StrPtr = SetupRenderCfg(KeyWord, StrPtr, Config);}
         else if (stricmp(SCR_VIDEO,  KeyWord) == 0) {StrPtr = SetupVideoCfg(KeyWord, StrPtr, Config);}
         else if (stricmp(SCR_WORLD,  KeyWord) == 0) {StrPtr = SetupWorld(KeyWord, StrPtr, Config, World);}
         else if (stricmp(SCR_ENTITY, KeyWord) == 0) {StrPtr = SetupEntity(KeyWord, StrPtr, Config, World);}
         else if (stricmp(SCR_LIGHT,  KeyWord) == 0) {StrPtr = SetupLight(KeyWord, StrPtr, Config, World);}
         else if (KeyWord[0] != 0) {printf("SCR_Class::Read( ): Script file \"%s\", invalid symbol \"%s\".\n", FileName, KeyWord); goto _ExitError;}
         
         if (StrPtr == NULL) {printf("SCR_Class::Read( ): StrPtr == NULL.\n"); goto _ExitError;}
         }


      //-- Normal exit --
      if (SCR_File  != NULL) {fclose(SCR_File);}
      if (ScriptStr != NULL) {delete[] ScriptStr;}
      return true;

      //-- Exit with error --
      _ExitError:
      if (SCR_File  != NULL) {fclose(SCR_File);}
      if (ScriptStr != NULL) {delete[] ScriptStr;}
      return false;
      }


   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
SCR_Class SCR;


/*==== End of file ===========================================================*/
#endif
