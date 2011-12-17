/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                          OpenGL Rendering Functions                        */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __RENDER_OPENGL_CPP__
#define __RENDER_OPENGL_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../_common/list.cpp"
#include "../render/render.cpp"
#include "../render/shade.cpp"
#include "../render/transform.cpp"
#include "../video_io/video.cpp"
#include "../mem_data/vertex.cpp"
#include "../mem_data/polygon.cpp"
#include "../mem_data/entity.cpp"
#include "../mem_data/world.cpp"


/*---------------------------------------------------------------------------
  The OpenGL rendering class.
  ---------------------------------------------------------------------------*/
class RenderOpenGLClass : public RenderClass, private ShadeClass, private TransformClass
   {
   /*==== Private Declarations ===============================================*/
   private:

   float        ModelViewMatrix[16];
   float        ProjMatrix[16];
   int          ViewPortMatrix[4];


   /*-------------------------------------------------------------------------
      Render the vertex normal of a Polygon.
     ------------------------------------------------------------------------*/
   inline bool RenderVertexNormal(PolygonRec* Polygon)
      {
      if (Polygon == NULL) {return false;}
      if (ProfCode != NULL) {return true;}

      SystemFlags.AccumPolys++;

      glBegin(GL_LINES); 
      
      for (int I = 0; I < POLY_PT_COUNT; I++)
         {
         glColor3f(0.0f, 0.5f, 0.0f);
         glVertex3fv((GLfloat*)&Polygon->Vertex[I]->Coord);   
         
         glColor3f(1.0f, 1.0f, 1.0f);
         glVertex3fv((GLfloat*)&(Polygon->Vertex[I]->Coord +
                                 Polygon->Vertex[I]->Normal.X * 0.25f));
         }
      
      glEnd(); 

      return true;
      }

   /*-------------------------------------------------------------------------
      Render a Gouraud shaded wire frame Polygon.
     ------------------------------------------------------------------------*/
   inline bool RenderWire(PolygonRec* Polygon)
      {
      if (Polygon == NULL) {return false;}

      SystemFlags.AccumPolys++;

      glBegin(GL_LINE_LOOP); 

      for (int I = 0; I < POLY_PT_COUNT; I++)
         {
         glColor3fv((GLfloat*)&Polygon->Shade[I]);
         glVertex3fv((GLfloat*)&Polygon->Vertex[I]->Coord);   
         }

      glEnd(); 

      return true;
      }

   /*-------------------------------------------------------------------------
      Render a Gouraud shaded Polygon.
     ------------------------------------------------------------------------*/
   inline bool RenderGouraud(PolygonRec* Polygon)
      {
      if (Polygon == NULL) {return false;}

      SystemFlags.AccumPolys++;

      glBegin(GL_TRIANGLES); 
      
      for (int I = 0; I < POLY_PT_COUNT; I++)
         {
         glColor3fv((GLfloat*)&Polygon->Shade[I]);
         glVertex3fv((GLfloat*)&Polygon->Vertex[I]->Coord);   
         }

      glEnd(); 
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Does a complete transformation of a point, taking the view coordinates 
      into account.
     ------------------------------------------------------------------------*/
   bool Transform(PointRec &tCoord, PointRec &Coord) 
      {
      //Translate/rotate first by using the model view matrix
      #define MV ModelViewMatrix
      tCoord.X = Coord.X*MV[0] + Coord.Y*MV[4] + Coord.Z*MV[8]  + MV[12];
      tCoord.Y = Coord.X*MV[1] + Coord.Y*MV[5] + Coord.Z*MV[9]  + MV[13];
      tCoord.Z = Coord.X*MV[2] + Coord.Y*MV[6] + Coord.Z*MV[10] + MV[14];
      tCoord.t = 0.0f;
      #undef MV

      //Transform the point, and increment counter if the point is invisible
      bool Visible;
      if (PCompFlag)
         {
         //Transform with projector compensation
         Visible = ProfCurve_Transform(tCoord, tCoord, POffset);
         }
      else
         {
         //Transform with NO projector compensation
         Visible = ProfCurve_Transform(tCoord, tCoord);
         }

      return Visible;
      }

   /*-------------------------------------------------------------------------
      Recursivaley subdivide the polygon.
     ------------------------------------------------------------------------*/
   void DivideRecurse(VertexRec* *BaseMesh, dword LOD)
      {
      //-- Transform the vertex data --
      dword CullCount = 0;
      for (int I = 0; I < POLY_PT_COUNT; I++)
         {
         //Do the point transformation for fresh points
         if (!BaseMesh[I]->TransFlag)
            {BaseMesh[I]->Visisble = Transform(BaseMesh[I]->tCoord, BaseMesh[I]->Coord);}
        
         if (!BaseMesh[I]->Visisble) {CullCount++;}
         }

      //Exit if all the points are invisible
      if (CullCount >= POLY_PT_COUNT) {return;}

      //-- Calculate the magnitude squared for the edges, 
      //   and set the subdivision flag if necessary --
      bool DoSubdivJob = false;
      for (I = 0; I < POLY_PT_COUNT; I++)
         {
         int I_Next = (I < POLY_PT_COUNT-1) ? (I+1) : 0;
         float MagSqr = sqr(BaseMesh[I_Next]->tCoord.X - BaseMesh[I]->tCoord.X) + 
                        sqr(BaseMesh[I_Next]->tCoord.Y - BaseMesh[I]->tCoord.Y);
         if (MagSqr > SubdivTresh) {DoSubdivJob = true; break;}
         }


      //-- Do some final perspective compesation --
//      for (I = 0; I < POLY_PT_COUNT; I++) {BaseMesh[I]->tCoord = Compensate(BaseMesh[I]->tCoord);}


      //-- Divide further if required --
      if (DoSubdivJob && (LOD < Max_LOD))
         {
         VertexRec  NewMeshData[POLY_PT_COUNT];
         VertexRec* NewMesh[POLY_PT_COUNT];
         
         for (int I = 0; I < POLY_PT_COUNT; I++)
            {
            //Find midpoint data
            int I_Next = (I < POLY_PT_COUNT-1) ? (I+1) : 0;
            NewMeshData[I].Coord     = (BaseMesh[I]->Coord  + BaseMesh[I_Next]->Coord)  * 0.5f;
            NewMeshData[I].Shade     = (BaseMesh[I]->Shade  + BaseMesh[I_Next]->Shade)  * 0.5f;
            NewMeshData[I].TransFlag = false;
            NewMeshData[I].Visisble  = true;

//if (LOD == 0) NewMeshData[I].Shade = 1;
//else NewMeshData[I].Shade = 0;
            }

         //Wire mesh A
         NewMesh[0] = BaseMesh[0];
         NewMesh[1] = &NewMeshData[0];
         NewMesh[2] = &NewMeshData[2];
         DivideRecurse(NewMesh, LOD+1);

         //Wire mesh B
         NewMesh[0] = &NewMeshData[0];
         NewMesh[1] = BaseMesh[1];
         NewMesh[2] = &NewMeshData[1];
         DivideRecurse(NewMesh, LOD+1);

         //Wire mesh C
         NewMesh[0] = &NewMeshData[2];
         NewMesh[1] = &NewMeshData[1];
         NewMesh[2] = BaseMesh[2];
         DivideRecurse(NewMesh, LOD+1);

         //Wire mesh D
         NewMesh[0] = &NewMeshData[0];
         NewMesh[1] = &NewMeshData[1];
         NewMesh[2] = &NewMeshData[2];
         DivideRecurse(NewMesh, LOD+1);
         }

      //Do the rendering
      else
         {
         if (WireFrame) {glBegin(GL_LINE_LOOP);}
         else           {glBegin(GL_TRIANGLES);}

         SystemFlags.AccumPolys++;

         for (int I = 0; I < POLY_PT_COUNT; I++)
            {
            glColor3fv((GLfloat*)&BaseMesh[I]->Shade);
            glVertex3fv((GLfloat*)&BaseMesh[I]->tCoord);
            }
        
         glEnd(); 
         }
      }

   /*-------------------------------------------------------------------------
      This function transforms a Polygon then subdivides it (if necessary), 
      and renders the result. Returns true on success.
     ------------------------------------------------------------------------*/
   bool RenderPolygon(PolygonRec* Polygon, WorldRec* World)
      {
      if (Polygon == NULL) {return false;}
      
      //---- Do some point transformations if the profile curve is defined ----
      if (ProfCode != NULL)
         {
         VertexRec  BaseMeshData[POLY_PT_COUNT];
         VertexRec* BaseMesh[POLY_PT_COUNT];
         int        I;
      
         //-- Setup the initial polygon vertex data --
         for (I = 0; I < POLY_PT_COUNT; I++)
            {
            BaseMeshData[I].Coord     = Polygon->Vertex[I]->Coord;
            BaseMeshData[I].Shade     = Polygon->Shade[I];
            BaseMeshData[I].TransFlag = false;
            BaseMeshData[I].Visisble  = true;
            BaseMesh[I]               = &BaseMeshData[I];
            }

         DivideRecurse(BaseMesh, 0);

         return true;
         }


      //---- No profile curve transforms ----
      bool Status = true;
      //Status &= RenderVertexNormal(Polygon);
      if (WireFrame) {Status &= RenderWire(Polygon);}
      else           {Status &= RenderGouraud(Polygon);}
      return Status;
      }

   /*-------------------------------------------------------------------------
      Recursively computes the shading colors and then renders an Entity list.

      EntityList  : The list of Entities and sub-Entities to render. This 
                    pointer can be NULL (empty list).
      LightList   : List of Ligths. This pointer can be NULL (empty list).
      World       : Access to the world data such as view origin and 
                    orientation, etc.
     ------------------------------------------------------------------------*/
   bool RenderShadeEntity(ListRec* EntityList, ListRec* LightList, WorldRec* World) 
      {
      //---- Render each Entity ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         #define Entity ((EntityRec*)EntityNode->Data)
         if (Entity == NULL) {return false;}

         //Determine the if shading is required
         bool ShadeFlag = ((Entity->Flags & ENTITY_SHADE) != ENTITY_NULL);
         if (ShadeFlag)
            {
            //Process every vertex
            ListRec* VertexNode = Entity->VertexList;
            while (VertexNode != NULL)
               {
               //Find the diffuse light color for this vertex
               if (!ShadeGouraud((VertexRec*)VertexNode->Data, LightList)) {return false;}
               VertexNode = VertexNode->Next;
               }
            }

         //Render each Polygon in the Entity
         ListRec* PolygonNode = Entity->PolygonList;
         while (PolygonNode != NULL)
            {
            //Find the rendering color for this Polygon
            if (ShadeFlag) 
               {
               if (!ShadeGouraud((PolygonRec*)PolygonNode->Data, LightList)) {return false;}
               }
            /*//Shade flag debug
            else
               {
               ((PolygonRec*)PolygonNode->Data)->Shade[0] = 
               ((PolygonRec*)PolygonNode->Data)->Shade[1] = 
               ((PolygonRec*)PolygonNode->Data)->Shade[2] = 1.0f;
               }/**/

            //Render the Polygon
            if (!RenderPolygon((PolygonRec*)PolygonNode->Data, World)) {return false;}
            PolygonNode = PolygonNode->Next;
            }

         //Recursively render the sub-Entities
         if (!RenderShadeEntity(Entity->EntityList, LightList, World)) {return false;}


         //Advance to the next Entity node
         EntityNode = EntityNode->Next;
         #undef Entity
         }
      
      return true;
      }


   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Constructor --------------------------------------------------------*/
   RenderOpenGLClass(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~RenderOpenGLClass(void) {}

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


      //Compile the profile curve equation
      if (ProfCurve != NULL)
         {
         str_append(ProfEqu, ProfCurve);
         ProfCode = EquSolver.Compile(ProfEqu);
         if (ProfCode == NULL) {printf("RenderOpenGLClass::Initialize( ): EquSolver.Compile( ) failed.\n\tPossible syntax error in profile curve equation.\n"); return false;}
         if (!EquSolver.Debug(ProfCode)) {return false;}
         ProfEquLimR = ProfEquLim;

         //Setup the transformation table
         if (!InitTable(Video, ProfCode, ProfEquLim, NewTabRes))
            {printf("RenderOpenGLClass::Initialize( ): TransformClass::InitTable( ) failed.\n"); return false;}
         }

      //Setup other data members
      CamApeture.X = Video->XY_Ratio / CamApeture.X;
      CamApeture.Y = 1.0f / CamApeture.Y;

      RenderValid = true;           //Indicate that rendering is allowed

      return true;
      }

   /*-------------------------------------------------------------------------
      Shutdown the renderer.
     ------------------------------------------------------------------------*/
   bool ShutDown(void) 
      {
      if (ProfEqu  != NULL) {delete[] ProfEqu;  ProfEqu  = NULL;}
      if (ProfCode != NULL) {delete[] ProfCode; ProfCode = NULL;}

      //Indicate that rendering is not allowed
      RenderValid = false;

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

      //Enable the depth buffer (if required)
      bool ToggleDepthTest = (glIsEnabled(GL_DEPTH_TEST) == GL_FALSE);
      if (ToggleDepthTest) {glEnable(GL_DEPTH_TEST);}

      //Enable anti aliasing
      if (AntiAliasFlag)
         {
         glEnable(GL_POINT_SMOOTH); 
         glEnable(GL_LINE_SMOOTH); 
         glEnable(GL_POLYGON_SMOOTH); 
         glEnable(GL_BLEND); 
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
         }

      //-- Use OpenGL to do the transformation for perspective views --
      if (ProfCode == NULL)
         {
         glMatrixMode(GL_MODELVIEW);
         glPushMatrix();                                   
         glRotatef(rad2deg(World->VOrientation.X), -1.0f,  0.0f,  0.0f);
         glRotatef(rad2deg(World->VOrientation.Y),  0.0f, -1.0f,  0.0f);
         glRotatef(rad2deg(World->VOrientation.Z),  0.0f,  0.0f, -1.0f);
         glTranslatef(-World->VOrigin.X, -World->VOrigin.Y, -World->VOrigin.Z);
         glScalef(CamApeture.X, CamApeture.Y, CamApeture.Z);
         }

      //-- In panoramic mode, use OpenGL to compute the transformation matrices --
      else
         {
         glMatrixMode(GL_PROJECTION);
         glPushMatrix();
         glLoadIdentity();
         glOrtho(-Video->XY_Ratio, Video->XY_Ratio, -1.0f, 1.0f, -1.0f, 100.0f);

         glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
         glRotatef(rad2deg(World->VOrientation.X), 1.0f,  0.0f,  0.0f);
         glRotatef(rad2deg(World->VOrientation.Y), 0.0f,  1.0f,  0.0f);
         glRotatef(rad2deg(World->VOrientation.Z), 0.0f,  0.0f, -1.0f);
         glTranslatef(-World->VOrigin.X, -World->VOrigin.Y, World->VOrigin.Z);
         glScalef(CamApeture.X, CamApeture.Y, -CamApeture.Z);
         
         //Store the matices
         glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&ModelViewMatrix);
         glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&ProjMatrix);
         glGetIntegerv(GL_VIEWPORT, (GLint*)&ViewPortMatrix);
         glMatrixMode(GL_MODELVIEW); 
         glPopMatrix();
         }

      //-- Shade and render the Entity list --
      if (!RenderShadeEntity(EntityList, LightList, World)) {return false;}

      //Pop the transformation matrix (if required)
      if (ProfCode == NULL) {glMatrixMode(GL_MODELVIEW); glPopMatrix();}
      else {glMatrixMode(GL_PROJECTION); glPopMatrix();}

      //Disable anti aliasing
      if (AntiAliasFlag)
         {
         glDisable(GL_BLEND); 
         glDisable(GL_POLYGON_SMOOTH); 
         glDisable(GL_LINE_SMOOTH); 
         glDisable(GL_POINT_SMOOTH); 
         }

      //Disable the depth buffer (if required)
      if (ToggleDepthTest) {glDisable(GL_DEPTH_TEST);}

      //Unlock the renderer
      if (!Video->UnLock()) {return false;}

      return true;
      }


   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global declaration of the class.
  ----------------------------------------------------------------------------*/
RenderOpenGLClass RenderOpenGL;


/*==== End of file ===========================================================*/
#endif
