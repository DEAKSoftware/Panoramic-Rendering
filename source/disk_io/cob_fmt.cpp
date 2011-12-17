/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                   Calgari Object File Format Reader/Saver                  */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __COB_FMT_CPP__
#define __COB_FMT_CPP__


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


/*----------------------------------------------------------------------------
  Definitions.
  ----------------------------------------------------------------------------*/
#define CHUNK_AUDI   "Audi"                        //Audio Chunk. New for trueSpace3.
#define CHUNK_AXES   "Axes"                        //Axes Chunk.
#define CHUNK_CAME   "Came"                        //Camera Chunk.
#define CHUNK_CHAN   "Chan"                        //Channel Chunk.
#define CHUNK_CIMG   "CImg"                        //Caligari Image Chunk.
#define CHUNK_DDIV   "DDiv"                        //Dynamic Division Chunk.
#define CHUNK_DEFP   "DefP"                        //Deformation Chunk.
#define CHUNK_DIMS   "Dims"                        //Dimensioning Chunk.
#define CHUNK_END    "END "                        //End of file Chunk.
#define CHUNK_EYEC   "EyeC"                        //Eye constraint Chunk.
#define CHUNK_GROU   "Grou"                        //Group Chunk.
#define CHUNK_HIDN   "Hidn"                        //Hidden Object Chunk. New for trueSpace3.
#define CHUNK_JOIN   "Join"                        //Joint Chunk. New for trueSpace3.
#define CHUNK_LGHT   "Lght"                        //Light Chunk.
#define CHUNK_MAT1   "Mat1"                        //Material Chunk.
#define CHUNK_MBAL   "Mbal"                        //Metaballs Chunk. New for trueSpace3.	
#define CHUNK_MREC   "MRec"                        //Material Rectangle Chunk.
#define CHUNK_NAIL   "Nail"                        //Nail Chunk. New for trueSpace3.
#define CHUNK_OPAR   "OPar"                        //Plastiform Chunk. New for trueSpace3.
#define CHUNK_OPDF   "OPDf"                        //Plastiform deform tool Chunk. New for trueSpace3.
#define CHUNK_OPPL   "OPPl"                        //Plastiform mesh Chunk. New for trueSpace3.
#define CHUNK_PANT   "PANT"                        //3Dpaint chunk. New for trueSpace3.
#define CHUNK_PATH   "Path"                        //Path Chunk.
#define CHUNK_PHAN   "PhAn"                        //Physics object's chunk. New for trueSpace3.
#define CHUNK_PHAS   "PhAS"                        //Physics scenes's chunk. New for trueSpace3.
#define CHUNK_PHWD   "PhWd"                        //Physics wind's chunk. New for trueSpace3.
#define CHUNK_POLH   "PolH"                        //Polygonal Data Chunk.
#define CHUNK_PRTX   "PrTx"                        //Procedural Texture Chunk.
#define CHUNK_ROPT   "ROpt"                        //Render Options Chunk.
#define CHUNK_SADF   "SADf"                        //Stand-Alone Deformation Chunk.
#define CHUNK_SCEN   "Scen"                        //Scene Chunk.
#define CHUNK_UNIT   "Unit"                        //Units Chunk.
#define CHUNK_URL    "URL "                        //URL Chunk. New for trueSpace3.
#define CHUNK_VCOL   "VCol"                        //Vertex Colors Chunk.
#define CHUNK_VEWS   "Vews"                        //Views Chunk.

//Flags used in CHUNK_POLH
#define F_HOLE       0x08                          //
#define F_BACKCULL   0x10                          //Back face cull flag


/*---------------------------------------------------------------------------
  The COB file IO class.
  ---------------------------------------------------------------------------*/
class COB_Class
   {
   /*==== Private Declarations ===============================================*/
   private:

   /*---- Private Data -------------------------------------------------------*/
   ListRec* Mat1List;                              //The material list
   ListRec* PolHList;                              //Object data list

   //The file header structure
   struct FileHeaderRec 
      {
      char ID[9];                                  //ID, "Caligari "
      char Version[6];                             //Version, "V00.01"
      char Format;                                 //"A" = ASCII, "B" = binary
      char Order[2];                               //"LH" = Little endian, "HL" = big endian
      char Blank[13];                              //Reserved
      char NewLine;                                //New line character, "\n"
      };

   //Chunk header structure
   struct ChunkHeaderRec
      {
      char  Type[4];                               //Chuck type string
      word  VerMajor;                              //Major version
      word  VerMinor;                              //Minor version
      dword ChunkID;                               //Chunk ID
      dword ParentID;                              //Parent ID
      dword DataSize;                              //Number of bytes in the chunk data
      };

   //Material structure
   struct Mat1Rec
      {
      dword ID;                                    //ID of this entry
      dword ParentID;                              //ID of parent
      word  Number;                                //Material number
      char  ShaderType;                            //Shader type: 'f'=flat, 'p'=phong, 'm'=metal
      char  FacetType;                             //Facet type: 'f'=faceted, 'a'=facet angle, 's'=smooth
      byte  FacetAngle;                            //Facet angle 0-179 degrees
      float kDiff_R;                               //Diffuse color
      float kDiff_G;
      float kDiff_B;
      float Opacity;                               //Opacity
      float kAmb;                                  //Ambient coefficient
      float kSpec;                                 //Specular coefficient
      float nSpec;                                 //Specular hilight size
      float IdxRefr;                               //Index of refration
      };

   //PolyData structure
   struct PolyDataRec
      {
      word        Mat1Index;                       //Index to the material
      PolygonRec* Polygon;                         //Pointer to the Polygon 
      };

   //Object structure
   struct PolHRec
      {
      dword        ID;                             //ID of this entry
      dword        ParentID;                       //ID of parent
      dword        VertexCount;
      dword        TexPointCount;
      dword        PolygonCount;
      VertexRec*   *VertexArray;
      TexPointRec* TexPointArray;
      PolyDataRec* PolygonArray;
      };
   

   /*================ BASIC DATA MANAGAMENT ==================================*/


   /*-------------------------------------------------------------------------
      Allocates a Vertex array data structure and resets the structure.
     -------------------------------------------------------------------------*/
   VertexRec* *AllocVertexArray(dword VertexCount)
      {
      if (VertexCount == 0) {return NULL;}
      
      VertexRec* *VertexArray = new VertexRec*[VertexCount];
      if (VertexArray == NULL) {return NULL;}
      
      for (dword I = 0; I < VertexCount; I++) {VertexArray[I] = NULL;}
      
      return VertexArray;
      }

   /*-------------------------------------------------------------------------
      Deletes a Vertex array data structure and returns NULL in VertexArray.
     -------------------------------------------------------------------------*/
   void DeleteVertexArray(VertexRec* *&VertexArray, dword VertexCount)
      {
      if (VertexArray == NULL) {return;}
      
      //Delete all the allocated VertexRec data
      for (dword I = 0; I < VertexCount; I++) 
         {if (VertexArray[I] != NULL) {delete VertexArray[I];}}

      delete[] VertexArray;
      VertexArray = NULL;
      }

   /*-------------------------------------------------------------------------
      Allocates a TexPoint array data structure and resets the structure.
     -------------------------------------------------------------------------*/
   TexPointRec* AllocTexPointArray(dword TexPointCount)
      {
      if (TexPointCount == 0) {return NULL;}
      
      TexPointRec* TexPointArray = new TexPointRec[TexPointCount];
      if (TexPointArray == NULL) {return NULL;}
      
      for (dword I = 0; I < TexPointCount; I++) {TexPointArray[I] = 0.0f;}
      
      return TexPointArray;
      }

   /*-------------------------------------------------------------------------
      Deletes a TexPoint array data structure and returns NULL in TexPointArray.
     -------------------------------------------------------------------------*/
   void DeleteTexPointArray(TexPointRec* &TexPointArray, dword TexPointCount)
      {
      if (TexPointArray == NULL) {return;}
      
      delete[] TexPointArray;
      TexPointArray = NULL;
      }

   /*-------------------------------------------------------------------------
      Allocates a Polygon array data structure and resets the structure.
     -------------------------------------------------------------------------*/
   PolyDataRec* AllocPolygonArray(dword PolygonCount)
      {
      if (PolygonCount == 0) {return NULL;}
      
      PolyDataRec* PolygonArray = new PolyDataRec[PolygonCount];
      if (PolygonArray == NULL) {return NULL;}
      
      for (dword I = 0; I < PolygonCount; I++) {PolygonArray[I].Polygon = NULL;}
      
      return PolygonArray;
      }

   /*-------------------------------------------------------------------------
      Deletes a Polygon array data structure and returns NULL in PolygonArray.
     -------------------------------------------------------------------------*/
   void DeletePolygonArray(PolyDataRec* &PolygonArray, dword PolygonCount)
      {
      if (PolygonArray == NULL) {return;}
      
      //Delete all the allocated PolygonRec data
      for (dword I = 0; I < PolygonCount; I++)
         {if (PolygonArray[I].Polygon != NULL) {delete PolygonArray[I].Polygon;}}

      delete[] PolygonArray;
      PolygonArray = NULL;
      }

   /*-------------------------------------------------------------------------
      Deletes a PolHRec data structure and returns NULL in PolH.
     -------------------------------------------------------------------------*/
   void DeletePolH(PolHRec* &PolH)
      {
      if (PolH == NULL) {return;}
      
      //Delete all the allocated data
      DeleteVertexArray(PolH->VertexArray, PolH->VertexCount);
      DeleteTexPointArray(PolH->TexPointArray, PolH->TexPointCount);
      DeletePolygonArray(PolH->PolygonArray, PolH->PolygonCount);

      delete PolH;
      PolH = NULL;
      }


   /*================ POST PROCESSING ========================================*/


   /*-------------------------------------------------------------------------
      This function allocates all the required Entities and su Entities, and
      intializes these objects with the Polygon and Vertex data. The Poygons
      are also initialized with the relevant material data. On sucess, the 
      function returns true and the base Entity pointer, plus the Material and 
      Object lists (Mat1List, PolHlist) are deleted. On fail, the function 
      returns NULL in Entity.

      NOTE: The function does not allocate the Objects in a hiearchial manner 
            as specified by the ParentID fields in the chunk headers. It simply 
            assigns the first Object as the base Entity, and the rest become 
            sub Entities. (This will be fixed l8r.)
     -------------------------------------------------------------------------*/
   bool ProstProcess(EntityRec* &Entity)
      {
      //Delete any existing Entities
      if (Entity != NULL) {delete Entity; Entity = NULL;}

      //Local variables
      EntityRec* NewEntity = NULL;
      ListRec*   Mat1Node  = NULL;
      ListRec*   PolHNode  = NULL;
      dword      I;


      //==== Go through every material and setup all the relevant Polygon 
      //     data for each Object ====
      Mat1Node = Mat1List;
      while (Mat1Node != NULL)
         {
         #define Mat1 ((Mat1Rec*)Mat1Node->Data)
         if (Mat1 == NULL) {goto _ExitError;}

         //See wich Object is the parent for the current material
         PolHNode = PolHList;
         while (PolHNode != NULL)
            {
            #define PolH ((PolHRec*)PolHNode->Data)
            if (PolH == NULL) {goto _ExitError;}

            //If the parent ID of material matches this object,
            // setup all the relevant polygons.
            if (PolH->ID == Mat1->ParentID)
               {
               //Search for matching polygon               
               for (I = 0; I < PolH->PolygonCount; I++) 
                  {
                  //See if the polygon's material index matches this material
                  if (PolH->PolygonArray[I].Mat1Index == Mat1->Number)
                     {
                     //Define a temp macro to make the code readable
                     #define Polygon PolH->PolygonArray[I].Polygon
                     if (Polygon == NULL) {goto _ExitError;}

                     if (Mat1->FacetType == 'f')
                        {Polygon->Facet[0] = Polygon->Facet[1] = Polygon->Facet[2] = true;}
                     else if (Mat1->FacetType == 'a')
                        {Polygon->Facet[0] = Polygon->Facet[1] = Polygon->Facet[2] = false;}
                     else if (Mat1->FacetType == 's')
                        {Polygon->Facet[0] = Polygon->Facet[1] = Polygon->Facet[2] = false;}

                     Polygon->kAmb    = Mat1->kAmb;
                     Polygon->Reflect = Mat1->kSpec * Mat1->nSpec;
                     Polygon->Trans   = 1.0f - Mat1->Opacity;
                     Polygon->Opacity = Mat1->Opacity;
                     Polygon->kDiff   = ColorRec(Mat1->kDiff_R, Mat1->kDiff_G, Mat1->kDiff_B, 0.0f);
                     Polygon->kSpec   = Mat1->kSpec;
                     Polygon->nSpec   = Mat1->nSpec * 100.0f;
                     Polygon->IdxRefr = Mat1->IdxRefr;

                     #undef Polygon
                     }
                  }
               }

            //Advance to the next Object
            PolHNode = PolHNode->Next;
            #undef PolH 
            }

         //Advance to the next material
         Mat1Node = Mat1Node->Next;
         #undef Mat1
         }



      //==== Allocate each Object as an Entity ====
      PolHNode = PolHList;
      while (PolHNode != NULL)
         {
         #define PolH ((PolHRec*)PolHNode->Data)
         if (PolH == NULL) {goto _ExitError;}

         NewEntity = new EntityRec;
         if (NewEntity == NULL) {goto _ExitError;}

         //Insert each Vertex into the Entity's Vertex list
         for (I = 0; I < PolH->VertexCount; I++) 
            {
            //Insert, then de reference from the array so that the entries 
            // will not be deleted on exit
            if (!LinkedList.Insert(NewEntity->VertexList, PolH->VertexArray[I])) {goto _ExitError;}
            PolH->VertexArray[I] = NULL;
            }
         
         //Insert each Polygon into the Entity's Polygon list
         for (I = 0; I < PolH->PolygonCount; I++) 
            {
            //Insert, then de reference from the array so that the entries 
            // will not be deleted on exit
            if (!LinkedList.Insert(NewEntity->PolygonList, PolH->PolygonArray[I].Polygon)) {goto _ExitError;}
            PolH->PolygonArray[I].Polygon = NULL;
            }
         

         //If Entity is NULL, the first object becomes the base Entity,
         // else the current object becomes a SubEntity
         if (Entity == NULL) {Entity = NewEntity;}
         else if (!LinkedList.Insert(Entity->EntityList, NewEntity)) 
            {delete NewEntity; goto _ExitError;}
         
  
         //Advence to the next Object
         PolHNode = PolHNode->Next;
         #undef PolH 
         }

      
      //If no Objects were found in the list, the function fails.
      if (Entity == NULL) {goto _ExitError;}


      //The material and the object list can be safely deleted, as 
      // they are no longer required
      LinkedList.Nuke(Mat1List);

      PolHNode = PolHList;
      while (PolHNode != NULL) {DeletePolH(((PolHRec*&)PolHNode->Data)); PolHNode = PolHNode->Next;}
      LinkedList.Nuke(PolHList);


      //-- Normal exit --
      return true;

      //-- Exit with error --
      _ExitError:
      if (Entity != NULL) {delete Entity; Entity = NULL;}
      return false;
      }


   /*================ FILE READ IO ===========================================*/


   /*-------------------------------------------------------------------------
      Reads a chunk header.

      COB_File    : File pointer. It is assumed that the file offset is at the
                    position of the data.
      ChunkHeader : Chunk header data to read into.
     -------------------------------------------------------------------------*/
   bool ReadChunkHeader(FILE* COB_File, ChunkHeaderRec* ChunkHeader)
      {
      if ((COB_File == NULL) || (ChunkHeader == NULL)) {return false;}
   
      //Read the chunk header
      fread(&ChunkHeader->Type,     4, 1, COB_File);
      fread(&ChunkHeader->VerMajor, 2, 1, COB_File);
      fread(&ChunkHeader->VerMinor, 2, 1, COB_File);
      fread(&ChunkHeader->ChunkID,  4, 1, COB_File);
      fread(&ChunkHeader->ParentID, 4, 1, COB_File);
      fread(&ChunkHeader->DataSize, 4, 1, COB_File);
      if (ferror(COB_File) != 0) {printf("COB_Class::ReadChunkHeader( ): File read error.\n"); return false;}
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Reads a name field (usually within a chunk).

      COB_File : File pointer. It is assumed that the file offset is at the
                 position of the data.
      Name     : An allocated name string (0 terminated) will be returned here.
                 On entry, this MUST be NULL.
     -------------------------------------------------------------------------*/
   bool ReadName(FILE* COB_File, char* &Name)
      {
      if ((COB_File == NULL) || (Name != NULL)) {return false;}
   
      //Local data
      char* TempName = NULL;
      word  NameDupeCount;
      word  NameLength;

      //Read the duplicate count number for the name and 
      // the number of bytes in the name string.
      fread(&NameDupeCount, 2, 1, COB_File);
      fread(&NameLength,    2, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::ReadName( ): File read error.\n"); goto _ExitError;}
      
      //Allocate the name buffer
      TempName = new char[NameLength+1];
      if (TempName == NULL) {goto _ExitError;}

      //Read the name
      fread(TempName, (dword)NameLength, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::ReadName( ): File read error.\n"); goto _ExitError;}

      //Terminate with 0
      TempName[NameLength] = 0;

      //Append the duplicate count to the name (if not 0)
      if (NameDupeCount != 0)
         {
         Name = str_compose_new("%s%u", TempName, (int)NameDupeCount);
         if (Name == NULL) {goto _ExitError;}
         delete[] TempName;               //We can safely remove the temp name
         }
      else {Name = TempName; TempName = NULL;}


      //-- Normal exit --
      return true;

      //-- Exit with error --
      _ExitError:
      if (TempName != NULL) {delete[] TempName; TempName = NULL;}
      if (Name     != NULL) {delete[] Name; Name = NULL;}
      return false;
      }

   /*-------------------------------------------------------------------------
      Reads the local axes (usually within a chunk).

      COB_File : File pointer. It is assumed that the file offset is at the
                 position of the data.
      Axes     : Returns an allocated array of 4 points. On entry, this 
                 MUST be NULL.
     -------------------------------------------------------------------------*/
   bool ReadLocalAxes(FILE* COB_File, PointRec* &Axes)
      {
      if ((COB_File == NULL) || (Axes != NULL)) {return false;}

      //Allocate a new array
      if ((Axes = new PointRec[4]) == NULL) {return false;}
   
      //Read in the point data
      for (int Incr = 0; Incr < 4; Incr++)
         {
         fread(&Axes[Incr].X, 4, 1, COB_File);
         fread(&Axes[Incr].Y, 4, 1, COB_File);
         fread(&Axes[Incr].Z, 4, 1, COB_File);
         if (ferror(COB_File) != 0) 
            {
            printf("COB_Class::ReadLocalAxes( ): File read error.\n"); 
            delete[] Axes; Axes = NULL;
            return false;
            }
         }
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Reads the current postion (usually within a chunk).

      COB_File : File pointer. It is assumed that the file offset
                 is at the position of the data.
      CurPos   : Returns an allocated array of 4 points. On entry, this 
                 MUST be NULL.
     -------------------------------------------------------------------------*/
   bool ReadCurPos(FILE* COB_File, PointRec* &CurPos)
      {
      if ((COB_File == NULL) || (CurPos != NULL)) {return false;}

      //Allocate a new array
      if ((CurPos = new PointRec[4]) == NULL) {return false;}
   
      //NOTE: The binary version reads only 3 transforamtion vectors!! 
      //      The fourth is initialized seperately (see below).
      for (int Incr = 0; Incr < 3; Incr++)
         {
         fread(&CurPos[Incr].X, 4, 1, COB_File);
         fread(&CurPos[Incr].Y, 4, 1, COB_File);
         fread(&CurPos[Incr].Z, 4, 1, COB_File);
         fread(&CurPos[Incr].t, 4, 1, COB_File);
         if (ferror(COB_File) != 0) 
            {
            printf("COB_Class::ReadCurPos( ): File read error.\n"); 
            delete[] CurPos; CurPos = NULL;
            return false;
            }
         }

      CurPos[3] = PointRec(0.0f, 0.0f, 0.0f, 1.0f);
      
      return true;
      }

   /*-------------------------------------------------------------------------
      Reads the entire Vertex list within a chunk.

      COB_File    : File pointer. It is assumed that the file offset
                    is at the position of the data.
      PolH        : The Object structure to save the vertices. Returns an 
                    allocated array of vertex pointers in VertexArray. Each 
                    vertex pointer entry is allocated with a vertex. If there 
                    are no vertices in this chunk, NULL is returned.
     -------------------------------------------------------------------------*/
   bool ReadVertexList(FILE* COB_File, PolHRec* PolH)
      {
      if ((COB_File == NULL) || (PolH == NULL)) {return false;}

      //Delete exisitng arrays
      DeleteVertexArray(PolH->VertexArray, PolH->VertexCount);


      //Read the number of Vertices
      fread(&PolH->VertexCount, 4, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::ReadVertexList( ): File read error.\n"); goto _ExitError;}
  
      //-- Do further processing if VertexCount is not 0 --
      if (PolH->VertexCount != 0)
         {
         //Allocate the and reset the array
         PolH->VertexArray = AllocVertexArray(PolH->VertexCount);
         if (PolH->VertexArray == NULL) {goto _ExitError;}

         //Read the entire Vertex list
         for (dword I = 0; I < PolH->VertexCount; I++)
            {
            //Allocate and insert each Vertex into the Entity's list
            if ((PolH->VertexArray[I] = new VertexRec) == NULL) {goto _ExitError;}
         
            //Read vertex data
            fread(&PolH->VertexArray[I]->Coord.X, 4, 1, COB_File);
            fread(&PolH->VertexArray[I]->Coord.Y, 4, 1, COB_File);
            fread(&PolH->VertexArray[I]->Coord.Z, 4, 1, COB_File);
            if (ferror(COB_File) != 0) 
               {printf("COB_Class::ReadVertexList( ): File read error.\n"); goto _ExitError;}
            }
         }

      //-- Normal exit --
      return true;

      //-- Exit with error --
      _ExitError:
      DeleteVertexArray(PolH->VertexArray, PolH->VertexCount);
      PolH->VertexCount = 0;
      return false;
      }

   /*-------------------------------------------------------------------------
      Reads the entire texture coordinate list within a chunk.

      COB_File      : File pointer. It is assumed that the file offset
                      is at the position of the data.
      PolH          : The Object structure to save the texture poits. Returns an 
                      allocated array of texture points in TexPointArray.
     -------------------------------------------------------------------------*/
   bool ReadTexPointList(FILE* COB_File, PolHRec* PolH)
      {
      if ((COB_File == NULL) || (PolH == NULL)) {return false;}

      //Delete exisitng arrays
      DeleteTexPointArray(PolH->TexPointArray, PolH->TexPointCount);


      //Read the number of texture coordinates
      fread(&PolH->TexPointCount, 4, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::ReadTexPointList( ): File read error.\n"); goto _ExitError;}
  
      //If TexPointCount is not 0, allocate a temp array
      if (PolH->TexPointCount != 0)
         {
         PolH->TexPointArray = new TexPointRec[PolH->TexPointCount];
         if (PolH->TexPointArray == NULL) {goto _ExitError;}

         //Read the entire texture coordinate list
         for (dword I = 0; I < PolH->TexPointCount; I++)
            {
            fread(&PolH->TexPointArray[I].U, 4, 1, COB_File);
            fread(&PolH->TexPointArray[I].V, 4, 1, COB_File);
            if (ferror(COB_File) != 0) 
               {printf("COB_Class::ReadTexPointList( ): File read error.\n"); goto _ExitError;}
            }
         }

      //-- Normal exit --
      return true;

      //-- Exit with error --
      _ExitError:
      DeleteTexPointArray(PolH->TexPointArray, PolH->TexPointCount);
      PolH->TexPointCount = 0;
      return false;
      }

   /*-------------------------------------------------------------------------
      Reads the entire Polygon list within a chunk.

      COB_File      : File pointer. It is assumed that the file offset
      PolH          : The Object structure to save the polygons. The following
                      the VertexArray and the TexPointArray related fields must 
                      be initialized. On return, an array of Polygon pointers are
                      allocated. Each Polygon pointer entry is allocated with a 
                      Polygon. If there are no Polygons in this chunk, NULL is 
                      returned in PolygonArray.
     -------------------------------------------------------------------------*/
   bool ReadPolygonList(FILE* COB_File, PolHRec* PolH)
      {
      if ((COB_File == NULL) || (PolH == NULL)) {return false;}
      if ((PolH->VertexArray   == NULL) && (PolH->VertexCount   != 0)) {return false;}
      if ((PolH->TexPointArray == NULL) && (PolH->TexPointCount != 0)) {return false;}
      
      //Delete existing polygon arrays
      DeletePolygonArray(PolH->PolygonArray, PolH->PolygonCount);

      //Local variables
      byte  PolygonFlag;
      word  PolygonPtCount;
      dword VertexIndex;
      dword TexPointIndex;
      dword PtRev;
      dword I;

      //Read the number of Polygons
      fread(&PolH->PolygonCount, 4, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::ReadPolygonList( ): File read error.\n"); goto _ExitError;}
  
      //-- Do further processing if PolygonCount is not 0 --
      if (PolH->PolygonCount != 0)
         {
         //Allocate and reset the array
         PolH->PolygonArray = AllocPolygonArray(PolH->PolygonCount);
         if (PolH->PolygonArray == NULL) {goto _ExitError;}

         //Read the entire Polygon list
         for (I = 0; I < PolH->PolygonCount; I++)
            {
            //Allocate and insert Polygon
            PolH->PolygonArray[I].Polygon = new PolygonRec;
            if (PolH->PolygonArray[I].Polygon == NULL) {goto _ExitError;}

            //Read Polygon data
            fread(&PolygonFlag,     1, 1, COB_File);  //Read the Polygon flag
            fread(&PolygonPtCount,  2, 1, COB_File);  //Get number of points
            if ((PolygonFlag & F_HOLE) == 0)          //Check for F_HOLE flag
               {
               fread(&PolH->PolygonArray[I].Mat1Index, 2, 1, COB_File); //Read material index 
               }
            else {PolH->PolygonArray[I].Mat1Index = 0xFFFF;} //Set flag to indicate that there is no material

            //Assign Polygon vertices, and texture coordinates
            for (dword Pt = 0; Pt < PolygonPtCount; Pt++)
               {
               fread(&VertexIndex,   4, 1, COB_File); //Read the Vertex index
               fread(&TexPointIndex, 4, 1, COB_File); //Read the Texture coordiante index

               if (Pt < POLY_PT_COUNT)                //Number of assinemnts must be < POLY_PT_COUNT
                  {
                  //Ensure that indeces don't exceed array bounds
                  if ((VertexIndex   >= PolH->VertexCount) || 
                      (TexPointIndex >= PolH->TexPointCount)) {goto _ExitError;}
               
                  //Triangulate in reverse
                  PtRev = POLY_PT_COUNT - Pt - 1;
                  if (PolH->VertexCount   > 0) {PolH->PolygonArray[I].Polygon->Vertex[PtRev]   = PolH->VertexArray[VertexIndex];}
                  if (PolH->TexPointCount > 0) {PolH->PolygonArray[I].Polygon->TexCoord[PtRev] = PolH->TexPointArray[TexPointIndex];}
                  }
               }                                      //END for(Pt = 0...)
            
            if (ferror(COB_File) != 0) {printf("COB_Class::ReadPolygonList( ): File read error.\n"); goto _ExitError;}
            }                                         //END for(dword I = 0...)
         }                                            //END if()

      
      //-- Normal exit --
      return true;

      //-- Exit with error --
      _ExitError:
      DeletePolygonArray(PolH->PolygonArray, PolH->PolygonCount);
      PolH->PolygonCount = 0;
      return false;
      }

   /*-------------------------------------------------------------------------
      Reads a 'Polygonal Data Chunk'.

      COB_File    : File to read from.
      ChunkHeader : Pointer to the chunk header structure
     -------------------------------------------------------------------------*/
   bool ReadChunk_POLH(FILE* COB_File, ChunkHeaderRec* ChunkHeader)
      {
      if ((COB_File == NULL) || (ChunkHeader == NULL)) {return false;}

      //Setup local data
      char*        Name          = NULL;
      PointRec*    Axes          = NULL;
      PointRec*    CurPos        = NULL;
      dword        I;

      
      //Allocate a new Object data
      PolHRec* PolH = new PolHRec;
      if (PolH == NULL) {goto _ExitError;}
      
      //Setup the PolH data stucture.
      PolH->ID            = ChunkHeader->ChunkID;
      PolH->ParentID      = ChunkHeader->ParentID;
      PolH->VertexCount   = 0;
      PolH->VertexArray   = NULL;
      PolH->TexPointCount = 0;
      PolH->TexPointArray = NULL;
      PolH->PolygonCount  = 0;
      PolH->PolygonArray  = NULL;


      //Read the name of this chunk
      if (!ReadName(COB_File, Name)) {goto _ExitError;}
      
      //Read the axis information
      if (!ReadLocalAxes(COB_File, Axes)) {goto _ExitError;}
      
      //Read the current position
      if (!ReadCurPos(COB_File, CurPos)) {goto _ExitError;}

      //Read Vertex data
      if (!ReadVertexList(COB_File, PolH)) {goto _ExitError;}

      //Read texture coordinate data
      if (!ReadTexPointList(COB_File, PolH)) {goto _ExitError;}

      //Read Polygon data
      if (!ReadPolygonList(COB_File, PolH)) {goto _ExitError;}



      //-- Transform all the local vertices to the world space by multiplying
      //   them with the CurPos transformation matrix. --
      for (I = 0; I < PolH->VertexCount; I++)
         {
         //Multiply then translate
         PolH->VertexArray[I]->Coord = 
            PointRec(CurPos[0].Dot(PolH->VertexArray[I]->Coord),
                     CurPos[1].Dot(PolH->VertexArray[I]->Coord),
                     CurPos[2].Dot(PolH->VertexArray[I]->Coord), 0.0f) + 
            PointRec(CurPos[0].t, CurPos[1].t, CurPos[2].t, 0.0f);
         }


      //-- Normal exit --
      if (!LinkedList.Insert(PolHList, PolH)) {goto _ExitError;} //Insert to the local list
      if (Name          != NULL) {delete[] Name;}
      if (Axes          != NULL) {delete[] Axes;}
      if (CurPos        != NULL) {delete[] CurPos;}
      return true;

      //-- Exit with error --
      _ExitError:
      DeletePolH(PolH);
      if (Name          != NULL) {delete[] Name;}
      if (Axes          != NULL) {delete[] Axes;}
      if (CurPos        != NULL) {delete[] CurPos;}
      return false;
      }

   /*-------------------------------------------------------------------------
      Reads a 'Material Data Chunk'.

      COB_File    : File to read from.
      ChunkHeader : Pointer to the chunk header structure
     -------------------------------------------------------------------------*/
   bool ReadChunk_MAT1(FILE* COB_File, ChunkHeaderRec* ChunkHeader)
      {
      if ((COB_File == NULL) || (ChunkHeader == NULL)) {return false;}

      //Local variables
      bool     SkipFlag;
      Mat1Rec* Material;


      //-- Allocate and initialize the material data --
      if ((Material = new Mat1Rec) == NULL) {goto _ExitError;}

      //Setup the material data stucture
      Material->ID       = ChunkHeader->ChunkID;
      Material->ParentID = ChunkHeader->ParentID;

      //Read the material attributes
      fread(&Material->Number,     2, 1, COB_File);
      fread(&Material->ShaderType, 1, 1, COB_File);
      fread(&Material->FacetType,  1, 1, COB_File);
      fread(&Material->FacetAngle, 1, 1, COB_File);
      fread(&Material->kDiff_R,    4, 1, COB_File);
      fread(&Material->kDiff_G,    4, 1, COB_File);
      fread(&Material->kDiff_B,    4, 1, COB_File);
      fread(&Material->Opacity,    4, 1, COB_File);
      fread(&Material->kAmb,       4, 1, COB_File);
      fread(&Material->kSpec,      4, 1, COB_File);
      fread(&Material->nSpec,      4, 1, COB_File);
      fread(&Material->IdxRefr,    4, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::ReadChunk_MAT1( ): File read error.\n"); goto _ExitError;}


      //-- Skip environment map, texture map and bump map data --
      SkipFlag = true;
      while (SkipFlag)
         {
         char MapID[2];
         word StrLen;                            

         fread(&MapID, 2, 1, COB_File);                   //Read the next ID field
         if (strncmp(MapID, "e:", 2) == 0)
            {
            fseek(COB_File, 1, SEEK_CUR);                //Skip flags
            fread(&StrLen, 2, 1, COB_File);              //Read string length
            fseek(COB_File, (dword)StrLen, SEEK_CUR);    //Skip string
            }
         else if (strncmp(MapID, "t:", 2) == 0)
            {
            fseek(COB_File, 1, SEEK_CUR);                //Skip flags
            fread(&StrLen, 2, 1, COB_File);              //Read string length
            fseek(COB_File, (dword)StrLen, SEEK_CUR);    //Skip string
            fseek(COB_File, 16, SEEK_CUR);               //Skip data
            }
         else if (strncmp(MapID, "b:", 2) == 0)
            {
            fseek(COB_File, 1, SEEK_CUR);                //Skip flags
            fread(&StrLen, 2, 1, COB_File);              //Read string length
            fseek(COB_File, (dword)StrLen, SEEK_CUR);    //Skip string
            fseek(COB_File, 20, SEEK_CUR);               //Skip data
            }
         else {SkipFlag = false;}

         if (ferror(COB_File) != 0) 
            {printf("COB_Class::ReadChunk_MAT1( ): File read error.\n"); goto _ExitError;}
         }
      
      
      //Compensate for illegal ID field read
      if (fseek(COB_File, -2, SEEK_CUR) != 0) {goto _ExitError;}

      
      //Insert this material into the list
      if (!LinkedList.Insert(Mat1List, Material)) {goto _ExitError;} 


      //-- Normal exit --
      return true;

      //-- Exit with error --
      _ExitError:
      if (Material != NULL) {delete Material;}
      return false;
      }



   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   COB_Class(void) 
      {
      Mat1List = NULL;
      PolHList = NULL;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~COB_Class(void) 
      {
      //Delete all the materials
      LinkedList.Nuke(Mat1List);

      //Delete all the Objects one by one
      ListRec* PolHNode = PolHList;
      while (PolHNode != NULL) {DeletePolH(((PolHRec*&)PolHNode->Data)); PolHNode = PolHNode->Next;}
      LinkedList.Nuke(PolHList);
      }

   /*-------------------------------------------------------------------------
      Reads an geometric object from a COB/SCN file and returns it in Entity. 
      The function returns true on success.

      FileName : File to open.
      Entity   : The allocated Entity pointer is returned here. This becoses 
                 NULL on fail.
     -------------------------------------------------------------------------*/
   bool Read(char* FileName, EntityRec* &Entity)
      {
      if (FileName == NULL) {return false;}

      //Delete existing Entity
      if (Entity != NULL) {delete Entity; Entity = NULL;}
      
      //Local data
      PointRec       Sum;
      ListRec*       PolHNode;
      FileHeaderRec  Header;
      ChunkHeaderRec ChunkHeader;
      bool           EndFlag = false;


      //-- Open the COB file --
      FILE* COB_File = fopen(FileName, "rb");
      if (COB_File == NULL) {return false;}
   
      //-- Read the file header --
      fseek(COB_File, 0, SEEK_SET);
      fread(&Header.ID,       9, 1, COB_File);
      fread(&Header.Version,  6, 1, COB_File);
      fread(&Header.Format,   1, 1, COB_File);
      fread(&Header.Order,    2, 1, COB_File);
      fread(&Header.Blank,   13, 1, COB_File);
      fread(&Header.NewLine,  1, 1, COB_File);
      if (ferror(COB_File) != 0) {printf("COB_Class::Read( ): File read error.\n"); goto _ExitError;}

      //Check the file header
      if ((strncmp(Header.ID,      "Caligari ", 9) != 0) ||
          (strncmp(Header.Version, "V00.01",    6) != 0) ||
          (strncmp(Header.Order,   "LH",        2) != 0) ||
          (Header.Format != 'B'))
         {printf("COB_Class::Read( ): Invalid or unsupported file format.\n"); goto _ExitError;}



      //---- The the entire file ----
      while (!EndFlag)
         {
         //Read a chunk
         if (!ReadChunkHeader(COB_File, &ChunkHeader)) {goto _ExitError;}

         //Perform specific operations for each chunk
         if      (strncmp(ChunkHeader.Type, CHUNK_END , 4) == 0) {EndFlag = true;}
         else if (strncmp(ChunkHeader.Type, CHUNK_POLH, 4) == 0) {if (!ReadChunk_POLH(COB_File, &ChunkHeader)) {goto _ExitError;}}
         else if (strncmp(ChunkHeader.Type, CHUNK_MAT1, 4) == 0) {if (!ReadChunk_MAT1(COB_File, &ChunkHeader)) {goto _ExitError;}}
         else if (fseek(COB_File, ChunkHeader.DataSize, SEEK_CUR) != 0) {goto _ExitError;}
         }



      //-- Setup other attributes --
      if (!ProstProcess(Entity))          {goto _ExitError;}
      if (!Entity->SetFacet(deg2rad(24.0f), true)) {goto _ExitError;} //Setup facet and normals
      if (!Entity->FindCentroid(0, &Sum)) {goto _ExitError;}
      if (!Entity->FindBoundingBox(true)) {goto _ExitError;}


      //-- Normal exit --
      LinkedList.Nuke(Mat1List);

      PolHNode = PolHList;
      while (PolHNode != NULL) {DeletePolH(((PolHRec*&)PolHNode->Data)); PolHNode = PolHNode->Next;}
      LinkedList.Nuke(PolHList);

      if (COB_File != NULL) {fclose(COB_File);}
      return true;


      //-- Exit on error --
      _ExitError:
      LinkedList.Nuke(Mat1List);

      PolHNode = PolHList;
      while (PolHNode != NULL) {DeletePolH(((PolHRec*&)PolHNode->Data)); PolHNode = PolHNode->Next;}
      LinkedList.Nuke(PolHList);

      if (COB_File != NULL) {fclose(COB_File);}
      if (Entity   != NULL) {delete Entity; Entity = NULL;}
      return false;
      }

   /*-------------------------------------------------------------------------
      NOT AVAILABLE YET.

      Saves an Entity to a COB/SCN file. The function returns true on success.

      FileName : File to open.
      Entity   : The Entity to save.
     -------------------------------------------------------------------------*/
   bool Save(char* FileName, EntityRec* Entity)
      { 
      if ((FileName == NULL) || (Entity == NULL)) {return false;}

      //-- Declare data --
      FileHeaderRec  Header;
      ChunkHeaderRec ChunkHeader;

      //-- Open the COB file --
      FILE* COB_File = fopen(FileName, "wb");
      if (COB_File == NULL) {return false;}
   
   
      //Setup the file header
      strncpy(Header.ID, "Caligari ", 9);
      strncpy(Header.Version, "V00.01", 6);
      strncpy(Header.Order, "LH", 2);
      memset(&Header.Blank, 0, 13);
      Header.Format  = 'B';
      Header.NewLine = '\n';


      //-- Write the file header --
      fseek(COB_File, 0, SEEK_SET);
      fwrite(&Header.ID,       9, 1, COB_File);
      fwrite(&Header.Version,  6, 1, COB_File);
      fwrite(&Header.Format,   1, 1, COB_File);
      fwrite(&Header.Order,    2, 1, COB_File);
      fwrite(&Header.Blank,   13, 1, COB_File);
      fwrite(&Header.NewLine,  1, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::Save( ): File write error.\n"); goto _ExitError;}





      //Setup chunk to write EOF
      strncpy(ChunkHeader.Type, CHUNK_END, 4);
      ChunkHeader.VerMajor = 0;
      ChunkHeader.VerMinor = 1;

      //Write EOF
      fwrite(&ChunkHeader.Type,     4, 1, COB_File);
      fwrite(&ChunkHeader.VerMajor, 1, 1, COB_File);
      fwrite(&ChunkHeader.VerMinor, 1, 1, COB_File);
      if (ferror(COB_File) != 0) 
         {printf("COB_Class::Save( ): File write error.\n"); goto _ExitError;}


      //-- Normal exit --
      if (COB_File != NULL) {fclose(COB_File);}
      return true;

      //-- Exit on error --
      _ExitError:
      if (COB_File != NULL) {fclose(COB_File);}
      return false;
      }

   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
COB_Class COB;


/*==== End of file ===========================================================*/
#endif




