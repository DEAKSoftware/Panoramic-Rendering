/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                   Truevision Targa Format Reader/Saver                     */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __TGA_FMT_CPP__
#define __TGA_FMT_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../bmp_edit/pixel.h"
#include "../mem_data/bitmap.cpp"


/*----------------------------------------------------------------------------
  TGA related definitions.
  ----------------------------------------------------------------------------*/
#define TGA_ATTRIB_COUNT   0x0F                    //Mask for the number of attributes per pixel
//Order which the pixels are stored in the file:
#define TGA_PIXEL_ORDER    0x30                    //Mask for the pixel order flags
#define TGA_PIXEL_L2R_B2T  0x00                    //Left to right, bottom to top.
#define TGA_PIXEL_R2L_B2T  0x10                    //Right to left, bottom to top.
#define TGA_PIXEL_L2R_T2B  0x20                    //Left to right, top to bottom.
#define TGA_PIXEL_R2L_T2B  0x30                    //Right to left, top to bottom.
#define TGA_PIXEL_R2L_MASK 0x10                    //Right to left mask.
#define TGA_PIXEL_T2B_MASK 0x20                    //Top to bottom mask.

//Scanline interleaving flags: (It's use has been disapproved by Truevision, so it can be ignored.)
#define TGA_INTERLEAVE     0xC0                    //Mask for the interleaving flags
#define TGA_INTERL_NULL    0x00                    //Non interleaved.
#define TGA_INTERL_EV_ODD  0x40                    //Two way even-odd interleaving.
#define TGA_INTERL_4_WAY   0x80                    //Four way interleaving.

#define TGA_IMAGE_NULL     0x00                    //No bitmap
#define TGA_IMAGE_CMAP     0x01                    //Color mapped
#define TGA_IMAGE_TRUE     0x02                    //True color
#define TGA_IMAGE_GREY     0x03                    //Grey scale
#define TGA_IMAGE_CMAP_RLE 0x09                    //Color mapped, RLE compression
#define TGA_IMAGE_TRUE_RLE 0x0A                    //True color, RLE compression
#define TGA_IMAGE_GREY_RLE 0x0B                    //Grey scale, RLE compression


/*---------------------------------------------------------------------------
  The TGA file IO class.
  ---------------------------------------------------------------------------*/
class TGA_Class
   {
   /*==== Private Declarations ===============================================*/
   private:

   /*---- Private Data -------------------------------------------------------*/
   Pixel_NULL_Class  Pixel_NULL;
   Pixel_A_Class     Pixel_A;
   Pixel_LO_Class    Pixel_LO;
   Pixel_HI_Class    Pixel_HI;
   Pixel_RGB_Class   Pixel_RGB;
   Pixel_BGR_Class   Pixel_BGR;
   Pixel_BGRA_Class  Pixel_BGRA;

   /*-------------------------------------------------------------------------
     The TGA file header structure.
     -------------------------------------------------------------------------*/
   struct FileHeaderRec
      {
      byte ID_FieldSize;                              //ID field lenght
      byte ColMapType;                                //Color map type
      byte ImageType;                                 //Image type
      word ColMapEntOffs;                             //Offset of the first color map entry in the table
      word ColMapEntCount;                            //Number of entries in the color map
      byte ColMapEntSize;                             //Color map entry size
      word X_Origin;                                  //Image X and Y origin
      word Y_Origin;
      word X_Res;                                     //Image X and Y resolution
      word Y_Res;
      byte BitsPerPixel;                              //Bits per pixel
      byte ImageDesc;                                 //Image descriptor bits
      };

   /*-------------------------------------------------------------------------
     Data structure used by the TGA functions internally.
     -------------------------------------------------------------------------*/
   struct ReadLoopDataRec
      {
      byte* StartPtr;
      byte* EndPtr;
      int   ScanLineIncr;

      dword PixelStart; 
      dword PixelEnd;
      int   PixelIncr;
      };
   

   /*=========================================================================
     TGA file reading functions.
     =========================================================================*/

   /*-------------------------------------------------------------------------
      Reads the color table from the TGA file. It can handle color table formats
      such as 15, 16, 24 and 32 bits per colors. The function requires the bitmap
      bits per pixel to be 8, that is the image must be a 256 color format. 
      Returns true on success.

      TGA_File   : File to read the color table.
      Header     : TGA file header.
      ColorTable : Pointer to the 256, RGB format color table.
     -------------------------------------------------------------------------*/
   bool ReadColorTable(FILE* TGA_File, FileHeaderRec* Header, byte* ColorTable)
      {
      if ((TGA_File == NULL) || (Header == NULL) || (ColorTable == NULL)) {return false;}
   
      //Seek for the color entry offset
      fseek(TGA_File, Header->ColMapEntOffs, SEEK_CUR);

      //Ensure that there are no more than 256 color entries
      if (Header->ColMapEntCount > COLOR_TABLE_MAX_COUNT) 
         {printf("TGA_Class::ReadColorTable( ): Color table entry count is > COLOR_TABLE_MAX_COUNT.\n"); return false;}

      //-- Determine which pixel function to use --
      dword       BytesPerCT = 0;               //Bytes per color table entry
      PixelClass* TGA_Pixel  = &Pixel_NULL;     //Color table pixel format
      switch (Header->ColMapEntSize)
         {
         case 15 : TGA_Pixel = &Pixel_LO;   BytesPerCT = 2; break;
         case 16 : TGA_Pixel = &Pixel_HI;   BytesPerCT = 2; break;
         case 24 : TGA_Pixel = &Pixel_BGR;  BytesPerCT = 3; break;
         case 32 : TGA_Pixel = &Pixel_BGRA; BytesPerCT = 4; break;
         default : printf("TGA_Class::ReadColorTable( ): File contains an unknown image type.\n"); return false;
         }

      //-- Read the color table --
      byte* ColorTablePtr = ColorTable;
      for (dword Incr = 0; Incr < Header->ColMapEntCount; Incr++)
         {
         //Read the colors into the temp buffer
         byte ColorBuffer[16];
         fread(&ColorBuffer, BytesPerCT, 1, TGA_File);
         if (ferror(TGA_File) != 0) {printf("TGA_Class::ReadColorTable( ): File read error.\n"); return false;}

         //Do the color conversion
         iColorRec Color;
         TGA_Pixel->Read(ColorBuffer, &Color);
         Pixel_RGB.Write(ColorTablePtr, &Color);

         ColorTablePtr += 3;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Reads an uncompressed bitmap from the TGA file.
   
      TGA_File          : TGA file to read from.
      Bitmap            : Target bitmap data.
      LoopData          : Various data used in the read loop.
      TGA_Pixel         : Class pointer for reading the TGA's pixel format.
      TGA_BytesPerPixel : The size of the TGA pixel format.
     -------------------------------------------------------------------------*/
   bool ReadBitmap(FILE* TGA_File, BitmapRec* Bitmap, ReadLoopDataRec* LoopData, PixelClass* TGA_Pixel, dword TGA_BytesPerPixel)
      {
      if ((TGA_File == NULL) || (Bitmap == NULL) || 
          (LoopData == NULL) || (TGA_Pixel == NULL)) {return false;}

      //Local stuff
      iColorRec Color;
      byte ColorBuffer[16];

      //-- Loop through every scanline --
      byte* ScanLinePtr = LoopData->StartPtr;
      while (ScanLinePtr != LoopData->EndPtr)
         {
         //Read every pixel in scanline
         byte* PixelPtr = ScanLinePtr + LoopData->PixelStart;
         while (PixelPtr != (ScanLinePtr + LoopData->PixelEnd))
            {
            //Read the colors into the temp buffer
            fread(&ColorBuffer, TGA_BytesPerPixel, 1, TGA_File);
            if (ferror(TGA_File) != 0) 
               {printf("TGA_Class::ReadBitmap( ): File read error.\n"); return false;}

            //Do the color conversion
            TGA_Pixel->Read(ColorBuffer, &Color);
            Bitmap->Pixel->Write(PixelPtr, &Color);

            PixelPtr += LoopData->PixelIncr;
            }

         ScanLinePtr += LoopData->ScanLineIncr;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Reads a RLE compressed bitmap from the TGA file.
   
      TGA_File          : TGA file to read from.
      Bitmap            : Target bitmap data.
      LoopData          : Various data used in the read loop.
      TGA_Pixel         : Class pointer for reading the TGA's pixel format.
      TGA_BytesPerPixel : The size of the TGA pixel format.
     -------------------------------------------------------------------------*/
   bool ReadBitmapRLE(FILE* TGA_File, BitmapRec* Bitmap, ReadLoopDataRec* LoopData, PixelClass* TGA_Pixel, dword TGA_BytesPerPixel)
      {
      if ((TGA_File == NULL) || (Bitmap == NULL) || 
          (LoopData == NULL) || (TGA_Pixel == NULL)) {return false;}

      //Local stuff
      bool Repeat;
      byte RLE_Byte;
      byte RepeatColor[16];
      iColorRec Color;

      //-- Loop through every scanline --
      byte* ScanLinePtr = LoopData->StartPtr;
      while (ScanLinePtr != LoopData->EndPtr)
         {
         //Read every pixel in scanline
         byte* PixelPtr = ScanLinePtr + LoopData->PixelStart;
         while (PixelPtr != (ScanLinePtr + LoopData->PixelEnd))
            {
            Repeat = false;                              //Assume no color repeat

            fread(&RLE_Byte, 1, 1, TGA_File);            //Get RLE code
            if (ferror(TGA_File) != 0) 
               {printf("TGA_Class::ReadBitmapRLE( ): File read error.\n"); return false;}
         
            if (RLE_Byte & 0x80)                         //Is the repeat flag set?
               {
               //Read the repeating color from file
               fread(&RepeatColor, TGA_BytesPerPixel, 1, TGA_File);
               if (ferror(TGA_File) != 0) 
                  {printf("TGA_Class::ReadBitmapRLE( ): File read error.\n"); return false;}

               //Do the color conversion
               TGA_Pixel->Read(RepeatColor, &Color);
               Repeat = !Repeat;                         //Set Repeat flag
               }

            //-- Do the run as required, or until the scanline fills up --
            dword Run = ((dword)RLE_Byte & 0x7F) + 1;
            while ((Run > 0) && (PixelPtr != (ScanLinePtr + LoopData->PixelEnd)))
               {
               //Ensure that we don't write outside the allocated memory
               if (PixelPtr == LoopData->EndPtr) {return false;}
               
               if (Repeat)                               //For repeats we copy the pixel colors
                  {Bitmap->Pixel->Write(PixelPtr, &Color);} 
               else                                      //Else we fetch colors from file
                  {
                  //Read the colors into the temp buffer
                  fread(&RepeatColor, TGA_BytesPerPixel, 1, TGA_File);
                  if (ferror(TGA_File) != 0) 
                     {printf("TGA_Class::ReadBitmapRLE( ): File read error.\n"); return false;}

                  //Do the color conversion
                  TGA_Pixel->Read(RepeatColor, &Color);
                  Bitmap->Pixel->Write(PixelPtr, &Color);
                  } 

               PixelPtr += LoopData->PixelIncr;          //Advance to the next pixel
               Run--;
               }
            }

         ScanLinePtr += LoopData->ScanLineIncr;
         }

      return true;
      }


   /*============================================================================
     TGA file saving functions.
     ============================================================================*/

   /*-------------------------------------------------------------------------
     Compares two pixels. Returns true if equal.
     -------------------------------------------------------------------------*/
   inline bool ComparePixel(byte* Pixel1, byte* Pixel2, int BytesPerPixel)
      {
      int Offs = 0;
      while (Offs < BytesPerPixel)
         {
         if (Pixel1[Offs] != Pixel2[Offs]) {return false;}
         Offs++;
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      This function determines the run-length of an image. It returns true if
      the pixels are repeating, returns false if the pixels are 
      non-repeating.

      Bitmap      : Bitmap data to process.
      Pixel       : Current pixel pointer.
      FrameEndPtr : Pointer to the end of the frame.
      RunCount    : Number of (non-)repetitions.
     -------------------------------------------------------------------------*/
   inline bool FindRepeatingPixels(BitmapRec* Bitmap, byte* Pixel, byte* FrameEndPtr, int &RunCount)
      {
      bool RepeatFlag = false;

      //Find the run length of the repeating pixel
      RunCount = 0;
      byte* NextPixel = Pixel + Bitmap->BytesPerPixel;
            
      while (ComparePixel(Pixel, NextPixel, Bitmap->BytesPerPixel) && 
             (NextPixel <= FrameEndPtr) && (RunCount < 0x0000007F))
         {
         NextPixel += Bitmap->BytesPerPixel; 
         RunCount++;
         }

      //Find the number of non repeating pixels
      if (RunCount == 0)
         {
         byte* PrevPixel = Pixel;
         NextPixel       = Pixel + Bitmap->BytesPerPixel;
         while (!ComparePixel(PrevPixel, NextPixel, Bitmap->BytesPerPixel) && 
                (NextPixel <= FrameEndPtr) && (RunCount < 0x0000007F))
            {
            NextPixel += Bitmap->BytesPerPixel;
            PrevPixel += Bitmap->BytesPerPixel;
            RunCount++;
            }

         //Compensate
         RunCount--;
         }
      else {RepeatFlag = true;}
      
      return RepeatFlag;
      }

   /*-------------------------------------------------------------------------
      Writes the color table to the TGA file. The function requires the bitmap
      bits per pixel to be 8, that is the image must be a 256 color format. 
      Returns true on success.

      TGA_File   : File to read the color table.
      Header     : TGA file header.
      ColorTable : Pointer to the 256, RGB format color table.
     -------------------------------------------------------------------------*/
   bool WriteColorTable(FILE* TGA_File, FileHeaderRec* Header, byte* ColorTable)
      {
      if ((TGA_File == NULL) || (Header == NULL) || (ColorTable == NULL)) {return false;}
   
      //Local variables
      byte ColorBuffer[16];
      iColorRec Color;

      //-- Save the color table --
      for (dword Entry = 0; Entry < COLOR_TABLE_MAX_COUNT; Entry++)
         {
         //Convert the RGB color to BGR type
         Pixel_RGB.Read((ColorTable + Entry*3), &Color);
         Pixel_BGR.Write(ColorBuffer, &Color);
            
         //Save color
         fwrite(ColorBuffer, 3, 1, TGA_File);
         if (ferror(TGA_File) != 0) 
            {printf("TGA_Class::WriteColorTable( ): File write error.\n"); return false;}
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Write an uncompressed bitmap to the TGA file.
   
      TGA_File          : TGA file to read from.
      Bitmap            : Source bitmap data.
      TGA_Pixel         : Class pointer for reading the TGA's pixel format.
      TGA_BytesPerPixel : The size of the TGA pixel format.
     -------------------------------------------------------------------------*/
   bool WriteBitmap(FILE* TGA_File, BitmapRec* Bitmap, PixelClass* TGA_Pixel, dword TGA_BytesPerPixel)
      {
      if ((TGA_File == NULL) || (Bitmap == NULL) || (TGA_Pixel == NULL)) {return false;}

      //Local stuff
      iColorRec Color;
      byte ColorBuffer[16];

      for (byte* PixelPtr = Bitmap->FramePtr; PixelPtr < (Bitmap->FramePtr + Bitmap->Size); PixelPtr += Bitmap->BytesPerPixel)
         {
         //Convert color type
         Bitmap->Pixel->Read(PixelPtr, &Color);
         TGA_Pixel->Write(ColorBuffer, &Color);
            
         //Save color
         fwrite(ColorBuffer, TGA_BytesPerPixel, 1, TGA_File);
         if (ferror(TGA_File) != 0) 
            {printf("TGA_Class::WriteBitmap( ): File write error.\n"); return false;}
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Write a RLE compressed bitmap to the TGA file.
   
      TGA_File          : TGA file to read from.
      Bitmap            : Source bitmap data.
      TGA_Pixel         : Class pointer for reading the TGA's pixel format.
      TGA_BytesPerPixel : The size of the TGA pixel format.
     -------------------------------------------------------------------------*/
   bool WriteBitmapRLE(FILE* TGA_File, BitmapRec* Bitmap, PixelClass* TGA_Pixel, dword TGA_BytesPerPixel)
      {
      if ((TGA_File == NULL) || (Bitmap == NULL) || (TGA_Pixel == NULL)) {return false;}

      //Local stuff
      iColorRec Color;
      byte ColorBuffer[16];

      //-- Go trough every pixel --
      byte* Pixel       = Bitmap->FramePtr;
      byte* FrameEndPtr = Bitmap->FramePtr + Bitmap->Size;
      int U = 0;
      while (Pixel < FrameEndPtr)
         {
         //Find repetitions/non-repetitions
         int RunCount = 0;
         bool RepeatFlag = FindRepeatingPixels(Bitmap, Pixel, FrameEndPtr, RunCount);

         //Reset U on overflow
         if (U >= (int)Bitmap->U_Res) {U = 0;}

         //Calculate the remaining pixels in this scanline, and 
         // truncate the run lenght if necessary.
         int U_Rem = (int)Bitmap->U_Res - U - 1;
         if (RunCount > U_Rem) {RunCount = U_Rem;}
         U += (RunCount+1);

         //Setup the RLE byte code
         byte RLE_Code = (byte)RunCount;
         if (RepeatFlag) 
            {
            RLE_Code |= 0x80;                            //Set repeat flag
            Pixel += (RunCount * Bitmap->BytesPerPixel); //Advance to the second last postion of the run
            RunCount = 0;                                //Don't write repeatedly
            }
            
         //Save RLE byte code
         fwrite(&RLE_Code, 1, 1, TGA_File);
         if (ferror(TGA_File) != 0) 
            {printf("TGA_Class::WriteBitmapRLE( ): File write error.\n"); return false;}

         //Save the repeating/non-repeating pixels
         while (RunCount >= 0)
            {
            //Convert color type
            Bitmap->Pixel->Read(Pixel, &Color);
            TGA_Pixel->Write(ColorBuffer, &Color);
               
            //Save color
            fwrite(ColorBuffer, TGA_BytesPerPixel, 1, TGA_File);
            if (ferror(TGA_File) != 0) 
               {printf("TGA_Class::WriteBitmapRLE( ): File write error.\n"); return false;}

            //Advance to the next pixel
            Pixel += Bitmap->BytesPerPixel;
            RunCount--;
            }
         }

      return true;
      }


   /*==== Public Declarations ================================================*/
   public:

   /*-------------------------------------------------------------------------
      Reads a TGA file. Returns true on success.

      FileName : File name and path to open.
      Bitmap   : Pointer to a BitmapRec data. This structure will be 
                 initialised if the function is successful. The existing 
                 structre will be deleted on entry. Bitmap and color table 
                 data will be allocated in this structure.
     -------------------------------------------------------------------------*/
   bool Read(char* FileName, BitmapRec* Bitmap)
      {
      if ((FileName == NULL) || (Bitmap == NULL)) {return false;}

      //-- Declare data --
      FileHeaderRec   Header;
      ReadLoopDataRec LoopData;
      PixelClass*     TGA_Pixel = &Pixel_NULL;
      dword           TGA_BytesPerPixel;
      bool            RLE_Data  = false;

      //-- Reset some of the data records in Bitmap --
      Bitmap->DeleteData();
 
      //-- Open the TGA file --
      FILE* TGA_File = fopen(FileName, "rb");
      if (TGA_File == NULL) {return false;}
   
      //-- Read the file header --
      fseek(TGA_File, 0, SEEK_SET);
      fread(&Header.ID_FieldSize,   1, 1, TGA_File);
      fread(&Header.ColMapType,     1, 1, TGA_File);
      fread(&Header.ImageType,      1, 1, TGA_File);
      fread(&Header.ColMapEntOffs,  2, 1, TGA_File);
      fread(&Header.ColMapEntCount, 2, 1, TGA_File);
      fread(&Header.ColMapEntSize,  1, 1, TGA_File);
      fread(&Header.X_Origin,       2, 1, TGA_File);
      fread(&Header.Y_Origin,       2, 1, TGA_File);
      fread(&Header.X_Res,          2, 1, TGA_File);
      fread(&Header.Y_Res,          2, 1, TGA_File);
      fread(&Header.BitsPerPixel,   1, 1, TGA_File);
      fread(&Header.ImageDesc,      1, 1, TGA_File);
      if (ferror(TGA_File) != 0) 
         {printf("TGA_Class::Read( ): File read error.\n"); goto _ReadTGA_ExitError;}

      fseek(TGA_File, Header.ID_FieldSize, SEEK_CUR);

   
      //==== Setup bitmap flags and function pointers ====
      Bitmap->Flags = BMP_TEXTURE;

      switch (Header.ImageType)
         {
         //-- No image present --
         case TGA_IMAGE_NULL     :  printf("TGA_Class::Read( ): File contains no bitmap.\n"); goto _ReadTGA_ExitError;

         //-- Bitmaps with color table --
         case TGA_IMAGE_CMAP_RLE :
         case TGA_IMAGE_CMAP     :  {
                                    if ((Header.BitsPerPixel != 8) || (Header.ColMapType != 1))
                                       {printf("TGA_Class::Read( ): File contains an unknown image type.\n"); goto _ReadTGA_ExitError;}
                                 
                                    TGA_Pixel         = &Pixel_A; 
                                    TGA_BytesPerPixel = 1;
                                    Bitmap->Flags    |= BMP_TYPE_C;
                                    if (Header.ImageType == TGA_IMAGE_CMAP_RLE) {RLE_Data = true;}
                                    break;
                                    }
      
         //-- HI, RGB and RGBA color images --
         case TGA_IMAGE_TRUE_RLE :
         case TGA_IMAGE_TRUE     :  {
                                    switch (Header.BitsPerPixel)
                                       {
                                       case 16 : TGA_Pixel = &Pixel_LO;   TGA_BytesPerPixel = 2; Bitmap->Flags |= BMP_TYPE_LO;   break;
                                       case 24 : TGA_Pixel = &Pixel_BGR;  TGA_BytesPerPixel = 3; Bitmap->Flags |= BMP_TYPE_RGB;  break;
                                       case 32 : TGA_Pixel = &Pixel_BGRA; TGA_BytesPerPixel = 4; Bitmap->Flags |= BMP_TYPE_RGBA; break;
                                       default : printf("TGA_Class::Read( ): File contains an unknown image type.\n"); goto _ReadTGA_ExitError;
                                       }
                                 
                                    if (Header.ImageType == TGA_IMAGE_TRUE_RLE) {RLE_Data = true;}
                                    break;
                                    }

         //-- Grey scale images --
         case TGA_IMAGE_GREY_RLE :
         case TGA_IMAGE_GREY     :  {
                                    if (Header.BitsPerPixel != 8) 
                                       {printf("TGA_Class::Read( ): File contains an unknown image type.\n"); goto _ReadTGA_ExitError;}
                                 
                                    TGA_Pixel         = &Pixel_A; 
                                    TGA_BytesPerPixel = 1;
                                    Bitmap->Flags    |= BMP_TYPE_A;
                                    if (Header.ImageType == TGA_IMAGE_GREY_RLE) {RLE_Data = true;}
                                    break;
                                    }

         //-- Unknown image type --
         default                 :  printf("TGA_Class::Read( ): File contains an unknown image type.\n"); goto _ReadTGA_ExitError;
         }


      //==== Setup the texture data ====
      Bitmap->U_Res         = (dword)Header.X_Res;
      Bitmap->V_Res         = (dword)Header.Y_Res;
      Bitmap->FrameCount    = 1;
      if (!Bitmap->AllocateData()) {goto _ReadTGA_ExitError;}



      //==== Get the start and end positions of the read loop, depending 
      //     on the image orientation ====
      //Row of pixels arranged right to left
      if (Header.ImageDesc & TGA_PIXEL_R2L_MASK)
         {
         LoopData.PixelStart   =  Bitmap->BytesPerLine - Bitmap->BytesPerPixel;
         LoopData.PixelEnd     = -1;
         LoopData.PixelIncr    = -(int)Bitmap->BytesPerPixel;
         }
      //Row of pixels arranged left to right
      else
         {
         LoopData.PixelStart   =  0;
         LoopData.PixelEnd     =  Bitmap->BytesPerLine;
         LoopData.PixelIncr    =  Bitmap->BytesPerPixel;
         }

      //Scanlines are arranged top to bottom
      if (Header.ImageDesc & TGA_PIXEL_T2B_MASK)
         {
         LoopData.StartPtr     =  Bitmap->BitmapPtr;
         LoopData.EndPtr       =  Bitmap->BitmapPtr + Bitmap->Size;
         LoopData.ScanLineIncr =  Bitmap->BytesPerLine;
         }
      //Scanlines are arranged bottom to top
      else
         {
         LoopData.StartPtr     =  Bitmap->BitmapPtr + Bitmap->Size - Bitmap->BytesPerLine;
         LoopData.EndPtr       =  Bitmap->BitmapPtr - Bitmap->BytesPerLine;
         LoopData.ScanLineIncr = -(int)Bitmap->BytesPerLine;
         }


      //-- Read the color table when needed --
      if (Bitmap->Flags & BMP_COLOR_TABLE)
         {
         if(!ReadColorTable(TGA_File, &Header, Bitmap->ColorTablePtr)) 
            {printf("TGA_Class::ReadColorTable( ) failed.\n"); goto _ReadTGA_ExitError;}
         }


      //-- Read the TGA bitmap --
      if (!RLE_Data)
         {
         if(!ReadBitmap(TGA_File, Bitmap, &LoopData, TGA_Pixel, TGA_BytesPerPixel))
            {printf("TGA_Class::ReadBitmap( ) failed.\n"); goto _ReadTGA_ExitError;}
         }
      else
         {
         if(!ReadBitmapRLE(TGA_File, Bitmap, &LoopData, TGA_Pixel, TGA_BytesPerPixel))
            {printf("TGA_Class::ReadBitmapRLE( ) failed.\n"); goto _ReadTGA_ExitError;}
         }


      //-- Normal exit --
      if (TGA_File != NULL) {fclose(TGA_File);}
      return true;


      //-- Exit on error --
      _ReadTGA_ExitError:
      if (TGA_File != NULL) {fclose(TGA_File);}
      Bitmap->DeleteData();
      return false;
      }

   /*-------------------------------------------------------------------------
      Saves to a TGA file. If the bitmap is a multi-frame image, only the current
      frame will be saved (pointed by Bitmap->FramePtr). Returns true on success.

      FileName     : File name with path to save.
      Bitmap       : Pointer to a BitmapRec data.
      RLE_Compress : If set true, the TGA file will be compressed.
     -------------------------------------------------------------------------*/
   bool Save(char* FileName, BitmapRec* Bitmap, bool RLE_Compress)
      { 
      if (FileName == NULL) {return false;}

      //Check if the bitmap is valid
      if (!Bitmap->Check()) {return false;}

      //-- Declare data --
      FileHeaderRec Header;
      PixelClass*   TGA_Pixel = &Pixel_NULL;
      dword         TGA_BytesPerPixel;
      char ID_Str[256];


      //-- Open the TGA file --
      FILE* TGA_File = fopen(FileName, "wb");
      if (TGA_File == NULL) {return false;}
   
      //-- Prepare file header data --
      ID_Str[0] = 0;
      strcat(ID_Str, "Generated by ");
      strcat(ID_Str, Title);
      Header.ID_FieldSize = strlen(ID_Str) + 1;

      Header.X_Origin     = 0;
      Header.Y_Origin     = 0;
      Header.X_Res        = (word)Bitmap->U_Res;
      Header.Y_Res        = (word)Bitmap->V_Res;
      Header.ImageDesc    = TGA_PIXEL_L2R_T2B;

      if (Bitmap->Flags & BMP_COLOR_TABLE)
         {
         Header.ColMapType     = 1;
         Header.ColMapEntOffs  = 0;
         Header.ColMapEntCount = 256;
         Header.ColMapEntSize  = 24;
         }
      else
         {
         Header.ColMapType     = 0;
         Header.ColMapEntOffs  = 0;
         Header.ColMapEntCount = 0;
         Header.ColMapEntSize  = 0;
         }
   
   
      //-- Prepare image type header data and pixel function pointers --
      switch (Bitmap->Flags & BMP_TYPE_MASK)
         {
         case BMP_TYPE_NULL : printf("TGA_Class::Save( ): Data contains no bitmap.\n"); goto _SaveTGA_ExitError;

         case BMP_TYPE_A    : {
                              if (RLE_Compress) {Header.ImageType = TGA_IMAGE_GREY_RLE;}
                              else {Header.ImageType = TGA_IMAGE_GREY;}
                              Header.BitsPerPixel = 8;
                              TGA_Pixel = &Pixel_A;
                              TGA_BytesPerPixel = 1;
                              break;
                              }

         case BMP_TYPE_C    : {
                              if (RLE_Compress) {Header.ImageType = TGA_IMAGE_CMAP_RLE;}
                              else {Header.ImageType = TGA_IMAGE_CMAP;}
                              Header.BitsPerPixel = 8;
                              TGA_Pixel = &Pixel_A;
                              TGA_BytesPerPixel = 1;
                              break;
                              }

         case BMP_TYPE_LO   :
         case BMP_TYPE_HI   :
         case BMP_TYPE_HIA  : {
                              if (RLE_Compress) {Header.ImageType = TGA_IMAGE_TRUE_RLE;}
                              else {Header.ImageType = TGA_IMAGE_TRUE;}
                              Header.BitsPerPixel = 16;
                              TGA_Pixel = &Pixel_LO;
                              TGA_BytesPerPixel = 2;
                              break;
                              }

         case BMP_TYPE_RGB  : 
         case BMP_TYPE_BGR  : 
         case BMP_TYPE_YUV  : {
                              if (RLE_Compress) {Header.ImageType = TGA_IMAGE_TRUE_RLE;}
                              else {Header.ImageType = TGA_IMAGE_TRUE;}
                              Header.BitsPerPixel = 24;
                              TGA_Pixel = &Pixel_BGR;
                              TGA_BytesPerPixel = 3;
                              break;
                              }

         case BMP_TYPE_ARGB : 
         case BMP_TYPE_ABGR : 
         case BMP_TYPE_RGBA : 
         case BMP_TYPE_BGRA : {
                              if (RLE_Compress) {Header.ImageType = TGA_IMAGE_TRUE_RLE;}
                              else {Header.ImageType = TGA_IMAGE_TRUE;}
                              Header.BitsPerPixel = 32;
                              TGA_Pixel = &Pixel_BGRA;
                              TGA_BytesPerPixel = 4;
                              break;
                              }

         default            : printf("TGA_Class::Save( ): Data is an unknown image type.\n"); goto _SaveTGA_ExitError;
         }
   
      //-- Write the file header --
      fseek(TGA_File, 0, SEEK_SET);
      fwrite(&Header.ID_FieldSize,   1, 1, TGA_File);
      fwrite(&Header.ColMapType,     1, 1, TGA_File);
      fwrite(&Header.ImageType,      1, 1, TGA_File);
      fwrite(&Header.ColMapEntOffs,  2, 1, TGA_File);
      fwrite(&Header.ColMapEntCount, 2, 1, TGA_File);
      fwrite(&Header.ColMapEntSize,  1, 1, TGA_File);
      fwrite(&Header.X_Origin,       2, 1, TGA_File);
      fwrite(&Header.Y_Origin,       2, 1, TGA_File);
      fwrite(&Header.X_Res,          2, 1, TGA_File);
      fwrite(&Header.Y_Res,          2, 1, TGA_File);
      fwrite(&Header.BitsPerPixel,   1, 1, TGA_File);
      fwrite(&Header.ImageDesc,      1, 1, TGA_File);

      if (ferror(TGA_File) != 0) 
         {printf("TGA_Class::Save( ): File write error.\n"); goto _SaveTGA_ExitError;}

      //-- Save the ID string --
      if (Header.ID_FieldSize != 0)
         {
         fwrite(&ID_Str, Header.ID_FieldSize, 1, TGA_File);
         if (ferror(TGA_File) != 0) 
            {printf("TGA_Class::Save( ): File write error.\n"); goto _SaveTGA_ExitError;}
         }

  
      //-- Save the color table if needed --
      if (Bitmap->Flags & BMP_COLOR_TABLE)
         {
         if (!WriteColorTable(TGA_File, &Header, Bitmap->ColorTablePtr))
            {printf("TGA_Class::WriteColorTable( ) failed.\n"); goto _SaveTGA_ExitError;}
         }



      //-- Save the bitmap, no RLE compression --
      if (!RLE_Compress) 
         {
         if (!WriteBitmap(TGA_File, Bitmap, TGA_Pixel, TGA_BytesPerPixel))
            {printf("TGA_Class::WriteBitmap( ) failed.\n"); goto _SaveTGA_ExitError;}
         }
      else if (!WriteBitmapRLE(TGA_File, Bitmap, TGA_Pixel, TGA_BytesPerPixel))
         {printf("TGA_Class::WriteBitmapRLE( ) failed.\n"); goto _SaveTGA_ExitError;}



      //-- Normal exit --
      if (TGA_File != NULL) {fclose(TGA_File);}
      return true;

      //-- Exit on error --
      _SaveTGA_ExitError:
      if (TGA_File != NULL) {fclose(TGA_File);}
      return false;
      }

   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
TGA_Class TGA;


/*==== End of file ===========================================================*/
#endif




