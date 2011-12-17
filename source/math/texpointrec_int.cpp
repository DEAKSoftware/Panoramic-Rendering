/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                  The Texture Point Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __TEXPOINTREC_INT_CPP__
#define __TEXPOINTREC_INT_CPP__



/*---------------------------------------------------------------------------
  The iTexPointRec class.
  ---------------------------------------------------------------------------*/
class iTexPointRec
   {
   /*==== Public Declarations ================================================*/
   public:

   int U, V, W, t;

   /*-------------------------------------------------------------------------
      Default constructor and destructor
     -------------------------------------------------------------------------*/
   iTexPointRec(void) {}
   ~iTexPointRec(void) {}

   /*-------------------------------------------------------------------------
      Copy constant constructor
   /*-------------------------------------------------------------------------*/
   inline iTexPointRec (const int k)// : X(k), V(k), Z(k)
      {
      U = V = W = t = k;
      }

   /*-------------------------------------------------------------------------
      Copy [U,V,W,t] constructor
     -------------------------------------------------------------------------*/
   inline iTexPointRec (const int _U, const int _V, const int _W, const int _t)// : U(_U), V(_V), W(_W), t(_t)
      {
      U = _U;
      V = _V;
      W = _W;
      t = _t;
      }

   /*-------------------------------------------------------------------------
      Copy iTexPointRec constructor
     -------------------------------------------------------------------------*/
   inline iTexPointRec (const iTexPointRec &TexPoint)// : U(TexPoint.U), V(TexPoint.V), W(TexPoint.W), t(TexPoint.t)
      {
      U = TexPoint.U;
      V = TexPoint.V;
      W = TexPoint.W;
      t = TexPoint.t;
      }

   /*-------------------------------------------------------------------------
      Increment
     -------------------------------------------------------------------------*/
   inline iTexPointRec &operator += (const iTexPointRec &TexPoint)
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
   inline iTexPointRec &operator += (const int k)
      {               
      U += k;
      V += k;
      W += k;
      t  = t;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Absolute value. Example, the absolute value of B:

      iTexPointRec F = B.Abs();
     -------------------------------------------------------------------------*/
   inline iTexPointRec Abs(void)
      {
      return iTexPointRec((int)labs(U), (int)labs(V), (int)labs(W), (int)t);
      }

   /*-------------------------------------------------------------------------
      Addition
     -------------------------------------------------------------------------*/
   inline iTexPointRec operator + (const iTexPointRec &TexPoint)
      {
      return iTexPointRec(U + TexPoint.U, V + TexPoint.V, W + TexPoint.W, t);
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   inline iTexPointRec operator + (const int k)
      {
      return iTexPointRec(U + k, V + k, W + k, t);
      }

   /*-------------------------------------------------------------------------
      Neagate
     -------------------------------------------------------------------------*/
   inline iTexPointRec operator - (void)
      {
      return iTexPointRec(-U, -V, -W, t);
      }

   /*-------------------------------------------------------------------------
      Decrement
     -------------------------------------------------------------------------*/
   inline iTexPointRec &operator -= (const iTexPointRec &TexPoint)
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
   inline iTexPointRec &operator -= (const int k)
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
   inline iTexPointRec operator - (const iTexPointRec &TexPoint)
      {
      return iTexPointRec(U - TexPoint.U, V - TexPoint.V, W - TexPoint.W, t);
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   inline iTexPointRec operator - (int k)
      {
      return iTexPointRec(U - k, V - k, W - k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   inline iTexPointRec &operator *= (const iTexPointRec &TexPoint)
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
   inline iTexPointRec &operator *= (const int k)
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
   inline iTexPointRec operator * (const iTexPointRec &TexPoint)
      {
      return iTexPointRec(U * TexPoint.U, V * TexPoint.V, W * TexPoint.W, t);
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   inline iTexPointRec operator * (const int k)
      {
      return iTexPointRec(U * k, V * k, W * k, t);
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   inline iTexPointRec &operator /= (const iTexPointRec &TexPoint)
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
   inline iTexPointRec &operator /= (const int k)
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
   inline iTexPointRec operator / (const iTexPointRec &TexPoint)
      {
      return iTexPointRec(U / TexPoint.U, V / TexPoint.V, W / TexPoint.W, t);
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   inline iTexPointRec operator / (const int k)
      {
      return iTexPointRec(U / k, V / k, W / k, t);
      }
  
  
   /*==== End of Class =======================================================*/
   };




/*==== End of file ===========================================================*/
#endif