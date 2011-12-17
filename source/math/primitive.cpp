/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*               Create Primitives, like Cubes, Planes, etc.                  */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __PRIMITIVE_CPP__
#define __PRIMITIVE_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../math/equsolver.cpp"
#include "../mem_data/vertex.cpp"
#include "../mem_data/polygon.cpp"
#include "../mem_data/entity.cpp"


/*---------------------------------------------------------------------------
  The Primitive class.
  ---------------------------------------------------------------------------*/
class PrimitiveClass
   {
   /*==== Private Declarations ===============================================*/
   private:

   /*---- Private Data -------------------------------------------------------*/
   struct GridRec
      {
      VertexRec*  *Array;                             //Vertex grid array
      dword        VCount;                            //Number of vertices in the grid
      dword        VPerLine;                          //Number of vertices per line
      iTexPointRec Index;                             //General purpose grid indexing variables
      };

   /*-------------------------------------------------------------------------
      Allocates Grid data. Returns true on success.

      NewGrid : The structure to initialize.
      GridRes : Resolution of the grid.
     ------------------------------------------------------------------------*/
   bool AllocateGridData(GridRec* NewGrid, iTexPointRec* GridRes)
      {
      if ((NewGrid == NULL) || (GridRes == NULL)) {return false;}

      //Find the grid array size
      NewGrid->VPerLine = GridRes->U + 1;
      NewGrid->VCount   = NewGrid->VPerLine * (GridRes->V + 1);
      if (NewGrid->VCount == 0) {return false;}

      //Allocate the grid
      NewGrid->Array = new VertexRec*[NewGrid->VCount];
      if (NewGrid->Array == NULL) {return false;}

      //Reset other members
      NewGrid->Index = 0;

      return true;
      }

   /*-------------------------------------------------------------------------
      Deletes all the Grid data.
     ------------------------------------------------------------------------*/
   void DeleteGridData(GridRec* OldGrid)
      {
      if (OldGrid == NULL) {return;}

      if (OldGrid->Array != NULL) {delete[] OldGrid->Array;}
      OldGrid->Array    = NULL;
      OldGrid->VCount   = 0;
      OldGrid->VPerLine = 0;
      OldGrid->Index    = 0;
      }


   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   PrimitiveClass(void) {}

   /*---- Destructor ---------------------------------------------------------*/
   ~PrimitiveClass(void) {}

   /*-------------------------------------------------------------------------
      Creates a new plane. The resulting plane will face upwards, basically the 
      normals of each polygon will run parallel with the Y-axis. Returns a new 
      entity pointer on success. Returns NULL on fail.

      GridRes  : Number of divisions in the U and V directions. If any of them 
                 is 0, the function returns NULL.
      Size     : Plane size in [X,Z] world dimensions. If any of them is 0.0, 
                 the function returns NULL.
      Position : The final [X,Y,Z] position of the plane in the world space.
      kDiff    : Diffuse color of the plane, [R,G,B], range 0.0 to 1.0.
     ------------------------------------------------------------------------*/
   EntityRec* Plane(iTexPointRec* GridRes, PointRec* Size, PointRec* Position, ColorRec* kDiff)
      {
      //-- See if the arguments are valid --
      if ((GridRes == NULL) || (Size == NULL)    || (kDiff == NULL)) {return NULL;}
      if ((GridRes->U == 0) || (GridRes->V == 0) || (Size->X == 0.0f) || (Size->Z == 0.0f)) {return NULL;}


      //-- Allocate the grid --
      GridRec Grid;
      if (!AllocateGridData(&Grid, GridRes)) {return NULL;}
  
      //-- Create a new Entity --
      EntityRec* NewEntity = new EntityRec;
      if (NewEntity == NULL) {DeleteGridData(&Grid); return NULL;}


      //Polygon colors
      ColorRec kAmb(0.0f, 0.0f, 0.0f, 0.0f);
      ColorRec kSpec(0.3f, 0.3f, 0.3f, 0.0f);
      float    nSpec   = 75.0f;
      float    Reflect = 0.0f;
      float    Trans   = 0.0f;
      float    IdxRefr = 1.0f;
  

      //Setup the counters
      PointRec Coord, dCoord;
      Coord.X  = -Size->X * 0.5f;
      Coord.Z  = -Size->Z * 0.5f;
      Coord.Y  =  Coord.t = 0.0f;
      dCoord.X =  Size->X / (float)GridRes->U;
      dCoord.Z =  Size->Z / (float)GridRes->V;
      dCoord.Y =  dCoord.t = 0.0f;
      
      //---- Allocate Vertices in the entire grid and instert them into a list ----
      for (Grid.Index.V = 0; Grid.Index.V <= GridRes->V; Grid.Index.V++)
         {
         for (Grid.Index.U = 0; Grid.Index.U <= GridRes->U; Grid.Index.U++)
            {
            //Allocate a new Vertex
            VertexRec* Vertex = new VertexRec;
            if (Vertex == NULL) 
               {
               DeleteGridData(&Grid);
               delete NewEntity; 
               return NULL;
               }

            //Setup the Vertex, at the same time translate it to position
            Vertex->Coord = Coord + *Position;
         
            //Insert the vertex into the Grid and VertexList of Entity
            Grid.Array[Grid.Index.V*Grid.VPerLine + Grid.Index.U] = Vertex;
            if (!LinkedList.Insert(NewEntity->VertexList, Vertex))
               {
               DeleteGridData(&Grid);
               delete Vertex;
               delete NewEntity; 
               return NULL;
               }

            Coord.X += dCoord.X;
            }
      
         Coord.X = -Size->X * 0.5f;
         Coord.Z += dCoord.Z;
         }


      //---- Create and setup the Polygons ----
      for (Grid.Index.V = 0; Grid.Index.V < GridRes->V; Grid.Index.V++)
         {
         for (Grid.Index.U = 0; Grid.Index.U < GridRes->U; Grid.Index.U++)
            {
            //-- Allocate a new Polygon1 --
            PolygonRec* Polygon = new PolygonRec;
            if (Polygon == NULL)
               {
               DeleteGridData(&Grid);
               delete NewEntity; 
               return NULL;
               }
            
            //Setup the Polygon1 (triangulate clock wise)
            Polygon->Vertex[2] = Grid.Array[Grid.Index.V*Grid.VPerLine     + Grid.Index.U];   //Top left
            Polygon->Vertex[1] = Grid.Array[Grid.Index.V*Grid.VPerLine     + Grid.Index.U+1]; //Top rigt
            Polygon->Vertex[0] = Grid.Array[(Grid.Index.V+1)*Grid.VPerLine + Grid.Index.U];   //Bottom left

            //Setup the color
            Polygon->kAmb     = kAmb;
            Polygon->kDiff    = *kDiff;
            Polygon->kSpec    = kSpec;
            Polygon->nSpec    = nSpec;
            Polygon->Reflect  = Reflect;
            Polygon->Trans    = Trans;
            Polygon->Opacity  = 1.0f - Trans;
            Polygon->IdxRefr  = IdxRefr;
            Polygon->Facet[0] = Polygon->Facet[1] = Polygon->Facet[2] = true;


            //Insert the Polygon1 into the PolygonList of Entity
            if (!LinkedList.Insert(NewEntity->PolygonList, Polygon)) 
               {
               DeleteGridData(&Grid);
               delete Polygon;
               delete NewEntity; 
               return NULL;
               }


            //-- Allocate a new Polygon2 --
            Polygon = new PolygonRec;
            if (Polygon == NULL)
               {
               DeleteGridData(&Grid);
               delete NewEntity; 
               return NULL;
               }
            
            //Setup the Polygon2 (triangulate clock wise)
            Polygon->Vertex[2] = Grid.Array[Grid.Index.V*Grid.VPerLine     + Grid.Index.U+1]; //Top rigt
            Polygon->Vertex[1] = Grid.Array[(Grid.Index.V+1)*Grid.VPerLine + Grid.Index.U+1]; //Bottom rigt
            Polygon->Vertex[0] = Grid.Array[(Grid.Index.V+1)*Grid.VPerLine + Grid.Index.U];   //Bottom left

            //Setup the color
            Polygon->kAmb     = kAmb;
            Polygon->kDiff    = *kDiff;
            Polygon->kSpec    = kSpec;
            Polygon->nSpec    = nSpec;
            Polygon->Reflect  = Reflect;
            Polygon->Trans    = Trans;
            Polygon->Opacity  = 1.0f - Trans;
            Polygon->IdxRefr  = IdxRefr;
            Polygon->Facet[0] = Polygon->Facet[1] = Polygon->Facet[2] = true;

            //Insert the Polygon2 into the PolygonList of Entity
            if (!LinkedList.Insert(NewEntity->PolygonList, Polygon))
               {
               DeleteGridData(&Grid);
               delete Polygon;
               delete NewEntity; 
               return NULL;
               }
            }
         }

      
      //-- If got here, the temp data can be safely deleted --
      DeleteGridData(&Grid);

      //-- Setup other attributes --
      PointRec Sum;
      if (!NewEntity->CalcNormals(false))    {delete NewEntity; return NULL;} //Set UseFacetFlags to false, coz we want vertex normals as well
      if (!NewEntity->FindCentroid(0, &Sum)) {delete NewEntity; return NULL;}
      if (!NewEntity->FindBoundingBox(true)) {delete NewEntity; return NULL;}

      return NewEntity;
      }

   /*----------------------------------------------------------------------------
      Create a new cube or recatangle, with 12 polygons. The layout of the cube
      is illutrated below. The function returns a new entity pointer on success.
      Returns NULL on fail.

                0------1
               /.     /|
              / .    / |
             3------2  |
             |  4...|..5
             | .    | /
             |.     |/
             7------6

      Size     : Cube size in [X,Y,Z] world dimensions. If any of them is 0.0, 
                 the function returns NULL.
      Position : The final [X,Y,Z] position of the cube in the world space.
      kDiff    : Diffuse color of the cube, [R,G,B], range 0.0 to 1.0.
     ----------------------------------------------------------------------------*/
   EntityRec* Cube(PointRec* Size, PointRec* Position, ColorRec* kDiff)
      {
      //-- See if the arguments are valid --
      if ((Size    == NULL) || (kDiff   == NULL)) {return NULL;}
      if ((Size->X == 0.0f) || (Size->Y == 0.0f) || (Size->Z == 0.0f)) {return NULL;}

      
      //Find the minimum and maximum cube corners, and position it at the same
      // time.
      PointRec Min = *Size * -0.5f;
      PointRec Max = -Min;
      Min += *Position;
      Max += *Position;
   
      
      //-- Allocate the grid --
      GridRec      Grid;
      iTexPointRec GridRes(1, 8, 0, 0);
      if (!AllocateGridData(&Grid, &GridRes)) {return NULL;}
   
      //-- Create a new Entity --
      EntityRec* NewEntity = new EntityRec;
      if (NewEntity == NULL) {DeleteGridData(&Grid); return NULL;}

      
      //Polygon colors
      ColorRec kAmb(0.0f, 0.0f, 0.0f, 0.0f);
      ColorRec kSpec(0.7f, 0.7f, 0.7f, 0.0f);
      float    nSpec   = 300.0f;
      float    Reflect = 0.3f;
      float    Trans   = 0.0f;
      float    IdxRefr = 1.0f;


      //---- Create the vertices ----
      int Incr;
      for (Incr = 0; Incr < 8; Incr++) 
         {
         //Allocate cube the Vertices
         Grid.Array[Incr] = new VertexRec;
         if (Grid.Array[Incr] == NULL) 
            {
            delete NewEntity; 
            DeleteGridData(&Grid);
            return NULL;
            }

         //Insert the Vertices in to the list
         if (!LinkedList.Insert(NewEntity->VertexList, Grid.Array[Incr])) 
            {
            delete NewEntity; 
            delete Grid.Array[Incr];
            DeleteGridData(&Grid);
            return NULL;
            }

         //-- Process X coordinates for the box --
         if ((Incr == 1) || (Incr == 2) || (Incr == 5) || (Incr == 6)) //Right side of the bounding cube gets Max.X
            {Grid.Array[Incr]->Coord.X = Max.X;}
         else {Grid.Array[Incr]->Coord.X = Min.X;}                //Left side of the cube gets Min.X

         //-- Process Y coordinates for the box --
         if (Incr < 4)                                            //Top side of the cube gets Max.Y
            {Grid.Array[Incr]->Coord.Y = Max.Y;}
         else {Grid.Array[Incr]->Coord.Y = Min.Y;}                //Bottom side of the cube gets Min.Y

         //-- Process Z coordinates for the box --
         if ((Incr == 0) || (Incr == 1) || (Incr == 4) || (Incr == 5)) //Back side of the cube gets Max.Z
            {Grid.Array[Incr]->Coord.Z = Max.Z;}
         else {Grid.Array[Incr]->Coord.Z = Min.Z;}                //Front side of the cube gets Min.Z
         }


      //---- Polygons ----
      for (Incr = 0; Incr < 6; Incr++) 
         {
         dword P0, P1, P2, P3;

         //Allocate Polygon A
         PolygonRec* Polygon_A = new PolygonRec;
         PolygonRec* Polygon_B = new PolygonRec;
         if ((Polygon_A == NULL) || (Polygon_B == NULL))
            {
            delete NewEntity; 
            delete Polygon_A;
            delete Polygon_B;
            DeleteGridData(&Grid);
            return NULL;
            }
         
         //Allocate and setup Polygon A
         if (!LinkedList.Insert(NewEntity->PolygonList, Polygon_A)) 
            {
            delete NewEntity; 
            delete Polygon_A;
            DeleteGridData(&Grid);
            return NULL;
            }

         //Allocate and setup Polygon B
         if (!LinkedList.Insert(NewEntity->PolygonList, Polygon_B)) 
            {
            delete NewEntity; 
            delete Polygon_B;
            DeleteGridData(&Grid);
            return NULL;
            }

         //Setup Vertex indeces for every cube face
         if (Incr == 0)      {P0 = 3; P1 = 0; P2 = 1; P3 = 2;}
         else if (Incr == 1) {P0 = 6; P1 = 5; P2 = 4; P3 = 7;}
         else if (Incr == 2) {P0 = 6; P1 = 2; P2 = 1; P3 = 5;}
         else if (Incr == 3) {P0 = 5; P1 = 1; P2 = 0; P3 = 4;}
         else if (Incr == 4) {P0 = 4; P1 = 0; P2 = 3; P3 = 7;}
         else if (Incr == 5) {P0 = 7; P1 = 3; P2 = 2; P3 = 6;}
      
         //Triangulate the Polygons
         Polygon_A->Vertex[0] = Grid.Array[P0];
         Polygon_A->Vertex[1] = Grid.Array[P1];
         Polygon_A->Vertex[2] = Grid.Array[P2];

         Polygon_B->Vertex[0] = Grid.Array[P0];
         Polygon_B->Vertex[1] = Grid.Array[P2];
         Polygon_B->Vertex[2] = Grid.Array[P3];

         //Set the Polygon color
         Polygon_A->kAmb    = Polygon_B->kAmb    = kAmb;
         Polygon_A->kDiff   = Polygon_B->kDiff   = *kDiff;
         Polygon_A->kSpec   = Polygon_B->kSpec   = kSpec;
         Polygon_A->nSpec   = Polygon_B->nSpec   = nSpec;
         Polygon_A->Reflect = Polygon_B->Reflect = Reflect;
         Polygon_A->Trans   = Polygon_B->Trans   = Trans;
         Polygon_A->Opacity = Polygon_B->Opacity = 1.0f - Trans;
         Polygon_A->IdxRefr = Polygon_B->IdxRefr = IdxRefr;
        
         Polygon_A->Facet[0] = Polygon_A->Facet[1] = Polygon_A->Facet[2] = true;
         Polygon_B->Facet[0] = Polygon_B->Facet[1] = Polygon_B->Facet[2] = true;
         }

      //-- If got here, the temp data can be safely deleted --
      DeleteGridData(&Grid);

      //-- Compute it's attributes --
      PointRec Sum;
      if (!NewEntity->CalcNormals(false))    {delete NewEntity; return NULL;} //Set UseFacetFlags to false, coz we want vertex normals as well
      if (!NewEntity->FindCentroid(0, &Sum)) {delete NewEntity; return NULL;}
      if (!NewEntity->FindBoundingBox(true)) {delete NewEntity; return NULL;}

      return NewEntity;
      }

   /*-------------------------------------------------------------------------
      Convert a profile curve equation to a surface. All the texture 
      coordinates are also generated.

      ProfEqu     : Profile curve equation string.
      ProfEquLim  : Profile cure limit.
      ProjComp    : If set, the projector viewpoint is taken into consideration.
      d           : Projector's offset.
      XY_Ratio    : Image plane aspect ratio.
     ------------------------------------------------------------------------*/
   EntityRec* Surface(char* ProfEqu, float ProfEquLim, bool ProjComp, float d, float XY_Ratio)
      {
      //Local stuff
      float        Scale;
      float        Scale_inv;
      float        PlaneSize = 20.0f;
      float        PlaneSize_half_inv = 1.0f / PlaneSize;
      iTexPointRec GridRes(50, 50, 0, 0);
      PointRec     Size(PlaneSize, 0.0f, PlaneSize, 0.0f);
      PointRec     Position = 0.0f;
      PointRec     Sum;
      ColorRec     kDiff = 0.5f;

      byte*        ProfCode    = NULL;
      EntityRec*   Surface     = NULL;
      ListRec*     VertexNode  = NULL;
      ListRec*     PolygonNode = NULL;


      XY_Ratio *= 0.5f;


      //-- Compile the profile curve equation --
      if (ProfEqu == NULL) 
         {
         ProfEqu    = "1;";
         ProfEquLim = 10.0f;
         }
      
      ProfCode = EquSolver.Compile(ProfEqu);
      if (ProfCode == NULL) {printf("PrimitiveClass::Surface( ): EquSolver.Compile( ) failed.\n\tPossible syntax error in profile curve equation.\n"); goto _ExitError;}
      if (!EquSolver.Debug(ProfCode)) {goto _ExitError;}

      
      //-- Create the base plane --
      Scale     = (ProfEquLim != 0.0f) ? (PlaneSize * 0.5f) / ProfEquLim : PlaneSize; 
      Scale_inv = 1.0f / Scale;

      Surface = Plane(&GridRes, &Size, &Position, &kDiff);
      if (Surface == NULL) {goto _ExitError;}
      
      //Rotate the base plane 
      Surface->Rotate(&PointRec(deg2rad(-90.0f), 0.0f, 0.0f, 0.0f), &Surface->Centroid);

      //Re center the surface
      if (!Surface->FindCentroid(0, &Sum)) {goto _ExitError;}
      Surface->Translate(&-Surface->Centroid);



      //---- Generate the curved surface by computing the Z displacement ----
      VertexNode = Surface->VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {goto _ExitError;}

         float R = sqrt(sqr(Vertex->Coord.X) + sqr(Vertex->Coord.Y)) * Scale_inv;
         if (ProfEquLim < R) {R = ProfEquLim;}
            
         EquSolver.Execute(Vertex->Coord.Z, ProfCode, R);
         Vertex->Coord.Z *= Scale;
         Vertex->Coord.t = R;
         
         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }

      //---- Generate the texture coordinates ----
      PolygonNode = Surface->PolygonList;
      while (PolygonNode != NULL)
         {
         #define Polygon ((PolygonRec*)PolygonNode->Data)
         if (Polygon == NULL) {return false;}

         for (int I = 0; I < POLY_PT_COUNT; I++)
            {
            Polygon->TexCoord[I].U = Polygon->Vertex[I]->Coord.X * XY_Ratio;
            Polygon->TexCoord[I].V = Polygon->Vertex[I]->Coord.Y;
            Polygon->TexCoord[I].W = Polygon->TexCoord[I].t = 0.0f;
            
            if (ProjComp)
               {
               float Z = Polygon->Vertex[I]->Coord.Z * Scale_inv;
               float t = (Z > d) ? fabs(d / (Z - d)) : 1.0f;
               Polygon->TexCoord[I] = Polygon->TexCoord[I] * t;
               }
            
            //Polygon->TexCoord[I] = ((Polygon->TexCoord[I] + PlaneSize * 0.5f) * PlaneSize_half_inv);
            Polygon->TexCoord[I] *= PlaneSize_half_inv;
            Polygon->TexCoord[I] *= ProfEquLim;

            if      (Polygon->TexCoord[I].U >  XY_Ratio) {Polygon->TexCoord[I].U =  XY_Ratio;}
            else if (Polygon->TexCoord[I].U < -XY_Ratio) {Polygon->TexCoord[I].U = -XY_Ratio;}
            if      (Polygon->TexCoord[I].V >  0.5f) {Polygon->TexCoord[I].V =  0.5f;}
            else if (Polygon->TexCoord[I].V < -0.5f) {Polygon->TexCoord[I].V = -0.5f;}

            Polygon->TexCoord[I] += 0.5f;
            }
        
         //Advance to the next Polygon
         PolygonNode = PolygonNode->Next;
         #undef Polygon
         }
   

      //-- Normal exit --
      if (ProfCode != NULL) {delete[] ProfCode;}
      return Surface;


      //-- Exit with error --
      _ExitError:
      if (Surface  != NULL) {delete Surface;}
      if (ProfCode != NULL) {delete[] ProfCode;}
      return NULL;
      }




   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
PrimitiveClass Primitive;



/*==== End of file ===========================================================*/
#endif