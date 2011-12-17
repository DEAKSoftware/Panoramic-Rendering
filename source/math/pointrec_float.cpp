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
      X += Point.X;
      Y += Point.Y;
      Z += Point.Z;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Increment by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator += (const float k)
      {               
      X += k;
      Y += k;
      Z += k;
      t  = t;
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
      return PointRec(X + Point.X, Y + Point.Y, Z + Point.Z, t);
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator + (const float k)
      {
      return PointRec(X + k, Y + k, Z + k, t);
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
      X -= Point.X; 
      Y -= Point.Y; 
      Z -= Point.Z;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Decrement by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator -= (const float k)
      {
      X -= k; 
      Y -= k; 
      Z -= k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Subtraction
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator - (const PointRec &Point)
      {
      return PointRec(X - Point.X, Y - Point.Y, Z - Point.Z, t);
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator - (float k)
      {
      return PointRec(X - k, Y - k, Z - k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator *= (const PointRec &Point)
      {
      X *= Point.X; 
      Y *= Point.Y; 
      Z *= Point.Z;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Self multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator *= (const float k)
      {
      X *= k; 
      Y *= k; 
      Z *= k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Multiply
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator * (const PointRec &Point)
      {
      return PointRec(X * Point.X, Y * Point.Y, Z * Point.Z, t);
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator * (const float k)
      {
      return PointRec(X * k, Y * k, Z * k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator /= (const PointRec &Point)
      {
      X /= Point.X; 
      Y /= Point.Y; 
      Z /= Point.Z;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Self divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec &operator /= (const float k)
      {
      X /= k;
      Y /= k;
      Z /= k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Divide
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator / (const PointRec &Point)
      {
      return PointRec(X / Point.X, Y / Point.Y, Z / Point.Z, t);
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline PointRec operator / (const float k)
      {
      return PointRec(X / k, Y / k, Z / k, t);
      }
  
   /*-------------------------------------------------------------------------
      Find the magnitude squared. Example, the magnitude squared of C:  
      
      float M = C.MagSqr();
     -------------------------------------------------------------------------*/
   __forceinline float MagSqr(void) const
      {
      return (X*X + Y*Y + Z*Z);
      }
   
   /*-------------------------------------------------------------------------
      Find the magnitude. Example, the magnitude of C:  
      
      float M = C.Mag();
     -------------------------------------------------------------------------*/
   __forceinline float Mag(void) const
      {
      return (float)sqrt(X*X + Y*Y + Z*Z);
      }
   
   /*-------------------------------------------------------------------------
      Find the unit vector. Example, the unit vector of C:  
      
      PointRec U = C.Unit();
     -------------------------------------------------------------------------*/
   __forceinline PointRec Unit(void)
      {
      float Magnitude = this->Mag();
      if (Magnitude != 0.0) {return *this * (1.0f / Magnitude);}
      return *this;
      //return *this * (1.0f / this->Mag());
      }
   
   /*-------------------------------------------------------------------------
      Returns the dot product of *this and Point. Example, the dot product 
      of B and Point:

      float D = B.Dot(Point);
     -------------------------------------------------------------------------*/
   __forceinline float Dot(const PointRec &Point)
      {
      return (X*Point.X + Y*Point.Y + Z*Point.Z);
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
      }

   /*-------------------------------------------------------------------------
      Returns the cosine value between vectors *this and Point. Example, the
      cosine of B and Point: 
      
      float C = B.COS(Point);
      
      To find the angle, the inverse of COS must be applied on this function's 
      result.
     -------------------------------------------------------------------------*/
   __forceinline float COS(const PointRec &Point)
      {
      return (this->Dot(Point) / (this->Mag() * Point.Mag()));
      }

   /*-------------------------------------------------------------------------
      Returns the angle (in radians) between vectors *this and Point. Example,
      the cosine of B and Point: 
      
      float C = B.AngleCOS(Point);
     -------------------------------------------------------------------------*/
   __forceinline float AngleCOS(const PointRec &Point)
      {
      return (float)acos(this->COS(Point));
      }

   /*-------------------------------------------------------------------------
      Returns the angle (in radians) between vectors *this and Point. It uses 
      the dot product to calculate the angle, and therefore this is only 
      applicable for unit vectors. Example, the cosine of B and Point: 
      
      float C = B.AngleDot(Point);
     -------------------------------------------------------------------------*/
   __forceinline float AngleDot(const PointRec &Point)
      {
      return (float)acos(this->Dot(Point));
      }

   /*-------------------------------------------------------------------------
      Computes the sine and cosine values of *this [X, Y, Z].
     -------------------------------------------------------------------------*/
   __forceinline void CalcSinCos(PointRec &SinR, PointRec &CosR)
      {
      #if defined (INTEL_x86)
      __asm {
            mov esi,[this]
            mov eax,[CosR]
            mov ebx,[SinR]
            fld DWORD PTR [esi]
            fsincos
            fstp DWORD PTR [eax]
            fstp DWORD PTR [ebx]

            fld DWORD PTR [esi+4]
            fsincos
            fstp DWORD PTR [eax+4]
            fstp DWORD PTR [ebx+4]

            fld DWORD PTR [esi+8]
            fsincos
            fstp DWORD PTR [eax+8]
            fstp DWORD PTR [ebx+8]
            }
      #else
      CosR = PointRec((float)cos(X), (float)cos(Y), (float)cos(Z), 0.0f);
      SinR = PointRec((float)sin(X), (float)sin(Y), (float)sin(Z), 0.0f);
      #endif
      }

   /*-------------------------------------------------------------------------
      Returns the X Y Z direction angles (in radians) between vectors *this 
      and Point. It uses  the dot product to calculate the angles, and therefore 
      this is only applicable for unit vectors. Example, the direction angles of 
      B and Point: 
      
      PointRec C = B.DirAngles(Point);
     -------------------------------------------------------------------------*/
   __forceinline PointRec DirAngles(const PointRec &Point)
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
   __forceinline PointRec Rotate(const PointRec &R)
      {
      //Pre-calculate the sine and cosine values of R[X,Y,Z]
      #if defined (INTEL_x86)
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
      #else
      PointRec CosR((float)cos(R.X), (float)cos(R.Y), (float)cos(R.Z), 0.0f);
      PointRec SinR((float)sin(R.X), (float)sin(R.Y), (float)sin(R.Z), 0.0f);
      #endif

      PointRec T;
      T.Y = Y * CosR.X - Z   * SinR.X; 
      T.Z = Z * CosR.X + Y   * SinR.X; 
      T.X = X * CosR.Y + T.Z * SinR.Y; 

      return PointRec(
                     (T.X * CosR.Z - T.Y * SinR.Z),
                     (T.Y * CosR.Z + T.X * SinR.Z),
                     (T.Z * CosR.Y - X   * SinR.Y), t
                     );
      /**/
      /*
      PointRec T;

      //Around X
      T.X =    X;
      T.Y =    Y*CosR.X +   Z*SinR.X;
      T.Z =   -Y*SinR.X +   Z*CosR.X;

      //Around Y
      T.X =  T.X*CosR.Y - T.Z*SinR.Y;
      T.Z =  T.X*SinR.Y + T.Z*CosR.Y;

      //Around Z
      T.X =  T.X*CosR.Z + T.Y*SinR.Z;
      T.Y = -T.X*SinR.Z + T.Y*CosR.Z;  

      return T
      /**/
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