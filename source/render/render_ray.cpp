/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                      Ray Tracer Rendering Functions                        */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __RENDER_RAY_CPP__
#define __RENDER_RAY_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../render/render.cpp"
#include "../video_io/video.cpp"
#include "../mem_data/entity.cpp"
#include "../mem_data/world.cpp"
#include "../mem_data/bitmap.cpp"
#include "../math/mathcnst.h"
#include "../math/mathpoly.cpp"
#include "../math/equsolver.cpp"
#include "../system/systimer.cpp"



/*---------------------------------------------------------------------------
  The RenderRay class.
  ---------------------------------------------------------------------------*/
class RenderRayClass : public RenderClass, private ShadeClass, private TransformClass
   {
   /*==== Private Declarations ===============================================*/
   private:

   /*---- Data ---------------------------------------------------------------*/
   BitmapRec Frame;                             //Bitmap image
   ListRec*  Loc_EntityList;                    //The local copy of the Entity list
   ListRec*  Loc_LightList;                     //The local copy of the Light list
   WorldRec* Loc_World;                         //The local copy of the World data
   int       gl_ColorFmt;                       //OpenGL specific flags
   float*    ColorScan;
   float*    LastColorScan;


   /*-------------------------------------------------------------------------
      Returs a jittered version of the ray.
     ------------------------------------------------------------------------*/
   PointRec JitterRay(PointRec* Ray)
      {
      //Create a randomized offset
      PointRec Jitter = PointRec((1.0f - 2.0f*(float)rand()/(float)RAND_MAX) * AA_Jitter,
                                 (1.0f - 2.0f*(float)rand()/(float)RAND_MAX) * AA_Jitter,
                                  1.0f, 0.0f);

      //Now rotate the jitter vector, so that it aligns with Ray
      float m = (float)sqrt(sqr(Ray->Y) + sqr(Ray->Z));
      PointRec NewRay;
      NewRay.X =  Jitter.X * m + Jitter.Z * Ray->X;
      m = (m != 0.0f) ? (m = 1.0f / m) : m;
      NewRay.Y = -(Jitter.X * Ray->X * Ray->Y + Jitter.Y * Ray->Z) * m + 
                   Jitter.Z * Ray->Y;
      NewRay.Z = -(Jitter.X * Ray->X * Ray->Z - Jitter.Y * Ray->Y) * m + 
                   Jitter.Z * Ray->Z;

      return NewRay.Unit();
      }

   /*-------------------------------------------------------------------------
      Test if a line intersects an Entity's bounding volume. Returns true
      if intersection occured.

      BUG: If the unit ray D is parralel to any of the bounding volume planes,
           the function incorrectly returns no intersection flag, even though
           the intersection should have occured. This will produce a vertical
           and a horizontal gap in the center of the final image. A quick hack
           is to make the ray D slightly non-parallel.

      O      : Origin of the line.
      D      : Direction of the line (unit vector).
      Entity : Entity to test.
     ------------------------------------------------------------------------*/
   bool inline EntityIntersect(PointRec* O, PointRec* D, EntityRec* Entity)
      {
      if (Entity == NULL) {return false;}

      PointRec I1, I2;
      bool     Flag1; 
      bool     Flag2;
      float    t_min = float_MIN; 
      float    t_max = float_MAX;

      for (int Incr = 0; Incr < 6; Incr += 2)
         {
         //Get the intersection with the two opposite faces
         Flag1 = Plane_InfLine_Intersect(&I1, O, D, &Entity->BV[1]->Coord, &Entity->BV[Incr  ]->Normal, true);
         Flag2 = Plane_InfLine_Intersect(&I2, O, D, &Entity->BV[7]->Coord, &Entity->BV[Incr+1]->Normal, true);

         //If an intersection occured with any bounding volume face, do 
         // further testing.
         if (Flag1 || Flag2)
            {            
            if (I1.t > I2.t) {float temp = I1.t; I1.t = I2.t; I2.t = temp;}
            if (I1.t > t_min) {t_min = I1.t;}
            if (I2.t < t_max) {t_max = I2.t;}
            if ((t_max < t_min) || (t_max < 0.0f)) {return false;}
            }
         else {return false;}
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      This function tests the entire scene whether the Ray intersects any object 
      in the scene. Returns true if intersection has occured.
      
      I           : The intersection point is returned here. On entry, the 
                    parameter I->t is used find the closest intersection point. 
                    I->t must be set to float_MAX when calling IntersectScene() 
                    for the first time.
      Origin      : Origin of the ray.
      Ray         : The direction of the ray, must be a unit vector.
      Surface     : The pointer of the intersected surface returned here.
      ExclSurface : The previous intersection surface in the previous recursion
                    level of TraceRay(). PSurface will be excluded from intersection 
                    tests, as the origin of the ray lies on that surface. It can
                    be left to NULL if no surface exclusion is required.
      EntityList  : The list of Entities and sub-Entities to test.
     ------------------------------------------------------------------------*/
   bool IntersectScene(PointRec* I, PointRec* Origin, PointRec* Ray, PolygonRec* &Surface, PolygonRec* ExclSurface, ListRec* EntityList)
      {
      bool        IFlag = false;                      //Intersect flag
      PointRec    NewI;                               //New intersection point
      
      //---- Test each Entity ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         #define Entity ((EntityRec*)EntityNode->Data)
         if (Entity == NULL) {return false;}

         //---- Do bounding volume test. If the ray intersects this Entity's
         //     bounding volume, it's polygons and subEntities will 
         //     be tested. ----
         if (EntityIntersect(Origin, Ray, Entity))
            {
            //-- Test for intersection in each Polygon in the Entity --
            ListRec* PolygonNode = Entity->PolygonList;
            while (PolygonNode != NULL)
               {
               #define Polygon ((PolygonRec*)PolygonNode->Data)
               if (Polygon == NULL) {return false;}

               //Exclude PSurface from testing
               if (Polygon != ExclSurface)
                  {
                  if (Poly_InfLine_Intersect(&NewI, Origin, Ray, Polygon, false))
                     {
                     //We want the closest intersection point from the origin
                     if (I->t > NewI.t)
                        {
                        *I      = NewI;
                        Surface = Polygon;
                        IFlag   = true;
                        }
                     }
                  }

               PolygonNode = PolygonNode->Next;
               #undef Polygon
               }                                   //END while PolygonNode...

            //-- Recursively test the sub-Entities, and set the 
            //   intersection flag accordingly. --
            if (Entity->EntityList != NULL)
               {
               IFlag |= IntersectScene(I, Origin, Ray, Surface, ExclSurface, Entity->EntityList);
               }
            }                                      //END if BV_Intersect()...

         //Advance to the next Entity node
         EntityNode = EntityNode->Next;
         #undef Entity
         }
            
      return IFlag;
      }

   /*-------------------------------------------------------------------------
     The ray-tracer function.

     LocalColor  : Shading color that will be returned
     Origin      : Origin of the incident ray
     Ray         : Direction of the incident ray, must be a unit vector.
     ExclSurface : Pointer to the surface to exclude from intersection testing.
                   This is useful if the Origin lies on the previously tested
                   surface. If no surfaces are to be excluded, set it to NULL.
     Depth       : Current recursion depth
     Attenuation : The accumulated attenuation factor
     Inside      : This flag is set true if Ray was transmitted into PrevSurface
     -------------------------------------------------------------------------*/
   void RayTrace(ColorRec* LocalColor, PointRec* Origin, PointRec* Ray, PolygonRec* ExclSurface, dword Depth, float &Attenuation, bool Inside)
      {
      //Do intersection test with every Entity in the world
      PointRec I;                                     //Intersection point 
      PolygonRec* Surface;                            //Surface that had the intersection
      I.t = float_MAX;                                //t must be set to extreme maximum!

      //Return becomes blackground color if no intersection occured
      if (!IntersectScene(&I, Origin, Ray, Surface, ExclSurface, Loc_EntityList))
         {*LocalColor = BackgndColor; return;} 


      //Get local color at intersection, take every light 
      // source into consideration
      ShadePhong(LocalColor, &I, &Loc_World->VOrigin, Surface, Loc_EntityList, Loc_LightList, ShadowFlag);


      //Compute the attenuation factor
      if ((Surface->Reflect != 0.0f) && ReflectFlag) {Attenuation *= Surface->Reflect;}
      if (Surface->Trans != 0.0f) {Attenuation *= Surface->Trans;}

                       
      //Don't recurse at maximum depth, or under the attenuation threshold
      ColorRec ReflectColor, RefractColor;
      //if ((Depth >= MaxRayDepth) || (Attenuation < AdaptDepthTresh))
      if (Depth >= MaxRayDepth)
         {
         ReflectColor = 0.0f; 
         RefractColor = BackgndColor;
         }  

      //======== Recurse for reflected and refracted rays ========
      else
         {
         //-- Ray trace the reflected ray --
         if ((Surface->Reflect != 0.0f) && ReflectFlag) //Trace only if reflection component is not 0 and the flag is set
            {
            PointRec R;
            Poly_Reflect(&R, Ray, Surface);        //Find the reflection
            RayTrace(&ReflectColor, &I, &R, Surface, Depth+1, Attenuation, false);
            }

         //-- Ray trace the refracted ray --
         if (Surface->Trans != 0.0f)               //Trace only for transparent surfaces
            {
            if (RefractFlag)                       //Do refraction if requested
               {
               PointRec T;
               Poly_Refract(&T, Ray, Surface, Inside); //Find the refraction
               RayTrace(&RefractColor, &I, &T, Surface, Depth+1, Attenuation, true);
               }
            else
               {
               RayTrace(&RefractColor, &I, Ray, Surface, Depth+1, Attenuation, false);
               }
            }
         }

      //======== Combine final colors ========
      //-- Combine transperacy when needed --
      if (Surface->Trans != 0.0f)
         {
         *LocalColor = *LocalColor * Surface->Opacity + RefractColor * Surface->Trans;
         }
   
      //-- Combine reflection when needed --
      if ((Surface->Reflect != 0.0f) && ReflectFlag)
         {
         *LocalColor = *LocalColor * (1.0f - Surface->Reflect) + ReflectColor * Surface->Reflect;
         }
      }


   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   RenderRayClass(void) 
      {
      Frame          = BitmapRec();
      Loc_EntityList = NULL; 
      Loc_LightList  = NULL;
      gl_ColorFmt    = 0;
      ColorScan      = NULL;
      LastColorScan  = NULL;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~RenderRayClass(void) 
      {
      Frame.DeleteData();
      if (ColorScan     != NULL) {delete[] ColorScan;     ColorScan     = NULL;}
      if (LastColorScan != NULL) {delete[] LastColorScan; LastColorScan = NULL;}
      }

   /*-------------------------------------------------------------------------
      Setup the renderer.
     ------------------------------------------------------------------------*/
   bool Initialize(VideoClass* Video, char* ProfCurve, float ProfEquLim, dword NewTabRes) 
      {
      //No NULL pointers please
      if (Video == NULL) {return false;}

      //Ensure that the Video uses the correct interface,
      // and the Video mode is valid.
      if ((Video->CurrentDevice != VIDEO_OPENGL) || !Video->ModeValid) {return false;}


      //-- Allocate a texture for the rendered image --
      Frame.DeleteData();

      switch(Video->BitsPerPixel)
         {
         case 32 : Frame.Flags = BMP_TYPE_RGBA; gl_ColorFmt = GL_RGBA; break;
         case 24 : Frame.Flags = BMP_TYPE_RGB;  gl_ColorFmt = GL_RGB;  break;
         case 16 : Frame.Flags = BMP_TYPE_RGB;  gl_ColorFmt = GL_RGB;  break;
         default : return false;
         }

      Frame.Flags |= BMP_TEXTURE;
      Frame.U_Res = Video->X_Res;
      Frame.V_Res = Video->Y_Res;
      Frame.FrameCount = 1;

      if (!Frame.AllocateData()) {return false;}
      if (!Frame.Check()) {return false;}

      
      //Allocate the color scan buffer for anti-aliasing
      ColorScan     = new float[Frame.U_Res];
      LastColorScan = new float[Frame.U_Res];
      if ((ColorScan == NULL) || (LastColorScan == NULL)) {return false;}

      
      //-- Compile the profile curve equation --
      if (ProfCurve != NULL)
         {
         str_replace(ProfEqu, ProfCurve);
         ProfCode = EquSolver.Compile(ProfEqu);
         if (ProfCode == NULL) {printf("RenderRayClass::Initialize( ): EquSolver.Compile( ) failed.\n\tPossible syntax error in profile curve equation.\n"); return false;}
         if (!EquSolver.Debug(ProfCode)) {return false;}
         ProfEquLimR = ProfEquLim;
         }

      //If there is no profile curve equation, then specify a plane
      else
         {
         str_replace(ProfEqu, "1;");
         ProfCode = EquSolver.Compile(ProfEqu);
         if (ProfCode == NULL) {printf("RenderRayClass::Initialize( ): EquSolver.Compile( ) failed.\n"); return false;}
         if (!EquSolver.Debug(ProfCode)) {return false;}
         ProfEquLimR = float_MAX;
         PCompFlag   = false;                         //Disable projetor compensation
         }


      //-- Setup other varaibles --
      CamApeture.X /= (float)Frame.U_Cent;            //Inegrate raster to image plane conversion with the camera apeture 
      CamApeture.Y /= (float)Frame.V_Cent;
      CamApeture.Z  = 1.0f / CamApeture.Z;
      RenderDone    = false;
      RenderValid   = true;                           //Indicate that rendering is allowed

      return true;
      }

   /*-------------------------------------------------------------------------
      Shutdown the renderer.
     ------------------------------------------------------------------------*/
   bool ShutDown(void) 
      {
      Frame.DeleteData();
      if (ProfEqu  != NULL) {delete[] ProfEqu;  ProfEqu  = NULL;}
      if (ProfCode != NULL) {delete[] ProfCode; ProfCode = NULL;}

      //Indicate that rendering is not allowed
      RenderValid = false;
      RenderDone  = false;

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
      if ((Video->CurrentDevice != VIDEO_OPENGL) || !Video->ModeValid || !RenderValid) {return false;}

      //If the display area is minimized, don't do any rendering
      if (Video->Minimized) {return true;}

      //Lock the renderer
      if (!Video->Lock()) {return false;}


      //---- Setup window so that coordinates it correspond to raster coords ----
      //Setup the 2D projection
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D((GLdouble)0.0, (GLdouble)Frame.U_Res, (GLdouble)Frame.V_Res, (GLdouble)0.0);

      //Setup the 2D model view matrix along with the OpenGL viewport area
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glViewport((GLint)0, (GLint)0, (GLsizei)Frame.U_Res, (GLsizei)Frame.V_Res); //Set the viewport size  


      //Simply copy the image to the screen if rendering is complete
      if (RenderDone) 
         {
         int U_Start = ((int)Video->X_Res - (int)Frame.U_Res) >> 1;
         int V_Start = ((int)Video->Y_Res - (int)Frame.V_Res) >> 1;
         if (U_Start < 0) {U_Start = 0;}
         if (V_Start < 0) {V_Start = 0;}

         byte* ScanLinePtr = Frame.FramePtr;
         for (int V = V_Start; V < (V_Start + (int)Frame.V_Res); V++)
            {
            glRasterPos2i((GLint)U_Start, (GLint)V);
            glDrawPixels((GLint)Frame.U_Res, (GLint)1, (GLenum)gl_ColorFmt, GL_UNSIGNED_BYTE, ScanLinePtr);
            ScanLinePtr += Frame.BytesPerLine;
            }

         //Restore the matrices
         glPopMatrix();
         glMatrixMode(GL_PROJECTION);
         glPopMatrix();

         return true;
         }


      //Save the Entity and Light list and World pointers to a local 
      // pointer to avoid parameter passing in RayTrace( ).
      Loc_EntityList = EntityList; 
      Loc_LightList  = LightList;
      Loc_World      = World;


      //If turn off anti-aliasing if sample count is < 2
      if ((AA_Samples < 2) && AntiAliasFlag) {AntiAliasFlag = false;}

      //Disable projetor compensation for standard perspective
      if (PCompFlag)
         {
         if (strcmp(ProfEqu, "1;") == 0) {PCompFlag = false;}
         }


      //-- Render the entire scene --
      byte*  PixelPtr = Frame.FramePtr;

      //Setup the timer functions
      SystemTimer.TS_DiffStart();

      //Do the loop
      int V;
      for (V = 0; V < (int)Frame.V_Res; V++)
         {
         float* ColorScanPtr     = ColorScan;
         float* LastColorScanPtr = LastColorScan;

         for (int U = 0; U < (int)Frame.U_Res; U++)
            {
            PointRec  Ray;
            ColorRec  Color;
            iColorRec iColor;

            //For the current projector view plane coordinate, find the incident ray
            Ray.X = (float)(U - (int)Frame.U_Cent) * CamApeture.X;
            Ray.Y = (float)((int)Frame.V_Cent - V) * CamApeture.Y;
            Ray.Z = 0.0f;  //Reset Z, becuse we compute Ray.Mag() later
            Ray.t = 0.0f;


            //---- Render if the radius is within the function bounds ----
            float r = sqrt(sqr(Ray.X) + sqr(Ray.Y));
            if (r <= ProfEquLimR)
               {
               //Projector compesation
               if (PCompFlag)
                  {
                  //Find the root for the projector vector
                  float r_inv = (r != 0.0f) ? (1.0 / r) : 1.0f;
                  float r_new = ProfCurve_FindRoot(fabs(POffset*r_inv), r, ProfCode, ProfEquLimR);
                  
                  //Compensate the initial ray
                  float t = r_new * r_inv;
                  Ray.X *= t;
                  Ray.Y *= t;
                  if (!EquSolver.Execute(Ray.Z, ProfCode, r_new)) {return false;}
                  Ray.Z *= CamApeture.Z;
                  }
               else
                  {
                  //Find the Z value according to the profile curve
                  if (!EquSolver.Execute(Ray.Z, ProfCode, r)) {return false;}
                  Ray.Z *= CamApeture.Z;
                  }

               //Rotate the ray vector according to the viewer's 
               // orientation, then convert to a unit vector.
               Ray = Ray.Rotate(Loc_World->VOrientation).Unit();

               //Do the ray tracing
               float Attenuation = 1.0f;
               RayTrace(&Color, &Loc_World->VOrigin, &Ray, NULL, 1, Attenuation, false);
            
               //Find the luminance of the color
               if (AntiAliasFlag)
                  {
                  *ColorScanPtr = 0.299f*Color.R + 0.587f*Color.G + 0.114f*Color.B;
            
                  float PrevLum_U, PrevLum_V;
                  if (V > 0) {PrevLum_V = *LastColorScanPtr;}
                  else {PrevLum_V = *ColorScanPtr;}
                  if (U > 0) {PrevLum_U = *(ColorScanPtr-1);}
                  else {PrevLum_U = *ColorScanPtr;}

                  //Begin anitialiasing if luminance difference is above the threshold
                  if ((fabs(PrevLum_U - *ColorScanPtr) > AA_Treshold) || 
                      (fabs(PrevLum_V - *ColorScanPtr) > AA_Treshold)) 
                     {
                     for (dword Sample = 0; Sample < AA_Samples-1; Sample++)
                        {
                        Attenuation = 1.0f;
                        ColorRec NewColor;
                        RayTrace(&NewColor, &Loc_World->VOrigin, &JitterRay(&Ray), NULL, 1, Attenuation, false);
                        Color += NewColor;
                        }
               
                     Color /= (float)(AA_Samples);
                     }
                  }

               //Convert float colors to integers (range 0 - 255)
               iColor = iColorRec((int)(Color.R * 255.0f), 
                                  (int)(Color.G * 255.0f), 
                                  (int)(Color.B * 255.0f), 0);
         
               //Saturate colors if necessay
               iColor = iColor.Saturate();
               }

            //---- Radius is outside the function bounds ----
            else
               {
               iColor = 0;
               }


            //Get the time delay in seconds and 
            // render hot spots only if requred
            if (HotspotFlag)
               {
               float expTime = 1.0f - exp(-10000.0f*SystemTimer.ReadTS_DiffSec());
               iColor = 255.0f * expTime;
               iColor = iColor.Saturate();
               }
            
            
            //Save RGBA colors
            Frame.Pixel->Write(PixelPtr, &iColor);
            
            
            //Advance to the next pixel
            ColorScanPtr++;
            LastColorScanPtr++;
            PixelPtr += Frame.BytesPerPixel;
            }


         //Update screen
         glRasterPos2i(0, V);
         glDrawPixels(Frame.U_Res, 1, (GLenum)gl_ColorFmt, GL_UNSIGNED_BYTE, (PixelPtr - Frame.BytesPerLine));
         Video->Refresh(false, 0.0f, 0.0f, 0.0f, 0.0f);


         float* TempPtr = ColorScan;
         ColorScan      = LastColorScan;
         LastColorScan  = TempPtr;
         }


      //---- Do a final re-display. ----
      int U_Start = ((int)Video->X_Res - (int)Frame.U_Res) >> 1;
      int V_Start = ((int)Video->Y_Res - (int)Frame.V_Res) >> 1;
      if (U_Start < 0) {U_Start = 0;}
      if (V_Start < 0) {V_Start = 0;}

      byte* ScanLinePtr = Frame.FramePtr;
      for (V = V_Start; V < (V_Start + (int)Frame.V_Res); V++)
         {
         glRasterPos2i((GLint)U_Start, (GLint)V);
         glDrawPixels((GLint)Frame.U_Res, (GLint)1, (GLenum)gl_ColorFmt, GL_UNSIGNED_BYTE, ScanLinePtr);
         ScanLinePtr += Frame.BytesPerLine;
         }

      //Restore the matrices
      glPopMatrix();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();

      RenderDone = true;

      //Unlock the renderer
      if (!Video->UnLock()) {return false;}

      return true;
      }



   /*==== End of Class =======================================================*/
   };



/*----------------------------------------------------------------------------
  Global declaration of the Video class.
  ----------------------------------------------------------------------------*/
RenderRayClass RenderRay;


/*==== End of file ===========================================================*/
#endif
