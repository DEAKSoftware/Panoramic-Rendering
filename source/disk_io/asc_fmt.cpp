/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                                    Format Reader/Saver                     */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __ASC_FMT_CPP__
#define __ASC_FMT_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_str.cpp"
#include "../_common/list.cpp"
#include "../math/texpointrec.h"
#include "../mem_data/vertex.cpp"
#include "../mem_data/polygon.cpp"
#include "../mem_data/entity.cpp"


/*---------------------------------------------------------------------------
  The ASC file IO class.
  ---------------------------------------------------------------------------*/
class ASC_Class
   {
   /*==== Public Declarations ================================================*/
   private:

   /*-------------------------------------------------------------------------
 
     -------------------------------------------------------------------------*/
   bool SaveSubEntity(FILE* ASC_File, EntityRec* Entity)
      {
      if ((ASC_File == NULL) ||  (Entity == NULL)) {return false;}
     
      dword VertexCount  = 0;
      dword PolygonCount = 0;
      
      //-- Setup texture coords, count polygons --
      ListRec* PolygonNode = Entity->PolygonList;
      while (PolygonNode != NULL)
         {
         #define Polygon ((PolygonRec*)PolygonNode->Data)
         if (Polygon == NULL) {return false;}

         for (int I = 0; I < POLY_PT_COUNT; I++)
            {
            Polygon->Vertex[I]->tCoord.X = Polygon->TexCoord[I].U;
            Polygon->Vertex[I]->tCoord.Y = Polygon->TexCoord[I].V;
            }

         PolygonCount++;

         //Advance to the next Polygon
         PolygonNode = PolygonNode->Next;
         #undef Polygon
         }
      
      //-- Count vertices --
      ListRec* VertexNode = Entity->VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         Vertex->tCoord.t = (float)VertexCount;
         VertexCount++;

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }


      //-- Save object info --
      fprintf(ASC_File, "\nNamed object: \"%X\"\n", Entity->Flags);
      fprintf(ASC_File, "Tri-mesh, Vertices: %u Faces: %u\n", VertexCount, PolygonCount << 1);
      fprintf(ASC_File, "Mapped\nVertex list:\n");


      //-- Save vertices --
      VertexCount = 0;
      VertexNode = Entity->VertexList;
      while (VertexNode != NULL)
         {
         #define Vertex ((VertexRec*)VertexNode->Data)
         if (Vertex == NULL) {return false;}

         fprintf(ASC_File, "Vertex %u: X:%.6f Y:%.6f Z:%.6f U:%.6f V:%.6f\n", 
                VertexCount,  
                Vertex->Coord.X, Vertex->Coord.Y, Vertex->Coord.Z, 
                Vertex->tCoord.X, Vertex->tCoord.Y);

         VertexCount++;

         //Advance to the next Vertex
         VertexNode = VertexNode->Next;
         #undef Vertex
         }

      
      fprintf(ASC_File, "Face list:\n");

      //-- Save polygons --
      PolygonCount = 0;
      PolygonNode = Entity->PolygonList;
      while (PolygonNode != NULL)
         {
         #define Polygon ((PolygonRec*)PolygonNode->Data)
         if (Polygon == NULL) {return false;}

         fprintf(ASC_File, "Face %u: A:%u B:%u C:%u   AB:1 BC:1 CA:1\n", 
                PolygonCount,
                (dword)Polygon->Vertex[0]->tCoord.t,
                (dword)Polygon->Vertex[1]->tCoord.t,
                (dword)Polygon->Vertex[2]->tCoord.t);

         PolygonCount++;


         fprintf(ASC_File, "Face %u: A:%u B:%u C:%u   AB:1 BC:1 CA:1\n", 
                PolygonCount,
                (dword)Polygon->Vertex[2]->tCoord.t,
                (dword)Polygon->Vertex[1]->tCoord.t,
                (dword)Polygon->Vertex[0]->tCoord.t);

         PolygonCount++;

         //Advance to the next Polygon
         PolygonNode = PolygonNode->Next;
         #undef Polygon
         }


      //---- Save all the sub-Entities ----
      ListRec* EntityNode = Entity->EntityList;
      while (EntityNode != NULL)
         {
         if (!SaveSubEntity(ASC_File, (EntityRec*)EntityNode->Data)) {return false;}
         EntityNode = EntityNode->Next;
         }


      return true;
      }



   /*==== Public Declarations ================================================*/
   public:

   /*-------------------------------------------------------------------------
      Read is not supported.
     -------------------------------------------------------------------------*/
   bool Read(char* FileName, EntityRec* Entity)
      {return false;}

   /*-------------------------------------------------------------------------

     -------------------------------------------------------------------------*/
   bool Save(char* FileName, EntityRec* Entity)
      {
      if ((FileName == NULL) || (Entity == NULL)) {return false;}

      //-- Open the PPM file --
      FILE* ASC_File = fopen(FileName, "wt");
      if (ASC_File == NULL) {return false;}


      fprintf(ASC_File, "Ambient light color: Red=0.3 Green=0.3 Blue=0.3\n\n");

      if (!SaveSubEntity(ASC_File, Entity))
         {printf("ASC_Class::Save( ): SaveSubEntity( ) failed\n"); goto _ExitError;}


      //---- Normal exit ----
      if (ASC_File != NULL) {fclose(ASC_File);}
      return true;

      //---- Exit on error ----
      _ExitError:
      if (ASC_File != NULL) {fclose(ASC_File);}
      return false;
      }

   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
ASC_Class ASC;


/*==== End of file ===========================================================*/
#endif
