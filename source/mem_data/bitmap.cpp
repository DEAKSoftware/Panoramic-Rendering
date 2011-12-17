/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                         Bitmap Data Functions                              */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __BITMAP_CPP__
#define __BITMAP_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../math/mathcnst.h"
#include "../math/colorrec.h"
#include "../bmp_edit/pixel.h"


/*----------------------------------------------------------------------------
   Definitions for the bitmaps flags.
  ----------------------------------------------------------------------------*/
#define BMP_TYPE_MASK         0x000000FF                 //Used to mask out the bitmap type
#define BMP_TYPE_NULL         0x00000000                 //Undefined bitmap type.
#define BMP_TYPE_A            0x00000001                 //08-bits/pix, greyscale, or just plain Alpha channel.
#define BMP_TYPE_C            0x00000002                 //08-bits/pix, 256 color, using a color table
#define BMP_TYPE_LO           0x00000003                 //15-bits/pix, 32k color bitmap, bit order ARRRRRGGGGGBBBBB.
#define BMP_TYPE_HI           0x00000004                 //16-bits/pix, 64k color bitmap, bit order RRRRRGGGGGGBBBBB.
#define BMP_TYPE_HIA          0x00000005                 //24-bits/pix, 64k color bitmap, plus Alpha channel, bit order RRRRRGGGGGGBBBBB AAAAAAAA.
#define BMP_TYPE_RGB          0x00000006                 //24-bits/pix, 16.7 million color bitmap, order R, G, B.
#define BMP_TYPE_RGBA         0x00000007                 //32-bits/pix, 16.7 million color bitmap, plus Alpha channel, order R, G, B, A.
#define BMP_TYPE_ARGB         0x00000008                 //32-bits/pix, Alpha channel, plus 16.7 million color bitmap, order A, R, G, B.
#define BMP_TYPE_BGR          0x00000009                 //24-bits/pix, 16.7 million color bitmap, order B, G, R.
#define BMP_TYPE_ABGR         0x0000000A                 //32-bits/pix, 16.7 million color bitmap, plus Alpha channel, order A, B, G, R.
#define BMP_TYPE_BGRA         0x0000000B                 //32-bits/pix, Alpha channel, plus 16.7 million color bitmap, order B, G, R, A.
#define BMP_TYPE_YUV          0x0000000C                 //48-bits/pix, Luminance, Chrominance Blue and Chrominance Red, order Y, U, V, each stored in 0xFF.0xFF fixed point format.

#define BMP_CLASS_MASK        0x0000FF00                 //Used to mask out the class
#define BMP_TEXTURE           0x00000100                 //Plain texture
#define BMP_FX_MAP            0x00000200                 //FX map
#define BMP_SHADOW_MAP        0x00000400                 //Shadow map

#define BMP_FLAGS_MASK        0x00FF0000                 //Used to mask out the flags
#define BMP_ANIMABLE          0x00010000                 //Bitmap is animable
#define BMP_FONT_TABLE        0x00020000                 //Bitmap is used as a font
#define BMP_COLOR_TABLE       0x00040000                 //Bitmap uses a color table
#define BMP_PROCTEX           0x00080000                 //This is a procedural texture

#define COLOR_TABLE_SIZE      256*3                      //Color table size using RGB color entries
#define COLOR_TABLE_MAX_COUNT 256




/*---------------------------------------------------------------------------
   A font table record.
  ---------------------------------------------------------------------------*/
class FontTableRec
   {
   /*==== Public Declarations ================================================*/
   public:

   dword X_Origin;
   dword Y_Origin;
   dword X_Size;
   dword Y_Size;
   };

#define FONT_TABLE_SIZE       256*sizeof(FontTableRec)   //Font table size in bytes
#define FONT_TABLE_MAX_COUNT  256                        //Max number of entries in the font table



/*---------------------------------------------------------------------------
  The BitmapRec class.
  ---------------------------------------------------------------------------*/
class BitmapRec
   {
   /*==== Public Declarations ================================================*/
   public:

   dword         Flags;                   //Bitmap property flags (see type definitions above)

   byte*         BitmapPtr;               //Points to the bitmap data
   byte*         FramePtr;                //Points to the current frame in the bitmap data
   byte*         BitmapEndPtr;            //The start of the invalid memory area. Valid memory area is always (BitmapPtr <= Ptr < BitmapEndPtr).
   dword         FrameCount;              //Number of frames present (including the first one)

   PixelClass*   Pixel;
   void*         ProcTexFn;               //Procedural texture function pointer

   byte*         ColorTablePtr;           //Points to the color table
   FontTableRec* FontTablePtr;            //Points to the font table. Its a 256 allocated array.
   dword         FileNameNumber;          //This will hold the number that indexes the texture filename table
   
   dword         Size;                    //Size of the bitmap in bytes (for a single frame only)
   dword         TotalSize;               //Total size of the bitmap in bytes, icluding all frames
   dword         BitsPerPixel;            //Number of bits for each pixel
   dword         BytesPerPixel;           //Pixel size in bytes
   dword         BytesPerLine;            //Number bytes in each line of the bitmap

   dword         U_Res;                   //Horizontal resolution of bitmap
   dword         V_Res;                   //Vertical resolution of bitmap
   dword         U_Cent;                  //Center point U of bitmap
   dword         V_Cent;                  //Center point V of bitmap
              

   /*---- Constructor --------------------------------------------------------*/
   BitmapRec(void)
      {
      Flags           = BMP_TYPE_NULL;

      BitmapPtr       = NULL;
      FramePtr        = NULL;
      BitmapEndPtr    = NULL;
      FrameCount      = 0;

      Pixel           = NULL;
      ProcTexFn       = NULL;

      ColorTablePtr   = NULL;
      FontTablePtr    = NULL;
      FileNameNumber  = 0;
   
      Size            = 0;
      TotalSize       = 0;
      BitsPerPixel    = 0;
      BytesPerPixel   = 0;
      BytesPerLine    = 0;

      U_Res           = 0;
      V_Res           = 0;
      U_Cent          = 0;
      V_Cent          = 0;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~BitmapRec(void)
      {
      if (Pixel         != NULL) {delete   Pixel;         Pixel = NULL;}
      if (BitmapPtr     != NULL) {delete[] BitmapPtr;     BitmapPtr = NULL;}
      if (ColorTablePtr != NULL) {delete[] ColorTablePtr; ColorTablePtr = NULL;}
      if (FontTablePtr  != NULL) {delete[] FontTablePtr;  FontTablePtr = NULL;}
      }


   /*-------------------------------------------------------------------------
      This function checks the correctness of a general purpose bitmap data 
      structure. It does not perform a comprehensive check, it only ensures that
      some of the allocated data structures should be present for certain 
      configurations. If the structure is correct it returns true.
     -------------------------------------------------------------------------*/
   bool Check(void)
      {
      return true;
      }

   /*-------------------------------------------------------------------------
      Returns the linear address within the *this bitmap (relative to 
      FramePtr) for a given [U, V] coordinate.
     -------------------------------------------------------------------------*/
   inline byte* LinAddr(int U, int V)
      {
      return FramePtr + V*BytesPerLine + U*BytesPerPixel;
      }

   /*-------------------------------------------------------------------------
      This function allocates a new texture in *this general purpose bitmap data 
      structure. On success the function returns true and all the data members 
      in *this are initialised or allocated. On fail, the function returns 
      false, and the entire *this data tructure is reset to zero.

      The following members of *this must be defined:
         Flags        : Bitmap type and flags. If the font table flag 
                        is set, a font table will be created. If the 
                        color table flag is set, and the image type 
                        specifes a non-color table image, this flag 
                        will be ignored and cleared.
         U_Res, V_Res : U/V resolution.
         FrameCount   : Number of frames.

      The rest of the data members are either allocated or reset.
     -------------------------------------------------------------------------*/
   bool AllocateData(void)
      {
      //Reset everyting, except the input values
      dword Temp_Flags      = Flags;
      dword Temp_U_Res      = U_Res;
      dword Temp_V_Res      = V_Res;
      dword Temp_FrameCount = FrameCount;
      DeleteData();
      Flags      = Temp_Flags;
      U_Res      = Temp_U_Res;
      V_Res      = Temp_V_Res;
      FrameCount = Temp_FrameCount;

      //-- Determine the bits per pixel and setup the color table flag --
      switch (Flags & BMP_TYPE_MASK)
         {
         case BMP_TYPE_NULL : Pixel = new Pixel_NULL_Class; BitsPerPixel = 0;  break;
         case BMP_TYPE_A    : Pixel = new Pixel_A_Class;    BitsPerPixel = 8;  Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_C    : Pixel = new Pixel_C_Class;    BitsPerPixel = 8;  Flags |=  BMP_COLOR_TABLE; break;
         case BMP_TYPE_LO   : Pixel = new Pixel_LO_Class;   BitsPerPixel = 16; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_HI   : Pixel = new Pixel_HI_Class;   BitsPerPixel = 16; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_HIA  : Pixel = new Pixel_HIA_Class;  BitsPerPixel = 24; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_RGB  : Pixel = new Pixel_RGB_Class;  BitsPerPixel = 24; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_RGBA : Pixel = new Pixel_RGBA_Class; BitsPerPixel = 32; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_ARGB : Pixel = new Pixel_ARGB_Class; BitsPerPixel = 32; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_BGR  : Pixel = new Pixel_BGR_Class;  BitsPerPixel = 24; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_ABGR : Pixel = new Pixel_BGRA_Class; BitsPerPixel = 32; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_BGRA : Pixel = new Pixel_ABGR_Class; BitsPerPixel = 32; Flags &= ~BMP_COLOR_TABLE; break;
         case BMP_TYPE_YUV  : Pixel = new Pixel_YUV_Class;  BitsPerPixel = 48; Flags &= ~BMP_COLOR_TABLE; break;
         default            : printf("BitmapRec::AllocateData( ): Specified image type is invalid or unknown.\n"); goto _AllocateData_Error;
         }

      if (Pixel == NULL) {printf("BitmapRec::AllocateData( ): Pixel class allocation error.\n"); goto _AllocateData_Error;}

      //-- Initialise the data members --
      BytesPerPixel  = BitsPerPixel >> 3;
      BytesPerLine   = BytesPerPixel * U_Res;
      Size           = BytesPerLine  * V_Res;
      TotalSize      = Size          * FrameCount;
      U_Cent         = U_Res >> 1;
      V_Cent         = V_Res >> 1;
      BitmapPtr      = NULL;
      FramePtr       = NULL;
      ColorTablePtr  = NULL;
      FontTablePtr   = NULL;
      FileNameNumber = 0;
      ProcTexFn      = NULL;
   
      //-- See if total size is not 0 --
      if ((TotalSize == 0) && ((Flags & BMP_TYPE_MASK) != BMP_TYPE_NULL))
         {
         printf("BitmapRec::AllocateData( ): TotalSize == 0.\n");
         goto _AllocateData_Error;
         }
   
      //-- Allocate the new bitmap if needed --
      if ((Flags & BMP_TYPE_MASK) != BMP_TYPE_NULL)
         {
         FramePtr = BitmapPtr = new byte[TotalSize];
         if (BitmapPtr == NULL)
            {
            printf("BitmapRec::AllocateData( ): Bitmap allocation error.\n");
            goto _AllocateData_Error;
            }

         //Calculate the end pointer
         BitmapEndPtr = BitmapPtr + TotalSize;
         }

      //-- Allocate color table if needed --
      if (Flags & BMP_COLOR_TABLE)
         {
         ColorTablePtr = new byte[COLOR_TABLE_SIZE];
         if (ColorTablePtr == NULL) 
            {
            printf("BitmapRec::AllocateData( ): Color table allocation error.\n");
            goto _AllocateData_Error;
            }
         
         //Set the color table pointer for the pixel IO
         Pixel->ColorTablePtr = ColorTablePtr;
         }

      //-- Allocate font table if needed --
      if (Flags & BMP_FONT_TABLE)
         {
         FontTablePtr = new FontTableRec[FONT_TABLE_MAX_COUNT];
         if (FontTablePtr == NULL) 
            {
            printf("BitmapRec::AllocateData( ): Font table allocation error.\n");
            goto _AllocateData_Error;
            }
         }

      //-- Normal exit --
      return true;

      //-- Exit on error --
      _AllocateData_Error:
      DeleteData();
      return false;
      }

   /*-------------------------------------------------------------------------
      This function de-allcoates and clears a general purpose bitmap record. 
      It is assumed that all data records in OldBitmap is initialised correctly.
     -------------------------------------------------------------------------*/
   void DeleteData(void)
      {
      if (Pixel         != NULL) {delete   Pixel;}
      if (BitmapPtr     != NULL) {delete[] BitmapPtr;}
      if (ColorTablePtr != NULL) {delete[] ColorTablePtr;}
      if (FontTablePtr  != NULL) {delete[] FontTablePtr;}

      //Clear this entire texture record
      *this = BitmapRec();
      }

   /*-------------------------------------------------------------------------
      Converts *this bitmap to an other type, specified by NewFlags. If the 
      MakeNew flag is set, the function duplicates *this bitmap, converts it, 
      then returns it's pointer. Otherwise *this bitmap will be converted and
      returns it's own pointer. The function returns NULL on fail.
     -------------------------------------------------------------------------*/
   BitmapRec* Convert(dword NewFlags, bool MakeNew)
      {
      //-- Create a new bitmap record --
      BitmapRec* NewBitmap = new BitmapRec;
      if (NewBitmap == NULL) {return NULL;}

      
      //-- Create the new bitmap data --
      NewBitmap->Flags      = NewFlags;
      NewBitmap->U_Res      = U_Res;
      NewBitmap->V_Res      = V_Res;
      NewBitmap->FrameCount = FrameCount;
      if (!NewBitmap->AllocateData()) {delete NewBitmap; return NULL;}

      
      //-- Copy and/or convert the image contents --
      byte* SourcePtr = BitmapPtr;
      byte* DestPtr   = NewBitmap->BitmapPtr;
      while (SourcePtr < BitmapEndPtr)
         {
         //Ensure that DestPtr doesn't overflow. (It shouldn't!!)
         if (DestPtr >= NewBitmap->BitmapEndPtr)
            {
            NewBitmap->DeleteData();
            delete NewBitmap;
            return NULL;
            }

         //Convert and/or copy the pixel
         iColorRec Color;
         Pixel->Read(SourcePtr, &Color);
         NewBitmap->Pixel->Write(DestPtr, &Color);
         
         //Advance to the next pixel
         SourcePtr += BytesPerPixel;
         DestPtr   += NewBitmap->BytesPerPixel;
         }

      
      //-- Replace the current bitmap if MakeNew is false --
      if (!MakeNew)
         {
         DeleteData();                 //Delete the data in *this bitmap
         *this      = *NewBitmap;      //Copy all the data over to *this bitmap
         *NewBitmap = BitmapRec();     //IMPORTANT: Zero out NewBitmap to prevent the destructor from deallocating the texture data!
         delete NewBitmap;             //Delete the new bitmap record
         }

      return NewBitmap;
      }

   /*-------------------------------------------------------------------------
      Calculates the mean absolute error between *this bitmap and Bitmap.
     -------------------------------------------------------------------------*/
   float CalcMAE(BitmapRec* Bitmap)
      {
      if (Bitmap == NULL) {return 0.0f;}
      
      //Ensure that the bitmaps have identical dimensions
      if (((Flags         & BMP_TYPE_MASK) == BMP_TYPE_NULL) ||
          ((Bitmap->Flags & BMP_TYPE_MASK) == BMP_TYPE_NULL) ||
          (U_Res != Bitmap->U_Res) || (V_Res != Bitmap->V_Res)) 
          {return 0.0f;}

      
      //-- Calculate the mean absolute error for the entire image --
      float SumMAE = 0.0f;
      byte* PixelPtr1 = FramePtr;
      byte* PixelPtr2 = Bitmap->FramePtr;
      while (PixelPtr1 < FramePtr + Size)
         {
         iColorRec Color1, Color2;
         Pixel->Read(PixelPtr1, &Color1);
         Bitmap->Pixel->Read(PixelPtr2, &Color2);

         //Use the luminace
         float Y1 = 0.299f*(float)Color1.R + 0.587f*(float)Color1.G + 0.114f*(float)Color1.B;
         float Y2 = 0.299f*(float)Color2.R + 0.587f*(float)Color2.G + 0.114f*(float)Color2.B;
         SumMAE += fabs(Y1 - Y2);

         PixelPtr1 += BytesPerPixel;
         PixelPtr2 += Bitmap->BytesPerPixel;
         }

      return SumMAE / (float)(U_Res * V_Res);
      }

   /*-------------------------------------------------------------------------
      Calculates the peak signal to noise ratio (in dB) between *this bitmap 
      and Bitmap.
     -------------------------------------------------------------------------*/
   float CalcPSNR(BitmapRec* Bitmap)
      {
      float MAE = CalcMAE(Bitmap);
      if (MAE == 0.0f) {return 0.0f;}
      return 10.0f * log10(65025.0f / MAE);
      }

   /*-------------------------------------------------------------------------
      Calculates the global entropy (in bits) of *this bitmap.
     -------------------------------------------------------------------------*/
   float CalcEntropy(void)
      {
      if ((Flags & BMP_TYPE_MASK) == BMP_TYPE_NULL) {return 0.0f;}

      //Define and reset the probability array for each grey level
      dword I, Pr[0x100];
      for (I = 0; I < 0x100; I++) {Pr[I] = 0;}

      //-- Find the probabilities in the image --
      byte* PixelPtr = FramePtr;
      while (PixelPtr < FramePtr + Size)
         {
         iColorRec Color;
         Pixel->Read(PixelPtr, &Color);

         //Use the luminance of the pixel to count the probabilty
         dword Y = (dword)(0.299f*(float)Color.R + 0.587f*(float)Color.G + 0.114f*(float)Color.B);
         Pr[Y]++;

         PixelPtr += BytesPerPixel;
         }

      //-- Calculate the entropy --
      float H = 0.0f, PixelCountInv = 1.0f / (float)(U_Res * V_Res);
      for (I = 0; I < 0x100; I++) 
         {
         float p = (float)Pr[I] * PixelCountInv;
         if (p != 0.0f) {H += (p * ln(p) * _ln2inv);}
         }
      
      return -H;
      }


   /*==== End of Class =======================================================*/
   };


/*==== End of File ===========================================================*/
#endif
