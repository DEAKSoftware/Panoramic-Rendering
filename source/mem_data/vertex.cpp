/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                        Vertex Related Functions                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __VERTEX_CPP__
#define __VERTEX_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"


/*---------------------------------------------------------------------------
  The Vertex class.
  ---------------------------------------------------------------------------*/
class VertexRec
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Public Data --------------------------------------------------------*/
   PointRec Coord;                        //Coordinate
   PointRec tCoord;                       //View transformed coordinate (used in rendering only)
   PointRec Normal;                       //Vertex normal
   ColorRec Shade;                        //Light intensity at this vertex
   dword    PolyRefCount;                 //Number of polygons sharing this vertex
   bool     TransFlag;
   bool     Visisble;

   /*---- Constructor --------------------------------------------------------*/
   VertexRec(void)
      {
      Coord        = 0.0f;
      tCoord       = 0.0f;
      Normal       = 0.0f;
      Shade        = 1.0f;
      PolyRefCount = 0;
      TransFlag    = false;
      Visisble     = false;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~VertexRec(void) {}

   /*==== End Class =============================================================*/
   };



/*==== End of file ===========================================================*/
#endif