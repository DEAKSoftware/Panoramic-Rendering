/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                       The 3D Point Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __POINTREC_INT_CPP__
#define __POINTREC_INT_CPP__



/*---------------------------------------------------------------------------
  The iPointRec class.
  ---------------------------------------------------------------------------*/
class iPointRec
   {
   /*==== Public Declarations ================================================*/
   public:

   int X, Y, Z, t;

   /*-------------------------------------------------------------------------
      Default constructor and destructor
     -------------------------------------------------------------------------*/
   iPointRec(void) {}
   ~iPointRec(void) {}

   /*-------------------------------------------------------------------------
      Copy constant constructor
   /*-------------------------------------------------------------------------*/
   __forceinline iPointRec (const int k)// : X(k), Y(k), Z(k)
      {
      X = Y = Z = t = k;
      }

   /*-------------------------------------------------------------------------
      Copy [X,Y,Z,t] constructor
     -------------------------------------------------------------------------*/
   __forceinline iPointRec (const int _X, const int _Y, const int _Z, const int _t)// : X(_X), Y(_Y), Z(_Z), t(_t)
      {
      X = _X;
      Y = _Y;
      Z = _Z;
      t = _t;
      }

   /*-------------------------------------------------------------------------
      Copy iPointRec constructor
     -------------------------------------------------------------------------*/
   __forceinline iPointRec (const iPointRec &Point)// : X(Point.X), Y(Point.Y), Z(Point.Z), t(Point.t)
      {
      X = Point.X;
      Y = Point.Y;
      Z = Point.Z;
      t = Point.t;
      }

   /*-------------------------------------------------------------------------
      Increment
     -------------------------------------------------------------------------*/
   __forceinline iPointRec &operator += (const iPointRec &Point)
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
   __forceinline iPointRec &operator += (const int k)
      {               
      X += k;
      Y += k;
      Z += k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Absolute value. Example, the absolute value of B:

      iPointRec F = B.Abs();
     -------------------------------------------------------------------------*/
   __forceinline iPointRec Abs(void)
      {
      return iPointRec((int)labs(X), (int)labs(Y), (int)labs(Z), (int)t);
      }

   /*-------------------------------------------------------------------------
      Addition
     -------------------------------------------------------------------------*/
   __forceinline iPointRec operator + (const iPointRec &Point)
      {
      return iPointRec(X + Point.X, Y + Point.Y, Z + Point.Z, t);
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   __forceinline iPointRec operator + (const int k)
      {
      return iPointRec(X + k, Y + k, Z + k, t);
      }

   /*-------------------------------------------------------------------------
      Neagate
     -------------------------------------------------------------------------*/
   __forceinline iPointRec operator - (void)
      {
      return iPointRec(-X, -Y, -Z, t);
      }

   /*-------------------------------------------------------------------------
      Decrement
     -------------------------------------------------------------------------*/
   __forceinline iPointRec &operator -= (const iPointRec &Point)
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
   __forceinline iPointRec &operator -= (const int k)
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
   __forceinline iPointRec operator - (const iPointRec &Point)
      {
      return iPointRec(X - Point.X, Y - Point.Y, Z - Point.Z, t);
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   __forceinline iPointRec operator - (int k)
      {
      return iPointRec(X - k, Y - k, Z - k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   __forceinline iPointRec &operator *= (const iPointRec &Point)
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
   __forceinline iPointRec &operator *= (const int k)
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
   __forceinline iPointRec operator * (const iPointRec &Point)
      {
      return iPointRec(X * Point.X, Y * Point.Y, Z * Point.Z, t);
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline iPointRec operator * (const int k)
      {
      return iPointRec(X * k, Y * k, Z * k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   __forceinline iPointRec &operator /= (const iPointRec &Point)
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
   __forceinline iPointRec &operator /= (const int k)
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
   __forceinline iPointRec operator / (const iPointRec &Point)
      {
      return iPointRec(X / Point.X, Y / Point.Y, Z / Point.Z, t);
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline iPointRec operator / (const int k)
      {
      return iPointRec(X / k, Y / k, Z / k, t);
      }
  
  
   /*==== End of Class =======================================================*/
   };




/*==== End of file ===========================================================*/
#endif