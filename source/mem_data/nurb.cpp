/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                         Nurb Related Functions                             */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __NURB_CPP__
#define __NURB_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../math/colorrec.h"
#include "../math/pointrec.h"
#include "../math/texpointrec.h"


/*---------------------------------------------------------------------------
   Definitions.
  ---------------------------------------------------------------------------*/
#define NURB_NULL          0x00000000                       //NULL attribute
#define NURB_ATTRIB_MASK   0xFF000000                       //Mask for the attributes
#define NURB_COLOR         0x01000000                       //Flag to indicate that color data is present
#define NURB_TEXTURE       0x02000000                       //Flag to indicate that texture point data is present
#define NURB_POINT         0x04000000                       //Flag to indicate that control point data is present
#define NURB_VALID         0x80000000                       //Flag indicating that the nurb has a vaild data stucture


/*---------------------------------------------------------------------------
  The Nurb class.
  ---------------------------------------------------------------------------*/
class NurbRec
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Public Data --------------------------------------------------------*/
   dword        Flags;                           //Polygon flags

   dword        S_Res;                          //Number of control points in the S and T direction
   dword        T_Res;
   dword        S_Order;                        //Order in the S and T direction
   dword        T_Order;
   dword        S_KnotRes;                      //Number of knots for the S and T direction
   dword        T_KnotRes;
   dword        TotalCount;                     //Total number of control points and colors
   dword        FloatsPerEntry;                 //Number of floats per control point/color
   dword        FloatsPerLine;                  //Number of floats per line of control points/colors in the T direction
   dword        S_Res_m1;                       //S_Res-1
   dword        T_Res_m1;                       //T_Res-1
   float        S_ResInv;                       //1.0f / (S_Res-1)
   float        T_ResInv;                       //1.0f / (T_Res-1)

   float*       S_Knots;                        //The S and T knot data
   float*       T_Knots;
   ColorRec*    Colors;                         //Colors for each control point
   TexPointRec* TexPoints;                      //Texture points
   PointRec*    Points;                         //The control points


   /*---- Constructor --------------------------------------------------------*/
   NurbRec(void)
      {
      S_Res          = NURB_NULL;
      T_Res          = 0;
      S_Order        = 0;
      T_Order        = 0;
      S_KnotRes      = 0;
      T_KnotRes      = 0;
      TotalCount     = 0;
      FloatsPerEntry = 0;
      FloatsPerLine  = 0;
      S_Res_m1       = 0;
      T_Res_m1       = 0;
      S_ResInv       = 0.0f;
      T_ResInv       = 0.0f;

      S_Knots        = NULL;
      T_Knots        = NULL;
      Colors         = NULL;
      Points         = NULL;
      TexPoints      = NULL;
      }

   /*-------------------------------------------------------------------------
      Create and intialize a Nurb instance. NOTE: If this contructor is used 
      to intialize an existing NurbRec, ensure that the existing NurbRec 
      calls it's destructor first. Eg:

      This is a no-no:
         NurbRec NURB(4, 4, NURB_POINT);
         NURB = NurbRec(5, 5, NURB_POINT);

      Correct:
         NurbRec NURB(4, 4, NURB_POINT);
         NURB.~NurbRec();
         NURB = NurbRec(5, 5, NURB_POINT);

      Such calling convetions must be observed to prevent memory leaks.
     ------------------------------------------------------------------------*/
   NurbRec(dword S_Res_New, dword T_Res_New, dword NewFlags)
      {
      if (S_Res_New < 2) {S_Res_New = 2;}
      if (T_Res_New < 2) {T_Res_New = 2;}

      //Setup the data
      Flags          = NURB_NULL;
      S_Res          = S_Res_New;
      T_Res          = T_Res_New;
      S_Order        = S_Res;
      T_Order        = T_Res;
      S_KnotRes      = S_Res + S_Order;
      T_KnotRes      = T_Res + T_Order;
      FloatsPerEntry = 4;
      FloatsPerLine  = S_Res * FloatsPerEntry;
      TotalCount     = S_Res * T_Res;
      Colors         = NULL;
      Points         = NULL;
      S_Res_m1       = S_Res-1;
      T_Res_m1       = T_Res-1;
      S_ResInv       = 1.0f / (float)(S_Res_m1);
      T_ResInv       = 1.0f / (float)(T_Res_m1);

      //Allocate the knots
      bool StatusFlag = true;
      S_Knots = new float[S_KnotRes];
      T_Knots = new float[T_KnotRes];
      if ((S_Knots == NULL) || (T_Knots == NULL)) {StatusFlag = false;}
      
      dword I;

      //Allocate a color matrix if requested
      if ((NewFlags & NURB_COLOR) != NURB_NULL) 
         {
         Colors = new ColorRec[TotalCount];
         if (Colors == NULL) {StatusFlag = false;}
         for (I = 0; I < TotalCount; I++) {Colors[I] = 0.0f;}
         }

      //Allocate a texture point matrix if requested
      if ((NewFlags & NURB_TEXTURE) != NURB_NULL) 
         {
         TexPoints = new TexPointRec[TotalCount];
         if (TexPoints == NULL) {StatusFlag = false;}
         for (I = 0; I < TotalCount; I++) {TexPoints[I] = 0.0f;}
         }

      //Allocate a control point matrix if requested
      if ((NewFlags & NURB_POINT) != NURB_NULL) 
         {
         Points = new PointRec[TotalCount];
         if (Points == NULL) {StatusFlag = false;}
         for (I = 0; I < TotalCount; I++) {Points[I] = 0.0f;}
         }

      //Error checking
      if (!StatusFlag) {this->~NurbRec(); return;}

      //Setup the knots
      for (I = 0; I < (S_KnotRes >> 1); I++) {S_Knots[I] = 0.0f; S_Knots[I + (S_KnotRes >> 1)] = 1.0f;}
      for (I = 0; I < (T_KnotRes >> 1); I++) {T_Knots[I] = 0.0f; T_Knots[I + (T_KnotRes >> 1)] = 1.0f;}

      //Setup the flags and indicate a valid data structure
      Flags |= (NewFlags & NURB_ATTRIB_MASK) | NURB_VALID;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~NurbRec(void)  
      {
      if (S_Knots     != NULL) {delete[] S_Knots;     S_Knots     = NULL;}
      if (T_Knots     != NULL) {delete[] T_Knots;     T_Knots     = NULL;}
      if (Colors      != NULL) {delete[] Colors;      Colors      = NULL;}
      if (TexPoints   != NULL) {delete[] TexPoints;   TexPoints   = NULL;}
      if (Points      != NULL) {delete[] Points;      Points      = NULL;}
      }

   /*-------------------------------------------------------------------------
      Calculates the linear offet (in array entries) for a given matrix 
      coordinate S, T.
     ------------------------------------------------------------------------*/
   __forceinline dword LinOffs(dword S, dword T) {return T*S_Res + S;}

   /*-------------------------------------------------------------------------
      This function sets up the data points necessary for rendering the Nurb.
      It accepts four data values that maps directly to the four corners of the 
      Nurb patch. All the intermediate points are interpolated between the four 
      input data.
   
      TopLeft, 
      TopRight, 
      BottomLeft, 
      BottomRight : The data that maps to the four corners of the Nurb patch.
     ------------------------------------------------------------------------*/
   __forceinline void Interpolate(ColorRec &TopLeft, ColorRec &TopRight, ColorRec &BottomLeft, ColorRec &BottomRight)
      {
      //Ensure that the required data structure exists
      if ((Flags & NURB_COLOR) == NURB_NULL) {return;}
      
      //Find the difference along the left and right edge
      ColorRec d_Left  = (BottomLeft  - TopLeft)  * T_ResInv;
      ColorRec d_Right = (BottomRight - TopRight) * T_ResInv;

      //Setup the starting left and right edge
      ColorRec* DataPtr    = Colors;
      *(DataPtr         )  = TopLeft;
      *(DataPtr+S_Res_m1)  = TopRight;

      for (dword T = 0; T < T_Res; T++)
         {
         //Find the difference along each hozizontal line
         ColorRec d_Mid = (*(DataPtr+S_Res_m1) - *DataPtr) * S_ResInv;
         DataPtr++;                       //Skip starting data

         //Interpolate between start and end data
         for (dword S = 1; S < S_Res_m1; S++) {*DataPtr = *(DataPtr-1) + d_Mid; DataPtr++;}
         DataPtr++;                       //Skip ending data

         //Update the next starting left and right edge
         if (T < T_Res_m1)
            {
            *(DataPtr         ) = *(DataPtr-S_Res) + d_Left;
            *(DataPtr+S_Res_m1) = *(DataPtr-1)     + d_Right;
            }
         }
      }

   /*-------------------------------------------------------------------------
      Behaves like the above function.
     ------------------------------------------------------------------------*/
   __forceinline void Interpolate(TexPointRec &TopLeft, TexPointRec &TopRight, TexPointRec &BottomLeft, TexPointRec &BottomRight)
      {
      //Ensure that the required data structure exists
      if ((Flags & NURB_TEXTURE) == NURB_NULL) {return;}
      
      //Find the difference along the left and right edge
      TexPointRec d_Left  = (BottomLeft  - TopLeft)  * T_ResInv;
      TexPointRec d_Right = (BottomRight - TopRight) * T_ResInv;

      //Setup the starting left and right edge
      TexPointRec* DataPtr = TexPoints;
      *(DataPtr         )  = TopLeft;
      *(DataPtr+S_Res_m1)  = TopRight;

      for (dword T = 0; T < T_Res; T++)
         {
         //Find the difference along each hozizontal line
         TexPointRec d_Mid = (*(DataPtr+S_Res_m1) - *DataPtr) * S_ResInv;
         DataPtr++;                       //Skip starting data

         //Interpolate between start and end data
         for (dword S = 1; S < S_Res_m1; S++) {*DataPtr = *(DataPtr-1) + d_Mid; DataPtr++;}
         DataPtr++;                       //Skip ending data

         //Update the next starting left and right edge
         if (T < T_Res_m1)
            {
            *(DataPtr         ) = *(DataPtr-S_Res) + d_Left;
            *(DataPtr+S_Res_m1) = *(DataPtr-1)     + d_Right;
            }
         }
      }

   /*-------------------------------------------------------------------------
      Behaves like the above function.
     ------------------------------------------------------------------------*/
   __forceinline void Interpolate(PointRec &TopLeft, PointRec &TopRight, PointRec &BottomLeft, PointRec &BottomRight)
      {
      //Ensure that the required data structure exists
      if ((Flags & NURB_POINT) == NURB_NULL) {return;}
      
      //Find the difference along the left and right edge
      PointRec d_Left  = (BottomLeft  - TopLeft)  * T_ResInv;
      PointRec d_Right = (BottomRight - TopRight) * T_ResInv;

      //Setup the starting left and right edge
      PointRec* DataPtr   = Points;
      *(DataPtr         ) = TopLeft;
      *(DataPtr+S_Res_m1) = TopRight;

      for (dword T = 0; T < T_Res; T++)
         {
         //Find the difference along each hozizontal line
         PointRec d_Mid = (*(DataPtr+S_Res_m1) - *DataPtr) * S_ResInv;
         DataPtr++;                       //Skip starting data

         //Interpolate between start and end data
         for (dword S = 1; S < S_Res_m1; S++) {*DataPtr = *(DataPtr-1) + d_Mid; DataPtr++;}
         DataPtr++;                       //Skip ending data

         //Update the next starting left and right edge
         if (T < T_Res_m1)
            {
            *(DataPtr         ) = *(DataPtr-S_Res) + d_Left;
            *(DataPtr+S_Res_m1) = *(DataPtr-1)     + d_Right;
            }
         }
      }

   /*==== End Class =============================================================*/
   };



/*==== End of file ===========================================================*/
#endif