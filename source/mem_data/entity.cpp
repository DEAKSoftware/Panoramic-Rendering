/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __ENTITY_CPP__
#define __ENTITY_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../_common/list.cpp"
#include "../mem_data/vertex.cpp"
#include "../math/mathcnst.h"


/*----------------------------------------------------------------------------
   Entity data flags.
  ----------------------------------------------------------------------------*/
#define ENTITY_NULL        0x00000000     //Undefined entity

//Entity type (bits 0 to 1), non flags, only one is valid at a time
#define ENTITY_OBJECT      0x00000001     //Non-living object
#define ENTITY_PLANT       0x00000002     //Living plant
#define ENTITY_CREATURE    0x00000003     //Living creature
#define ENTITY_THE_TING    0x00000004     //???
#define ENTITY_TYPE_MASK   0x0000000F     //Mask for the TYPE bits

//Entity properties (bits 2 to 3), flags, may have several bits set at a time
#define ENTITY_PARTICLE    0x00000010     //It has particle behaviour
#define ENTITY_BOID        0x00000020     //It has boid (flocking) behaviour
#define ENTITY_LSYSTEM     0x00000040     //It has an L-System property
#define ENTITY_PROP_MASK   0x000000F0     //Mask for the PROPERTY bits

//Other flags
#define ENTITY_SHADE       0x00000100     //Flag to indicate that shading is required

#define ENTITY_BV_COUNT    8              //Bounding volume vertex count
#define ENTITY_BVFACE_COUNT 6              //Bounding volume face count


/*---------------------------------------------------------------------------
  The Entity class.
  ---------------------------------------------------------------------------*/
class EntityRec
   {
   /*==== Private Declarations ===============================================*/
   private:


   /*==== Public Declarations ================================================*/
   public:

   /*---- Public Data --------------------------------------------------------*/
   dword      Flags;                      //Flags defining the entity type
   PointRec   Centroid;                   //The centroid for the entity
   PointRec   ScaleConst;                 //Scaling constant (1.0f is no scaling)
   PointRec   Velocity;                   //The velociy of the entity
   PointRec   Rotation;                   //The rotational angles
   VertexRec* BV[ENTITY_BV_COUNT];        //Bounding volume vertex pointers
   ListRec*   VertexList;                 //List of vertices for the entity
   ListRec*   PolygonList;                //List of polygons for the entity
   ListRec*   EntityList;                 //Lisy of sub-entities


   /*---- Constructor --------------------------------------------------------*/
   EntityRec(void) 
      {
      //Flags       = ENTITY_SHADE;         //Set the shade flag for the first time
      Flags       = ENTITY_NULL;         //Set the shade flag for the first time
      
      Centroid    = 0.0f;
      ScaleConst  = 1.0f;
      Velocity    = 0.0f;
      Rotation    = 0.0f;
      
      VertexList  = NULL;
      PolygonList = NULL;
      EntityList  = NULL;
      
      for (int I = 0; I < ENTITY_BV_COUNT; I++) {BV[I] = NULL;}
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~EntityRec(void) 
      {
      //Deallocate all the sub-Entitites (this will recursively call the 
      // destructor, ~EntityRec( )).
      while (EntityList != NULL) {delete (EntityRec*)LinkedList.Retrieve(EntityList);}

      //Nuke all the lists
      LinkedList.Nuke(VertexList);
      LinkedList.Nuke(PolygonList);
      }

   /*-------------------------------------------------------------------------
      This will setup all the Polygon and Vertex Normals for *this Entity and
      it's sub-Entities. Returns true on success.

      UseFacetFlags : If set true, the vertex normals are calculated and 
                      affected by the Facet[] flags in the polygon. It is 
                      assumed that the Facet[] flags are initialized correctly.
     -------------------------------------------------------------------------*/
   bool CalcNormals(bool UseFacetFlags)
      {
      //---- Find normals for all Polygons ----
      ListRec* PolygonNode = PolygonList;
      while (PolygonNode != NULL)
         {
         #define Polygon ((PolygonRec*)PolygonNode->Data)
         if (Polygon == NULL) {return false;}

         //Ensure that no Vertex pointers in Polygon is NULL
         if ((Polygon->Vertex[0] == NULL) || 
             (Polygon->Vertex[1] == NULL) || 
             (Polygon->Vertex[2] == NULL)) {return false;}


         //Compute the normal for this Polygon
         Polygon->Edge[0] = Polygon->Vertex[1]->Coord - Polygon->Vertex[0]->Coord;
         Polygon->Edge[1] = Polygon->Vertex[2]->Coord - Polygon->Vertex[0]->Coord;
         Polygon->Edge[2] = Polygon->Vertex[2]->Coord - Polygon->Vertex[1]->Coord;
         Polygon->Normal  = (Polygon->Edge[0].Cross(Polygon->Edge[1])).Unit();


         //Add this polygon normal to it's vertertex normals
         for (int I = 0; I < POLY_PT_COUNT; I++)
            {
            //Take Facet flags into consideration if necessary
            if (!(UseFacetFlags && Polygon->Facet[I]))
               {
               //Initialize if 0, else add if not 0
               if (Polygon->Vertex[I]->PolyRefCount == 0) {Polygon->Vertex[I]->Normal = Polygon->Normal;}
               else {Polygon->Vertex[I]->Normal += Polygon->Normal;}
         
               Polygon->Vertex[I]->PolyRefCount++;
               }
            }

         //Advance to the next Polygon
         PolygonNode = PolygonNode->Next;
         #undef Polygon
         }


      //---- Find normals for all Vertices ----
      ListRec* VertexNode  = VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         //Vertex normal is the average of all Polygon normals that share this Vertex
         if (Vertex->PolyRefCount != 0) 
            {
            Vertex->Normal *= (1.0f / (float)Vertex->PolyRefCount);
            Vertex->Normal  = Vertex->Normal.Unit();
            }

         Vertex->PolyRefCount = 0;  //Reset polygon reference count
         
         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }


      //---- Find normals for all sub-Entities ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->CalcNormals(false)) {return false;}
         EntityNode = EntityNode->Next;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Setup entire Entity's facet flags. The function requires that the polygon 
      normals and the vertex normals are initialized correctly.

      FacetAngle     : The angle (in radians) for the facet threshold.
      CalcNormalFlag : If set true, the normals will be calculated. This is 
                       only necessary if the normals are not defined yet.
     -------------------------------------------------------------------------*/
   bool SetFacet(float FacetAngle, bool CalcNormalFlag)
      {
      //Calculate the normals if required
      if (CalcNormalFlag)
         {
         if (!CalcNormals(false)) {return false;}
         CalcNormalFlag = false;             //We can clear this, as CalcNormals() computes normals recursively
         }

      //---- Find normals for all Polygons ----
      ListRec* PolygonNode = PolygonList;
      while (PolygonNode != NULL)
         {
         #define Polygon ((PolygonRec*)PolygonNode->Data)
         if (Polygon == NULL) {return false;}

         //Ensure that no Vertex pointers in Polygon is NULL
         if ((Polygon->Vertex[0] == NULL) || 
             (Polygon->Vertex[1] == NULL) || 
             (Polygon->Vertex[2] == NULL)) {return false;}


         //Process facet flags for current Polygon
         for (int I = 0; I < POLY_PT_COUNT; I++)
            {
            //Get the angle between Polygon normal and vertex normal
            float Angle = Polygon->Normal.AngleDot(Polygon->Vertex[I]->Normal);
            if (Angle > FacetAngle) {Polygon->Facet[I] = true;}
            else {Polygon->Facet[I] = false;}
            /**/

            /*
            PointRec R = Polygon->Normal.DirAngles(Polygon->Vertex[I]->Normal);
            PointRec RD(rad2deg(R.X), rad2deg(R.Y), rad2deg(R.Z), 0);

            int C = 0;
            if (R.X < FacetAngle) {C++;}
            if (R.Y < FacetAngle) {C++;}
            if (R.Z < FacetAngle) {C++;}

            if (C == 1) {Polygon->Facet[I] = false;}
            else {Polygon->Facet[I] = true;}
            /**/
            }


         //Advance to the next Polygon
         PolygonNode = PolygonNode->Next;
         #undef Polygon
         }


      //---- Set facet for all sub-Entities ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->SetFacet(FacetAngle, false)) {return false;}
         EntityNode = EntityNode->Next;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Returns the minimum and the maximum [X,Y,Z] dimensions of *this Entity. 
      All it's sub entities are taken into consideration.

      Min      : Minimum dimensions. Must be set to float_MAX on function entry.
      Max      : Maximum dimensions. Must be set to float_MIN on function entry.
     -------------------------------------------------------------------------*/
   bool MinMax(PointRec* Min, PointRec* Max)
      {
      if ((Min == NULL) || (Max == NULL)) {return false;}

      //---- Find the minimum and maximum in the current vertex list ----
      ListRec* VertexNode = VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         //See if the current Vertex is part of the bounding volume. If it is,
         // the Vertex will be excluded from the comparison tests later on.
         int  I         = 0;
         bool BV_Vertex = false;
         while ((I < ENTITY_BV_COUNT) && !BV_Vertex)
            {
            if (BV[I] == Vertex) {BV_Vertex = true;}
            I++;
            }

         //If the vertex is not part of the bounding volume, do comparison tests.
         if (!BV_Vertex)
            {
            if (Vertex->Coord.X < Min->X) {Min->X = Vertex->Coord.X;}
            if (Vertex->Coord.Y < Min->Y) {Min->Y = Vertex->Coord.Y;}
            if (Vertex->Coord.Z < Min->Z) {Min->Z = Vertex->Coord.Z;}

            if (Vertex->Coord.X > Max->X) {Max->X = Vertex->Coord.X;}
            if (Vertex->Coord.Y > Max->Y) {Max->Y = Vertex->Coord.Y;}
            if (Vertex->Coord.Z > Max->Z) {Max->Z = Vertex->Coord.Z;}
            }

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }


      //---- Find the minimum and maximum in all sub-Entities ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->MinMax(Min, Max)) {return false;}
         EntityNode = EntityNode->Next;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Calculates the bounding box for *this entity. The bounding bow will also 
      surround the sub-Entities. Returns true on success.
      
      The bounding volume structure is as follows:
      
            0------1
           /.     /|
          / .    / |
         3------2  |
         |  4...|..5
         | .    | /
         |.     |/
         7------6
      
      Recursive : If this flag is set, the bounding volume for *this is
                  computed, and recursively for its children as well. If 
                  the flag is false, only the parent's bouding volume is
                  found.
     -------------------------------------------------------------------------*/
   bool FindBoundingBox(bool Recursive)
      {
      //-- Recursively compute bounding volume --
      if (Recursive)
         {
         ListRec* EntityNode = EntityList;
         while (EntityNode != NULL)
            {
            if (!((EntityRec*)EntityNode->Data)->FindBoundingBox(Recursive)) {return false;}
            EntityNode = EntityNode->Next;
            }
         }

      
      //-- Find the extreme dimensions --
      PointRec Min = float_MAX;
      PointRec Max = float_MIN;
      if (!MinMax(&Min, &Max)) {return false;}

      
      //-- Setup the bouding volume for the Entity --
      for (int I = 0; I < ENTITY_BV_COUNT; I++)
         {
         //Create and insert a bouding vertex
         if (BV[I] == NULL)
            {
            BV[I] = new VertexRec;
            if (BV[I] == NULL) {return false;}
            if (!LinkedList.Insert(VertexList, BV[I])) 
               {delete BV[I]; BV[I] = NULL; return false;}
            }

         //---- Setup the volume coordinates ----

         //-- Process X coordinates in bouding vertices --
         if ((I == 1) || (I == 2) || (I == 5) || (I == 6)) //Right side of the bounding cube gets Max.X
            {BV[I]->Coord.X = Max.X;}
         else {BV[I]->Coord.X = Min.X;}         //Left side of the bounding cube gets Min.X

         //-- Process Y coordinates in bouding vertices --
         if (I < 4)                                   //Top side of the bounding cube gets Max.Y
            {BV[I]->Coord.Y = Max.Y;}
         else {BV[I]->Coord.Y = Min.Y;}         //Bottom side of the bounding cube gets Min.Y

         //-- Process Z coordinates in bouding vertices --
         if ((I == 0) || (I == 1) || (I == 4) || (I == 5)) //Back side of the bounding cube gets Max.Z
            {BV[I]->Coord.Z = Max.Z;}
         else {BV[I]->Coord.Z = Min.Z;}         //Front side of the bounding cube gets Min.Z
         }
         
      //---- Set up the bounding volume face normals. These are stored in
      //     the first six BV[]->Normal data structures. NOTE: These
      //     should NOT be treated as bounding volume vertex normals, they
      //     are face normals!! ----
      BV[0]->Normal = PointRec(0.0f, 1.0f, 0.0f, 0.0f);     //Top face normal
      BV[1]->Normal = -BV[0]->Normal;                 //Bottom face normal
         
      BV[2]->Normal = PointRec(1.0f, 0.0f, 0.0f, 0.0f);     //Right face normal
      BV[3]->Normal = -BV[2]->Normal;                 //Left face normal
         
      BV[4]->Normal = PointRec(0.0f, 0.0f, 1.0f, 0.0f);     //Back face normal
      BV[5]->Normal = -BV[4]->Normal;                 //Front face normal
            
      return true;
      }

   /*-------------------------------------------------------------------------
      Finds the centroid for *this Entity and for it's children. The centroid 
      of a base Entity is affected by the sub-Entities. Returns true on 
      success.

      Level    : Recursion level, MUST be 0 on entry.
      Sum      : The accumulated sum in the sub-Entity tree is returned
                 here. The t parameter holds the number of coordinates 
                 accumulated.
     -------------------------------------------------------------------------*/
   bool FindCentroid(dword Level, PointRec* Sum)
      {
      if (Sum == NULL) {return false;}

      //Since this is the first level, the sum must be reset
      if (Level == 0) {*Sum = 0.0f;}
      Centroid = 0.0f;

      //-- Find the local centroid from the sub-Entity list --
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->FindCentroid(Level+1, &Centroid)) {return false;}
         EntityNode = EntityNode->Next;
         }

      //-- Find contribute to the local centroid from the local Vertex list --
      ListRec* VertexNode = VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         //Add Vertex coordinate to the local centroid
         Centroid += Vertex->Coord; Centroid.t += 1.0f;

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }

      //-- Setup the Entity's centroid --
      if (Centroid.t != 0.0f) 
         {
         Centroid *= (1.0f / Centroid.t);
         *Sum     += Centroid;
         Sum->t   += Centroid.t;
         }

      return true;

      }

   /*-------------------------------------------------------------------------
      Scales the entire *this Entity, along with it's sub-Entities. Returns 
      true on success.
   
      ScaleParam : X, Y, Z scaling parameters.
      CentPt     : The scaling is done about this center point (usually the 
                   base Entity's cetroid).
     -------------------------------------------------------------------------*/
   bool Scale(PointRec* ScaleParam, PointRec* CentPt)
      {
      if ((ScaleParam == NULL) || (CentPt == NULL)) {return false;}

      //Compute the inverse of scaling parameter to fix the normal
      PointRec ScaleParamInv = PointRec(1.0f, 1.0f, 1.0f, 1.0f) / *ScaleParam;

      //Set the shade flag
      Flags |= ENTITY_SHADE;

      //---- Translate the vertices ----
      ListRec* VertexNode = VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         Vertex->Coord  = (Vertex->Coord - *CentPt) * *ScaleParam + *CentPt;
         Vertex->Normal = (Vertex->Normal * ScaleParamInv).Unit();

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }

      //-- Also scale the centroid for the base Entity's children --
      if (&Centroid != CentPt) 
         {
         Centroid *= *ScaleParam;
         }

      //---- Translate all the sub-Entities ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->Scale(ScaleParam, CentPt)) {return false;}
         EntityNode = EntityNode->Next;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Translates the entire *this Entity, along with it's sub-Entities. 
      Returns true on success.
   
      TransVector : X, Y, Z translation parameters.
     -------------------------------------------------------------------------*/
   bool Translate(PointRec* TransVector)
      {
      if (TransVector == NULL) {return false;}

      //Set the shade flag
      Flags |= ENTITY_SHADE;

      //---- Translate the vertices ----
      ListRec* VertexNode = VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         Vertex->Coord += *TransVector;

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }

      //-- Translate the centroid as well --
      Centroid += *TransVector;

      //---- Translate all the sub-Entities ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->Translate(TransVector)) {return false;}
         EntityNode = EntityNode->Next;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Rotates the entire Entity and it's sub-Entities around a point. Returns 
      true on success.

      RotateAngle : X, Y, Z rotation parameters (in radians). NOTE: If the
                    RotateAngle pointer is the same as &this->Rotation, the 
                    the centroid, scaling constant, velocity, and the 
                    angular velocity will not be rotated.
      CentPt      : The Entity will be rotated around this point.
     -------------------------------------------------------------------------*/
   bool Rotate(PointRec* RotateAngle, PointRec* CentPt)
      {
      if ((RotateAngle == NULL) || (CentPt == NULL)) {return false;}

      //Set the shade flag
      Flags |= ENTITY_SHADE;

      //---- Rotate the vertices ----
      ListRec* VertexNode = VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         Vertex->Coord  = Vertex->Coord.Rotate(*RotateAngle, *CentPt);
         Vertex->Normal = Vertex->Normal.Rotate(*RotateAngle);

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }

      //---- Rotate the Polygon normals and re-calculate edges ----
      ListRec* PolygonNode = PolygonList;
      while (PolygonNode != NULL)
         {
         #define Polygon ((PolygonRec*)PolygonNode->Data)
         if (Polygon == NULL) {return false;}

         Polygon->Normal = Polygon->Normal.Rotate(*RotateAngle);

         //Its cheaper to re-calculate the edges
         Polygon->Edge[0] = Polygon->Vertex[1]->Coord - Polygon->Vertex[0]->Coord;
         Polygon->Edge[1] = Polygon->Vertex[2]->Coord - Polygon->Vertex[0]->Coord;
         Polygon->Edge[2] = Polygon->Vertex[2]->Coord - Polygon->Vertex[1]->Coord;

         //Advance to the next Polygon
         PolygonNode = PolygonNode->Next;
         #undef Polygon
         }

      //-- Also rotate the centroid, scaling constant, velocity, and the 
      //   angular velocity for the base Entity's children --
      if (&Rotation != RotateAngle)
         {
         if (&Centroid != CentPt) {Centroid = Centroid.Rotate(*RotateAngle, *CentPt);}
//**** fix **** ScaleConst = ScaleConst.Rotate(*RotateAngle);
         Velocity   = Velocity.Rotate(*RotateAngle);
         Rotation   = Rotation.Rotate(*RotateAngle);
         }

      //---- Rotate all the sub-Entities ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         if (!((EntityRec*)EntityNode->Data)->Rotate(RotateAngle, CentPt)) {return false;}
         EntityNode = EntityNode->Next;
         }
   
      return true;
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif