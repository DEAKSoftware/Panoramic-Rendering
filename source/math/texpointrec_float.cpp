/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                  The Texture Point Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __TEXPOINTREC_FLOAT_CPP__
#define __TEXPOINTREC_FLOAT_CPP__



/*---------------------------------------------------------------------------
  The TexPointRec class.
  ---------------------------------------------------------------------------*/
class TexPointRec
   {
   /*==== Public Declarations ================================================*/
   public:

   float U, V, W, t;

   /*-------------------------------------------------------------------------
      Default constructor and destructor
     -------------------------------------------------------------------------*/
   TexPointRec(void) {}
   ~TexPointRec(void) {}

   /*-------------------------------------------------------------------------
      Copy constant constructor
   /*-------------------------------------------------------------------------*/
   inline TexPointRec (const float k)// : X(k), V(k), Z(k)
      {
      U = V = W = t = k;
      }

   /*-------------------------------------------------------------------------
      Copy [U,V,W,t] constructor
     -------------------------------------------------------------------------*/
   inline TexPointRec (const float _U, const float _V, const float _W, const float _t)// : U(_U), V(_V), W(_W), t(_t)
      {
      U = _U;
      V = _V;
      W = _W;
      t = _t;
      }

   /*-------------------------------------------------------------------------
      Copy TexPointRec constructor
     -------------------------------------------------------------------------*/
   inline TexPointRec (const TexPointRec &TexPoint)// : U(TexPoint.U), V(TexPoint.V), W(TexPoint.W), t(TexPoint.t)
      {
      U = TexPoint.U;
      V = TexPoint.V;
      W = TexPoint.W;
      t = TexPoint.t;
      }

   /*-------------------------------------------------------------------------
      Increment
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator += (const TexPointRec &TexPoint)
      {
      U += TexPoint.U;
      V += TexPoint.V;
      W += TexPoint.W;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Increment by a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator += (const float k)
      {               
      U += k;
      V += k;
      W += k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Absolute value. Example, the absolute value of B:

      TexPointRec F = B.Abs();
     -------------------------------------------------------------------------*/
   inline TexPointRec Abs(void)
      {
      return TexPointRec((float)fabs(U), (float)fabs(V), (float)fabs(W), t);
      }

   /*-------------------------------------------------------------------------
      Addition
     -------------------------------------------------------------------------*/
   inline TexPointRec operator + (const TexPointRec &TexPoint)
      {
      return TexPointRec(U + TexPoint.U, V + TexPoint.V, W + TexPoint.W, t);
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   inline TexPointRec operator + (const float k)
      {
      return TexPointRec(U + k, V + k, W + k, t);
      }

   /*-------------------------------------------------------------------------
      Neagate
     -------------------------------------------------------------------------*/
   inline TexPointRec operator - (void)
      {
      return TexPointRec(-U, -V, -W, t);
      }

   /*-------------------------------------------------------------------------
      Decrement
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator -= (const TexPointRec &TexPoint)
      {
      U -= TexPoint.U; 
      V -= TexPoint.V; 
      W -= TexPoint.W;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Decrement by a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator -= (const float k)
      {
      U -= k; 
      V -= k; 
      W -= k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Subtraction
     -------------------------------------------------------------------------*/
   inline TexPointRec operator - (const TexPointRec &TexPoint)
      {
      return TexPointRec(U - TexPoint.U, V - TexPoint.V, W - TexPoint.W, t);
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec operator - (float k)
      {
      return TexPointRec(U - k, V - k, W - k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator *= (const TexPointRec &TexPoint)
      {
      U *= TexPoint.U; 
      V *= TexPoint.V; 
      W *= TexPoint.W;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Self multiply with a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator *= (const float k)
      {
      U *= k; 
      V *= k; 
      W *= k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Multiply
     -------------------------------------------------------------------------*/
   inline TexPointRec operator * (const TexPointRec &TexPoint)
      {
      return TexPointRec(U * TexPoint.U, V * TexPoint.V, W * TexPoint.W, t);
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec operator * (const float k)
      {
      return TexPointRec(U * k, V * k, W * k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator /= (const TexPointRec &TexPoint)
      {
      U /= TexPoint.U; 
      V /= TexPoint.V; 
      W /= TexPoint.W;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Self divide by a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec &operator /= (const float k)
      {
      U /= k;
      V /= k;
      W /= k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Divide
     -------------------------------------------------------------------------*/
   inline TexPointRec operator / (const TexPointRec &TexPoint)
      {
      return TexPointRec(U / TexPoint.U, V / TexPoint.V, W / TexPoint.W, t);
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   inline TexPointRec operator / (const float k)
      {
      return TexPointRec(U / k, V / k, W / k, t);
      }
  
  
   /*==== End of Class =======================================================*/
   };




/*==== End of file ===========================================================*/
#endif