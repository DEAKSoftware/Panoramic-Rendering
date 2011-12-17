/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                  Panoramic Transformation Functions                        */
/*============================================================================*/


/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __TRANSFORM_CPP__
#define __TRANSFORM_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../math/equsolver.cpp"
#include "../video_io/video.cpp"


//#define TRANSFORM_SLOW


/*---------------------------------------------------------------------------
  The Transform class.
  ---------------------------------------------------------------------------*/
class TransformClass
   {
   /*==== Public Declarations ================================================*/
   private:

   struct DegRec
      {
      float R;
      float Z;
      };

   DegRec*  DegTable;                              //Fast lookup table for profile curve transformation
   int      DegRes;
   float    DegResInv;
   int      ArrayShift;
   int      ArraySize;                             //Number of table entries.
   float    z_max;                                 //Z value at f(r_max)
   float    r_max;                                 //r_max clipper
   float    v_m_lim;                               //View vector gradient limit at r_max

   #if defined (TRANSFORM_SLOW)
   byte*    LocalProfCode;
   #endif


   /*==== Public Declarations ================================================*/
   public:

   /*---- Constructor --------------------------------------------------------*/
   TransformClass(void)
      {
      DegTable   = NULL;
      DegRes     = 0;
      DegResInv  = 0.0f;
      ArrayShift = 0;
      ArraySize  = 0;
      z_max      = 1.0f;
      r_max      = 1.0f;
      v_m_lim    = 1.0f;

      #if defined (TRANSFORM_SLOW)
      LocalProfCode = NULL;
      #endif
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~TransformClass(void)
      {
      if (DegTable != NULL) {free(DegTable); DegTable = NULL;}
      }

   /*-------------------------------------------------------------------------
      Setup the profile curve table.

      NOTE: The for projecting points outside r_max, we use a tangent line to
      extend the profile curve. The tangent gadient (m_max) at r_max of the 
      profile curve is given by:
         m_max = (z_d - z_max) / (r_d - r_max),

      where [r_d, z_d] is a small displacement point from [r_max, z_max].
      Therefore the tangent is:
         z = m_max(r - r_max) + z_max.
      
      Intersection of a view vector with the tangent:
         z = m*r                          //the view vector
         z = m_max(r - r_max) + z_max     //the tangent vector
         
      Intersetion occurs when they are equal
                         m*r = m_max(r - r_max) + z_max
                           0 = m_max*r - m_max*r_max + z_max - m*r
                           0 = r(m_max - m) - m_max*r_max + z_max
         m_max*r_max - z_max = r(m_max - m)
                           r = (m_max*r_max - z_max) / (m_max - m)
      
      The intersection occurs only if : (m_max - m) < 0, otherwise 
      r = extreme value.
     ------------------------------------------------------------------------*/
     bool InitTable(VideoClass* Video, byte* ProfCode, float ProfEquLim, dword NewDegRes)
      {
      if ((Video == NULL) || (ProfCode == NULL)) {return false;}

      //Ensure that the Video mode is valid.
      if (!Video->ModeValid) {return false;}
      
      //Notes on DegTable: Forward gradients DegTable[0] to DegTable[ArrayShift-1],
      // backard gradients DegTable[ArrayShift+1] to DegTable[ArraySize-1].
      DegRes     = (NewDegRes > 0) ? NewDegRes : 1;
      DegResInv  = 1.0f / (float)DegRes;
      ArrayShift = 90  * DegRes;
      ArraySize  = 180 * DegRes + 1;
      
      //(Re)allocate the profile curve table
      DegRec* TempPtr = (DegRec*)realloc(DegTable, ArraySize*sizeof(DegRec));
      if (TempPtr == NULL) {return false;}
      DegTable = TempPtr;


      //Compute maximums
      r_max      = ProfEquLim;
      EquSolver.Execute(z_max, ProfCode, r_max);
      v_m_lim    = (r_max != 0.0f) ? (z_max / r_max) : ((z_max < 0.0f) ? -10000.0f : 10000.0f);

      
      //Find the tangent just before the limit
      float z_d, r_d = r_max-0.1;
      EquSolver.Execute(z_d, ProfCode, r_d);
      float m_max = (z_d - z_max) / (r_d - r_max);

      #if defined (TRANSFORM_SLOW)
      LocalProfCode = ProfCode;
      #endif

      //---- Handle forward and backward view vectors 
      //     (both with and infinite gradient). ----
      DegTable[0].R           =  0.0f;
      DegTable[0].Z           =  1.0f;
      DegTable[ArraySize-1].R =  100.0f;
      DegTable[ArraySize-1].Z = -1.0f;
      
      //---- Setup the table ----
      float view_hyp = 1.0f;
      for (int Deg = 1; Deg < ArraySize-1; Deg++)
         {
         float Angle  = deg2rad((float)(ArrayShift-Deg) * DegResInv);
         float view_r = cos(Angle) * view_hyp;
         float view_z = sin(Angle) * view_hyp;
         float m      = view_z / view_r;

         //Compute the r and z intercept for the interval [0, r_max]
         if (m >= v_m_lim)
            {
            DegTable[Deg].R = ProfCurve_FindRoot(m, ProfCode, ProfEquLim);
            EquSolver.Execute(DegTable[Deg].Z, ProfCode, DegTable[Deg].R);            
            }

         //Compute the r and z intercept for the interval [r_max, inf]
         else 
            {
            float dm = (m_max - m);
            DegTable[Deg].R = (dm < 0.0) ? ((m_max*r_max - z_max) / dm) : 100.0f;
            DegTable[Deg].Z = m * DegTable[Deg].R;
            }
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Finds the root of the profile curve, according to the gradient of the
      view vector. The view vector is defined in the profile curve space:
      
         z = m*r.
      
      m is the gradient of the 3D view vector V in the profile curve space:
      
         m = V.Z / sqrt(sqr(V.X) + sqr(V.Y)).
      
      The profile curve is given by z = f(r). By finding the root of the 
      profile curve, we can determine the r intersection of the view vector. 
      This is true when
      
            m*r = f(r)  =>  0 = f(r) - m*r,  for r >= 0.

      The root is solved with the iterative bisection method. The root can be 
      used as a transformation constant for a given 3D point, which 
      essentially maps the 3D point onto the surface.
     ------------------------------------------------------------------------*/
   inline float ProfCurve_FindRoot(float m, byte* ProfCode, float ProfEquLim)
      {
      dword Iteration = 0;                            //Loop counter
      float r1 = 0.0f;                                //r1 and r2 are the extreme profile curve range
      float r2 = ProfEquLim;
      float r;                                        //Estimated r intersection point (mid point of r1 and r2)
      float z;

      //-- Find the root r, (accurate to 4 decimal places). If someting strange 
      //   happens, the loop stops after 100 iterations. --
      while ((fabs(r1 - r2) * 0.5f > 0.00001f) && (Iteration < 100))
         {
         r = (r1 + r2) * 0.5f;                        //Find the mid point
         EquSolver.Execute(z, ProfCode, r);
         
         //If z is not a number, set z to float_MAX. Note that this
         // strange comparison is inteded to test for NaN. If we use
         // standard float comparison, an exception is generated, because
         // you can't load NaNs into the FPU directly.
         if (*(dword*)&z == *(dword*)&_NaN) {z = float_MAX;}
         
         z -= m*r;                                    //Find it's z value
         if (z > 0.0f) {r1 = r;} else {r2 = r;}       //Narrow the range accoding to the sign of z

         Iteration++;
         }

      return r;
      }
 
   /*-------------------------------------------------------------------------
      Same as above, but the vector is in the form of   z = m*(r - r_offs).
     ------------------------------------------------------------------------*/
   inline float ProfCurve_FindRoot(float m, float r_offs, byte* ProfCode, float ProfEquLim)
      {
      dword Iteration = 0;                            //Loop counter
      float r1 = 0.0f;                                //r1 and r2 are the extreme profile curve range
      float r2 = ProfEquLim;
      float r;                                        //Estimated r intersection point (mid point of r1 and r2)
      float z;

      //-- Find the root r, (accurate to 4 decimal places). If someting strange 
      //   happens, the loop stops after 100 iterations. --
      while ((fabs(r1 - r2) * 0.5f > 0.00001f) && (Iteration < 100))
         {
         r = (r1 + r2) * 0.5f;                        //Find the mid point
         EquSolver.Execute(z, ProfCode, r);
         
         //If z is not a number, set z to float_MAX. Note that this
         // strange comparison is inteded to test for NaN. If we use
         // standard float comparison, an exception is generated, because
         // you can't load NaNs into the FPU directly.
         if (*(dword*)&z == *(dword*)&_NaN) {z = float_MAX;}
         
         z -= m*(r - r_offs);                         //Find it's z value
         if (z > 0.0f) {r1 = r;} else {r2 = r;}       //Narrow the range accoding to the sign of z

         Iteration++;
         }

      return r;
      }
 
   /*-----------------------f--------------------------------------------------
      This function transforms the input point according to the profile curve.
      Returns true if the point after the transformation is visible.

      tPoint   : Transformed point
      Point    : Input point
     ------------------------------------------------------------------------*/
   __forceinline bool ProfCurve_Transform(PointRec &tPoint, PointRec &Point)
      {
      if (DegTable == NULL) {return false;}
      
      //Use the X and Y coordinates to find the radius, which is used 
      // to find the gradient of the view vector (in profile curve space).
      #define V (Point)
      float SqrMag_XY = sqr(V.X) + sqr(V.Y);
      float R         = sqrt(SqrMag_XY);

      //Compute the inverse of R to avoid divisions
      R = (R != 0.0f) ? (1.0f / R) : 1.0f;

      //Compute the angle of the view vector in degrees, which is used
      // as the lookup table index. This is done by computing the 
      // arc tangent of the view vector gradient.
      #if defined (TRANSFORM_SLOW)
         int Deg = 0;
         DegTable[Deg].R = ProfCurve_FindRoot(V.Z * R, LocalProfCode, r_max);
      #else
         int Deg = ArrayShift - (int)(rad2deg(atan(V.Z * R)) * (float)DegRes);
      #endif

      SystemFlags.AccumPoints++;

      //The curved display coordinates is obtained by scaling the V[X,Y] to 
      // unit coordinates and then further scaling them by the r-Int value.
      R *= DegTable[Deg].R;
      tPoint.X = V.X * R;
      tPoint.Y = V.Y * R;

      //The depth value Z becomes the magnitude of the V vector. This 
      // is used by OpenGL for Z-buffering.
      tPoint.Z = sqrt(SqrMag_XY + sqr(V.Z)) * 2.0f; //Increase magnitude size by 2 (gives better point "sort" in z buffer)
      
      //rInt value is stored in tPoint->t.
      tPoint.t = DegTable[Deg].R;
      
      //If r-Int spans outside the clipping region, the point is flagged 
      // invisible.
      return (DegTable[Deg].R < r_max);
      #undef V
      }

   /*-------------------------------------------------------------------------
      This function transforms the input point according to the profile curve.
      It also performs projector compensation. Returns true if the point 
      after the transformation is visible.

      tPoint   : Transformed point
      Point    : Input point
      d        : The Z dinstance of the projector from the origin. Must be 
                 grater than 0.0f.
     ------------------------------------------------------------------------*/
   __forceinline bool ProfCurve_Transform(PointRec &tPoint, PointRec &Point, float d)
      {
      if (DegTable == NULL) {return false;}
      
      //Use the X and Y coordinates to find the radius, which is used 
      // to find the gradient of the view vector (in profile curve space).
      #define V (Point)
      float SqrMag_XY = sqr(V.X) + sqr(V.Y);
      float R         = sqrt(SqrMag_XY);

      //Compute the inverse of R to avoid divisions
      R = (R != 0.0f) ? (1.0f / R) : 1.0f;

      //Compute the angle of the view vector in degrees, which is used
      // as the lookup table index. This is done by computing the 
      // arc tangent of the view vector gradient.
      #if defined (TRANSFORM_SLOW)
         int Deg = 0;
         DegTable[Deg].R = ProfCurve_FindRoot(V.Z * R, LocalProfCode, r_max);
      #else
         int Deg = ArrayShift - (int)(rad2deg(atan(V.Z * R)) * (float)DegRes);
      #endif

      SystemFlags.AccumPoints++;

      //The curved display coordinates is obtained by scaling the V[X,Y,Z] to 
      // unit coordinates and then further scaling them by the m-Int value.
      R *= DegTable[Deg].R;
      tPoint.X = V.X * R;
      tPoint.Y = V.Y * R;

      //Projector compensation (only for points that are in front ot d)
      float t = (DegTable[Deg].Z > d) ? fabs(d / (DegTable[Deg].Z - d)) : 100.0f;
      tPoint.X *= t;
      tPoint.Y *= t;

      //The depth value Z becomes the magnitude of the V vector. This 
      // is used by OpenGL for Z-buffering.
      tPoint.Z = sqrt(SqrMag_XY + sqr(V.Z)) * 2.0f;  //Increase magnitude size by 2 (gives better point "sort" in z buffer)
      
      //rInt value is stored in tPoint->t.
      tPoint.t = DegTable[Deg].R;
      
      //If r-Int spans outside the clipping region, the point is flagged 
      // invisible.
      return (DegTable[Deg].R < r_max);
      #undef V
      }

   /*-------------------------------------------------------------------------
      Performs a perspective view compensation for a panoramic transformed 
      point.
     ------------------------------------------------------------------------*/
   inline PointRec Compensate(PointRec &Point)
      {
      //return Point;
      return PointRec(Point.X*fabs(Point.Z), Point.Y*fabs(Point.Z), Point.Z, Point.t);
      }

   /*==== End of Class =======================================================*/
   };


/*==== End of file ===========================================================*/
#endif
