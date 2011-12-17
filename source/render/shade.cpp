/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                          Shading Functions                                 */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __SHADE_CPP__
#define __SHADE_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/list.cpp"
#include "../mem_data/vertex.cpp"
#include "../mem_data/polygon.cpp"
#include "../mem_data/entity.cpp"
#include "../mem_data/world.cpp"


/*---------------------------------------------------------------------------
  The Shade rendering class.
  ---------------------------------------------------------------------------*/
class ShadeClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*-------------------------------------------------------------------------
      Computes the diffuse light intensity for a Vertex point. Returns true on 
      success.

      Input parameters:
         Vertex         : Vertex position and normal
         LightList      : List of lights

      Output:
         Vertex->Shade  : Light intensity
     ------------------------------------------------------------------------*/
   inline bool ShadeGouraud(VertexRec* Vertex, ListRec* LightList)
      {
      if (Vertex == NULL) {return false;}

      Vertex->Shade = 0.0f;                                 //Reset the shading color

      ListRec* LightNode = LightList;                       //Start at head of the light list
      while (LightNode != NULL)
         {
         #define Light ((LightRec*)LightNode->Data)
         if (Light == NULL) {return false;}

         //Find angle for the diffuse light
         PointRec L = (Light->Coord - Vertex->Coord).Unit(); //Compute the unit light vector
         float Ang = Vertex->Normal.Dot(L);                 //Find the dot product between the N and L

         //Contribute this light if diffuse angle is > 0.0
         if (Ang > 0.0f)
            {
            Vertex->Shade += (Light->Color * Ang);
            }
      
         //Advance to the next Light node
         LightNode = LightNode->Next;
         #undef Light
         }
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Computes the diffuse rendering colors for a Polygon. The function assumes
      that the light intensity for each Polygon point is already calculated.
      Returns true on success.

      Input parameters:
         Polygon          : Polygon data
         LightList        : List of lights

      Output:
         Polygon->Shade[] : Light intensity, diffuse and ambient rendering color.
     ------------------------------------------------------------------------*/
   inline bool ShadeGouraud(PolygonRec* Polygon, ListRec* LightList)
      {
      if (Polygon == NULL) {return false;}

      //-- Compute the shade color for this Polygon -- 
      for (int I = 0; I < POLY_PT_COUNT; I++)
         {
         //For a faceted point, calculate the shading color, 
         // by using the polygon's normal
         if (Polygon->Facet[I])
            {
            //Use a dummy vertex
            VertexRec Vertex;
            Vertex.Coord  = Polygon->Vertex[I]->Coord;
            Vertex.Normal = Polygon->Normal;

            if (!ShadeGouraud(&Vertex, LightList)) {return false;}

            //Compute the final (faceted) diffuse rendering color
            Polygon->Shade[I] = Polygon->kDiff * Vertex.Shade + Polygon->kAmb * World.AmbLight;
            }
         else
            {
            //Compute the final (non-faceted) diffuse rendering color
            Polygon->Shade[I] = Polygon->kDiff * Polygon->Vertex[I]->Shade + Polygon->kAmb * World.AmbLight;
            }
         }
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Computes the diffuse and phong shading color for a given point on a 
      polygon. Returns true on success.

      LocalColor  : The shading color will be returned here
      I           : Intersection point on the Polygon
      VO          : View origin
      Surface     : The Polygon with all the surface parameters
      EntityList  : List of Entities for shadow testing.
      LightList   : List of Lights
      TestShadows : If set true, shadow testing will be performed
     ------------------------------------------------------------------------*/
   inline bool ShadePhong(ColorRec* LocalColor, PointRec* I, PointRec* VO, PolygonRec* Surface, ListRec* EntityList, ListRec* LightList, bool TestShadows)
      {
      ColorRec LightColor  = 0.0f;

      ListRec* LightNode = LightList;                       //Start at head of the light list
      while (LightNode != NULL)
         {
         #define Light ((LightRec*)LightNode->Data)
         if (Light == NULL) {return false;}

         //Find the light vector
         PointRec dL = Light->Coord - *I;
         PointRec L  = dL.Unit();                           //Compute the unit vector

         //Test for shadows if requested
         ColorRec TransColor;
         dword    TC_Count   = 0;
         bool     ShadowFlag = false;
         if (TestShadows) 
            {
            TransColor = 0.0f;
            ShadowFlag |= TestShadow(&TransColor, TC_Count, I, &L, dL.Mag(), Surface, EntityList);
            }

         //Process this light if there is no shadow
         if (!ShadowFlag)
            {
            //Find angle for the diffuse light
            PointRec N   = Surface->GetNormal();            //Get the interpolated normal
            float    Ang = N.Dot(L);                        //Find the dot product between the N and L
            if (Ang < 0.0f) {Ang = 0.0f;}                   //Don't want negative colors

            //Find angle for the specular component
            PointRec V = (*VO - *I).Unit();                 //Compute the unit view vector
            PointRec H = ((L + V) * 0.5f).Unit();           //Find halfway unit vector between light and view vectors
            float SpecAng = N.Dot(H);                       //Find the dot product between the N and H
            if (SpecAng < 0.0f) {SpecAng = 0.0f;}           //Don't want negative colors
            SpecAng = (float)pow(SpecAng, Surface->nSpec);  //Compute specular size

            //Contribute this light if either diffuse or specular angles are > 0.0
            if ((SpecAng > 0.0f) || (Ang > 0.0f))
               {
               //Find the average of the accumulated transperacy shadow colors
               if (TC_Count != 0) 
                  {
                  float CountInv = 1.0f / (float)TC_Count;  //Average transperacy color
                  float t     = TransColor.A * CountInv;    //Average transperacy
                  TransColor *= CountInv;
                  
                  //Find the "transparent" shadow: TransShadowColor = (LightColor*t + TransColor*(1-t)) * t
                  TransColor = ((Light->Color - TransColor)*t + TransColor)*t;

                  //Multiply with coeffs add the contributing light color
                  LightColor += (TransColor * (Surface->kDiff * Ang + Surface->kSpec * SpecAng));
                  }

               //No transparent shadows, nultiply with coeffs add the contributing light color
               else {LightColor += (Light->Color * (Surface->kDiff * Ang + Surface->kSpec * SpecAng));}
               }
            }

         //Advance to the next Light node
         LightNode = LightNode->Next;
         #undef Light
         }

      //-- Add the ambient component to the obtained light color --
      *LocalColor = LightColor + Surface->kAmb * World.AmbLight;
      
      return true;
      }

   /*-------------------------------------------------------------------------
      This function tests if a bounded line (the light vector) intersects with 
      an object. Returns true if intersecttion occurs.

      TransColor  : The accumulated transperacy color for transparent objects.
                    The accumulated transperacy constant is stored in 
                    TransColor->A.
      TC_Count    : Number of transparent shadow colors accumulated.
      O           : Origin of the ray.
      L           : Direction of the ray, must be a unit vector.
      Length      : Length of the ray.
      ExclSurface : Polygon to exclude from shadow testing. Can be set to NULL
                    if no exclusion is desired.
      EntityList  : List of Entities to test.
     ------------------------------------------------------------------------*/
   bool TestShadow(ColorRec* TransColor, dword &TC_Count, PointRec* O, PointRec* D, float Length, PolygonRec* ExclSurface, ListRec* EntityList)
      {
      PointRec    I;                   //Light vector intersection point

      //---- Test each Entity ----
      ListRec* EntityNode = EntityList;
      while (EntityNode != NULL)
         {
         #define Entity ((EntityRec*)EntityNode->Data)
         if (Entity == NULL) {return false;}

         //---- Test for intersection in each Polygon in the Entity ----
         ListRec* PolygonNode = Entity->PolygonList;
         while (PolygonNode != NULL)
            {
            #define Polygon ((PolygonRec*)PolygonNode->Data)
            if (Polygon == NULL) {return false;}

            //Test if current Polygon is not the exclusion surface
            if (Polygon != ExclSurface)
               {
               //Test for shadow
               if (Poly_InfLine_Intersect(&I, O, D, Polygon, true)) 
                  {
                  //Is the intersection beyond the light? If not, 
                  // the intersection point lies in a shadow.
                  if (I.t < Length) 
                     {
                     //If the polygon is not transparent, exit.
                     if (Polygon->Trans == 0.0f) {return true;}
                     
                     //For transparent polygons, we simply accumulate the shadow colors.
                     else 
                        {
                        *TransColor += ColorRec(Polygon->kDiff.R, 
                                                Polygon->kDiff.G, 
                                                Polygon->kDiff.B, 
                                                Polygon->Trans);
                        TC_Count++;
                        }
                     }
                  }
               }

            PolygonNode = PolygonNode->Next;
            #undef Polygon
            }

         //Recursively test the sub-Entities
         if (Entity->EntityList != NULL)
            {
            if (TestShadow(TransColor, TC_Count, O, D, Length, ExclSurface, Entity->EntityList)) {return true;}
            }

         //Advance to the next Entity node
         EntityNode = EntityNode->Next;
         #undef Entity
         }

      return false;
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif
