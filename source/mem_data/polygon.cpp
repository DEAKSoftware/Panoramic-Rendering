/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                       Polygon Related Functions                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __POLYGON_CPP__
#define __POLYGON_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../math/colorrec.h"
#include "../math/pointrec.h"
#include "../math/texpointrec.h"
#include "../mem_data/vertex.cpp"
#include "../mem_data/bitmap.cpp"


/*---------------------------------------------------------------------------
   Definitions.
  ---------------------------------------------------------------------------*/
#define POLY_PT_COUNT      3                    //Number of points

#define POLYGON_NULL       0x00000000           //NULL flag
#define POLYGON_FLAG_MASK  0xFF000000           //Mask for the polygon flags
#define POLYGON_TEXTURE    0x01000000           //Texture present flag. The Texture pointer must be intialized properly.
#define POLYGON_SHADOWMAP  0x02000000           //Shadow map present flag. The ShadowMap pointer must be intialized properly.


/*---------------------------------------------------------------------------
  The Polygon class.
  ---------------------------------------------------------------------------*/
class PolygonRec
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Public Data --------------------------------------------------------*/
   dword       Flags;                           //Polygon flags
   
   ColorRec    kAmb;                            //RGB ambient color
   ColorRec    kDiff;                           //RGB diffuse color
   ColorRec    kSpec;                           //RGB specular color
   float       nSpec;                           //The size of the specular reflection
   float       Reflect;                         //Reflectivity
   float       Trans;                           //Transperacy
   float       Opacity;                         //Opacity = 1 - Transperacy
   float       IdxRefr;                         //Index of refraction
   
   PointRec    Normal;                          //Polygon normal
   float       BaryCent[POLY_PT_COUNT];         //Barycentric coordinates of an intersection (used of intersection testing only)
   bool        Facet[POLY_PT_COUNT];            //Flags indicate which corners needs facet shading
   ColorRec    Shade[POLY_PT_COUNT];            //Shading colors for each corner
   VertexRec*  Vertex[POLY_PT_COUNT];           //Polygon's vertices in the vertex list
   PointRec    Edge[POLY_PT_COUNT];             //Polygon edge vectors: Edge[0] = Vertex[1]-Vertex[0], Edge[1] = Vertex[2]-Vertex[0], Edge[2] = Vertex[2]-Vertex[1]
   TexPointRec TexCoord[POLY_PT_COUNT];         //Texture coordinates
   TexPointRec ShadMapCoord[POLY_PT_COUNT];     //Shadow map coordinates

   BitmapRec*  Texture;                         //Pointer to the texture
   BitmapRec*  ShadowMap;                       //Pointer to the shadow map


   /*---- Constructor --------------------------------------------------------*/
   PolygonRec(void)
      {
      Flags     = POLYGON_NULL;
      kAmb      = 0.0f;
      kDiff     = 1.0f;
      kSpec     = 0.0f;
      nSpec     = 0.0f;
      Reflect   = 0.0f;
      Trans     = 0.0f;
      Opacity   = 1.0f - Trans;
      IdxRefr   = 1.0f;
      Normal    = 0.0f;

      Texture   = NULL;
      ShadowMap = NULL;
   
      for (int I = 0; I < POLY_PT_COUNT; I++) 
         {             
         BaryCent[I]       = 0.0f;
         Facet[I]          = false;
         Shade[I]          = 1.0f;
         Vertex[I]         = NULL;
         Edge[I]           = 0.0f;
         TexCoord[I]       = 0.0f;
         ShadMapCoord[I]   = 0.0f;
         }
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~PolygonRec(void)  {}

   /*-------------------------------------------------------------------------
      Returns the interpolated normal of *this Polygon. The function assumes
      that the barycentric coordinate for *this Polygon is already found.
     ------------------------------------------------------------------------*/
   inline PointRec GetNormal(void)
      {
      if (Facet[0] && Facet[1] && Facet[2]) {return Normal;}

      PointRec AvgNormal = 0.0f;
      for (int I = 0; I < POLY_PT_COUNT; I++) 
         {
         AvgNormal += ((Facet[I]) ? Normal * BaryCent[I] : Vertex[I]->Normal * BaryCent[I]);
         }

      return AvgNormal.Unit();
      }

   /*-------------------------------------------------------------------------
      Computes the texture coordinate of *this Polygon by using the 
      barycentric coordinates.
     ------------------------------------------------------------------------*/
   inline TexPointRec GetTexCoord(void)
      {
      TexPointRec AvgTexCoord = TexCoord[0] * BaryCent[0];
      for (int I = 1; I < POLY_PT_COUNT; I++) 
         {AvgTexCoord += (TexCoord[I] * BaryCent[I]);}
      return AvgTexCoord;
      }

   /*-------------------------------------------------------------------------
      Computes the shadow map coordinate of *this Polygon by using the 
      barycentric coordinates.
     ------------------------------------------------------------------------*/
   inline TexPointRec GetShadMapCoord(void)
      {
      TexPointRec AvgShadMapCoord = ShadMapCoord[0] * BaryCent[0];
      for (int I = 1; I < POLY_PT_COUNT; I++) 
         {AvgShadMapCoord += (ShadMapCoord[I] * BaryCent[I]);}
      return AvgShadMapCoord;
      }

   /*==== End Class =============================================================*/
   };



/*==== End of file ===========================================================*/
#endif