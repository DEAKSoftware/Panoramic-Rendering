/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                       The 3D Point Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __POINTREC_FLOAT_CPP__
#define __POINTREC_FLOAT_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "xmmintrin.h"
#include "fvec.h"


/*---------------------------------------------------------------------------
  The PointRec class.
  ---------------------------------------------------------------------------*/
class PointRec
   {
   /*==== Public Declarations ================================================*/
   public:

   float X, Y, Z, t;

   /*-------------------------------------------------------------------------
      Default constructor and destructor
     -------------------------------------------------------------------------*/
   PointRec(void) {}
   ~PointRec(void) {}

   /*-------------------------------------------------------------------------
      Copy constant constructor
   /*-------------------------------------------------------------------------*/
   __forceinline PointRec (const float k)// : X(k), Y(k), Z(k)
      {
      X = Y = Z = t = k;
      }

   /*-------------------------------------------------------------------------
      Copy [X,Y,Z,t] constructor
     -------------------------------------------------------------------------*/
   __forceinline PointRec (const float _X, const float _Y, const float _Z, const float _t)// : X(_X), Y(_Y), Z(_Z), t(_t)
      {
      X = _X;
      Y = _Y;
      Z = _Z;
      t = _t;
      }

   /*-------------------------------------------------------------------------
      Copy PointRec constructor
     -------------------------------------------------------------------------*/
   __forceinline PointRec (const PointRec &Point)// : X(Point.X), Y(Point.Y), Z(Point.Z), t(Point.t)
      {
      X = Point.X;
      Y = Point.Y;
      Z = Point.Z;
      t = Point.t;
      }

   /*-------------------------------------------------------------------------
      Increment
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator += (const PointRec &Point)
      {
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            addps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Increment by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator += (const float k)
      {
      __asm {
            mov esi,[this]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [esp]
            addps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            add esp,16
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Absolute value. Example, the absolute value of B:

      PointRec F = B.Abs();
     -------------------------------------------------------------------------*/
   __forceinline PointRec Abs(void)
      {
      return PointRec((float)fabs(X), (float)fabs(Y), (float)fabs(Z), t);
      }

   /*-------------------------------------------------------------------------
      Addition
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator + (const PointRec &Point)
      {
      PointRec Temp;
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            addps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator + (const float k)
      {
      PointRec Temp(k, k, k, k);
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [Temp]
            addps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }

   /*-------------------------------------------------------------------------
      Neagate
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator - (void)
      {
      return PointRec(-X, -Y, -Z, t);
      }

   /*-------------------------------------------------------------------------
      Decrement
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator -= (const PointRec &Point)
      {
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            subps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Decrement by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator -= (const float k)
      {
      __asm {
            mov esi,[this]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [esp]
            subps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            add esp,16
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Subtraction
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator - (const PointRec &Point)
      {
      PointRec Temp;
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            subps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator - (float k)
      {
      PointRec Temp(k, k, k, k);
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [Temp]
            subps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator *= (const PointRec &Point)
      {
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            mulps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Self multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator *= (const float k)
      {
      __asm {
            mov esi,[this]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [esp]
            mulps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            add esp,16
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Multiply
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator * (const PointRec &Point)
      {
      PointRec Temp;
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            mulps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator * (const float k)
      {
      PointRec Temp(k, k, k, k);
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [Temp]
            mulps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator /= (const PointRec &Point)
      {
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            divps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Self divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator /= (const float k)
      {
      __asm {
            mov esi,[this]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            push DWORD PTR [k]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [esp]
            divps  xmm0,xmm1
            movups XMMWORD PTR [esi],xmm0
            add esp,16
            }

      return *this;
      }

   /*-------------------------------------------------------------------------
      Divide
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator / (const PointRec &Point)
      {
      PointRec Temp;
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            divps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator / (const float k)
      {
      PointRec Temp(k, k, k, k);
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [Temp]
            divps  xmm0,xmm1
            movups XMMWORD PTR [Temp],xmm0
            }

      return Temp;
      }
  
   /*-------------------------------------------------------------------------
      Find the magnitude squared. Example, the magnitude squared of C:  
      
      float M = C.MagSqr();
     -------------------------------------------------------------------------*/
   float __forceinline MagSqr(void) const
      {
      float Temp;
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            mulps  xmm0,xmm0
            sub esp,16
            movups XMMWORD PTR [esp],xmm0
            
            fld  DWORD PTR [esp]
            fadd DWORD PTR [esp+4]
            fadd DWORD PTR [esp+8]
            add esp,16
            fstp DWORD PTR [Temp]
            }
      
      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Find the magnitude. Example, the magnitude of C:  
      
      float M = C.Mag();
     -------------------------------------------------------------------------*/
   float __forceinline Mag(void) const
      {
      float Temp;
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            mulps  xmm0,xmm0
            sub esp,16
            movups XMMWORD PTR [esp],xmm0
            
            fld  DWORD PTR [esp]
            fadd DWORD PTR [esp+4]
            fadd DWORD PTR [esp+8]
            fsqrt
            add esp,16
            fstp DWORD PTR [Temp]
            }
      
      return Temp;
      }
   
   /*-------------------------------------------------------------------------
      Find the unit vector. Example, the unit vector of C:  
      
      PointRec U = C.Unit();
     -------------------------------------------------------------------------*/
   __forceinline PointRec Unit(void)
      {
      float Magnitude;// = this->Mag();
      __asm {
            mov esi,[this]
            movups xmm0,XMMWORD PTR [esi]
            mulps  xmm0,xmm0
            sub esp,16
            movups XMMWORD PTR [esp],xmm0
            
            fld  DWORD PTR [esp]
            fadd DWORD PTR [esp+4]
            fadd DWORD PTR [esp+8]
            fsqrt
            add esp,16
            fstp DWORD PTR [Magnitude]
            }

      if (Magnitude != 0.0) 
         {
         __asm {
               fld1 
               fdiv DWORD PTR [Magnitude]
               fstp DWORD PTR [Magnitude]
               }
         
         PointRec Temp(Magnitude, Magnitude, Magnitude, Magnitude);

         __asm {
               mov esi,[this]
               movups xmm0,XMMWORD PTR [esi]
               movups xmm1,XMMWORD PTR [Temp]
               mulps  xmm0,xmm1
               movups XMMWORD PTR [Temp],xmm0
               }

         return Temp;
         }
      return *this;
      }
   
   /*-------------------------------------------------------------------------
      Returns the dot product of *this and Point. Example, the dot product 
      of B and Point:

      float D = B.Dot(Point);
     -------------------------------------------------------------------------*/
   float __forceinline Dot(const PointRec &Point)
      {
      return (X*Point.X + Y*Point.Y + Z*Point.Z);
/*
      float Temp;
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            mulps  xmm0,xmm1
            movaps xmm1,xmm0//fix!
            shufps xmm1,xmm1,4Eh
            addps  xmm0,xmm1
            movaps xmm1,xmm0
            shufps xmm1,xmm1,11h
            addps  xmm0,xmm1
            movss  DWORD PTR [Temp],xmm0
            }
      
      return Temp;
/**/
      }

   /*-------------------------------------------------------------------------
      Returns the cross product of *this and Point. Example, the cross product 
      of B and Point:

      PointRec D = B.Cross(Point);
     -------------------------------------------------------------------------*/
   __forceinline PointRec Cross(const PointRec &Point)
      {
      return PointRec((Y*Point.Z - Z*Point.Y), 
                      (Z*Point.X - X*Point.Z), 
                      (X*Point.Y - Y*Point.X), t);
      /**/

      /*
      PointRec Temp;
      __asm {
            mov esi,[this]
            mov edi,[Point]
            movups xmm0,XMMWORD PTR [esi]
            movups xmm1,XMMWORD PTR [edi]
            shufps xmm2,xmm0,0x01         //this
            shufps xmm3,xmm1,0x01         //Point
            mulps  xmm2,xmm3

            }
      
      return Temp;
      /**/
      }

   /*-------------------------------------------------------------------------
      Returns the cosine value between vectors *this and Point. Example, the
      cosine of B and Point: 
      
      float C = B.COS(Point);
      
      To find the angle, the inverse of COS must be applied on this function's 
      result.
     -------------------------------------------------------------------------*/
   float __forceinline COS(const PointRec &Point)
      {
      return (this->Dot(Point) / (this->Mag() * Point.Mag()));
      }

   /*-------------------------------------------------------------------------
      Returns the angle (in radians) between vectors *this and Point. Example,
      the cosine of B and Point: 
      
      float C = B.AngleCOS(Point);
     -------------------------------------------------------------------------*/
   float inline AngleCOS(const PointRec &Point)
      {
      return (float)acos(this->COS(Point));
      }

   /*-------------------------------------------------------------------------
      Returns the angle (in radians) between vectors *this and Point. It uses 
      the dot product to calculate the angle, and therefore this is only 
      applicable for unit vectors. Example, the cosine of B and Point: 
      
      float C = B.AngleDot(Point);
     -------------------------------------------------------------------------*/
   float inline AngleDot(const PointRec &Point)
      {
      return (float)acos(this->Dot(Point));
      }

   /*-------------------------------------------------------------------------
      Returns the X Y Z direction angles (in radians) between vectors *this 
      and Point. It uses  the dot product to calculate the angles, and therefore 
      this is only applicable for unit vectors. Example, the direction angles of 
      B and Point: 
      
      PointRec C = B.DirAngles(Point);
     -------------------------------------------------------------------------*/
   inline PointRec DirAngles(const PointRec &Point)
      {
      PointRec R;

      PointRec VA(0.0f, Y, Z, 0.0f);
      PointRec VB(0.0f, Point.Y, Point.Z, 0.0f);
      R.X = VA.AngleDot(VB);

      VA = PointRec(0.0f, X, 0.0f, Z);
      VB = PointRec(0.0f, Point.X, 0.0f, Point.Z);
      R.Y = VA.AngleDot(VB);

      VA = PointRec(0.0f, X, Y, 0.0f);
      VB = PointRec(0.0f, Point.X, Point.Y, 0.0f);
      R.Z = VA.AngleDot(VB);
      
      R.t = 0.0f;

      return R;
      }

   /*-------------------------------------------------------------------------
      Rotates *this by R (in radians) around the point [0.0, 0.0, 0.0].
      Example, the rotation of B by R: 
      
      PointRec P = B.Rotate(R);

      Algorithm:

      //Rotated around X-axis 
      T.Y     = Y   * CosR.X - Z   * SinR.X; 
      T.Z     = Z   * CosR.X + Y   * SinR.X; 
      //Rotated around Y-axis 
      Point.Z = T.Z * CosR.Y - X   * SinR.Y; //Final Z 
      T.X     = X   * CosR.Y + T.Z * SinR.Y; 
      //Rotated around Z-axis 
      Point.X = T.X * CosR.Z - T.Y * SinR.Z; //Final X 
      Point.Y = T.Y * CosR.Z + T.X * SinR.Z; //Final Y
	   Point.t = 0.0f;
     -------------------------------------------------------------------------*/
   inline PointRec Rotate(const PointRec &R)
      {
      //Pre-calculate the sine and cosine values of R[X,Y,Z]
      PointRec CosR;
      PointRec SinR;
      __asm {
            mov esi,[R]
            fld DWORD PTR [esi]
            fsincos
            fstp DWORD PTR [CosR]PointRec.X
            fstp DWORD PTR [SinR]PointRec.X

            fld DWORD PTR [esi+4]
            fsincos
            fstp DWORD PTR [CosR]PointRec.Y
            fstp DWORD PTR [SinR]PointRec.Y

            fld DWORD PTR [esi+8]
            fsincos
            fstp DWORD PTR [CosR]PointRec.Z
            fstp DWORD PTR [SinR]PointRec.Z
            }

      PointRec T;
      T.Y = Y * CosR.X - Z   * SinR.X; 
      T.Z = Z * CosR.X + Y   * SinR.X; 
      T.X = X * CosR.Y + T.Z * SinR.Y; 

      return PointRec(
                     (T.X * CosR.Z - T.Y * SinR.Z),
                     (T.Y * CosR.Z + T.X * SinR.Z),
                     (T.Z * CosR.Y - X   * SinR.Y), t
                     );
      }

   /*-------------------------------------------------------------------------
      Rotates *this by R (in radians) around the point C.
      Example, the rotation of B by R, around C: 
      
      PointRec P = B.Rotate(R, C);
     -------------------------------------------------------------------------*/
   __forceinline PointRec Rotate(const PointRec &R, const PointRec &C)
      {
      *this -= C;
      return this->Rotate(R) + C;
      }


   /*==== End of Class =======================================================*/
   };



/*==== End of file ===========================================================*/
#endif