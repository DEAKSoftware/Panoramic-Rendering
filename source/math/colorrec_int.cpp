/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                          The Color Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __COLORREC_INT_CPP__
#define __COLORREC_INT_CPP__


/*---------------------------------------------------------------------------
  The iColorRec class.
  ---------------------------------------------------------------------------*/
class iColorRec
   {
   /*==== Public Declarations ================================================*/
   public:

   int R, G, B, A;

   /*-------------------------------------------------------------------------
      Default constructor and destructor
     -------------------------------------------------------------------------*/
   iColorRec(void) {}
   ~iColorRec(void) {}

   /*-------------------------------------------------------------------------
      Copy constant constructor
   /*-------------------------------------------------------------------------*/
   __forceinline iColorRec (const int k)// : R(k), G(k), B(k)
      {
      R = G = B = A = k;
      }

   /*-------------------------------------------------------------------------
      Copy [R,G,B,A] constructor
     -------------------------------------------------------------------------*/
   __forceinline iColorRec (const int _R, const int _G, const int _B, const int _A)// : R(_R), G(_G), B(_B), A(_A)
      {
      R = _R;
      G = _G;
      B = _B;
      A = _A;
      }

   /*-------------------------------------------------------------------------
      Copy iColorRec constructor
     -------------------------------------------------------------------------*/
   __forceinline iColorRec (const iColorRec &Color)// : R(Color.R), G(Color.G), B(Color.B), A(Color.A)
      {
      R = Color.R;
      G = Color.G;
      B = Color.B;
      A = Color.A;
      }

   /*-------------------------------------------------------------------------
      Increment
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator += (const iColorRec &Color)
      {               
      R += Color.R;
      G += Color.G;
      B += Color.B;
      A += Color.A;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Increment by a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator += (const int k)
      {               
      R += k;
      G += k;
      B += k;
      A += k;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Absolute value. Example, the absolute value of B:

      iColorRec F = B.Abs();
     -------------------------------------------------------------------------*/
   __forceinline iColorRec Abs(void)
      {
      return iColorRec((int)labs(R), (int)labs(G), (int)labs(B), (int)labs(A));
      }

   /*-------------------------------------------------------------------------
      Addition
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator + (const iColorRec &Color)
      {
      return iColorRec(R + Color.R, G + Color.G, B + Color.B, A + Color.A);
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator + (const int k)
      {
      return iColorRec(R + k, G + k, B + k, A + k);
      }

   /*-------------------------------------------------------------------------
      Neagate
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator - (void)
      {
      return iColorRec(-R, -G, -B, -A);
      }

   /*-------------------------------------------------------------------------
      Decrement
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator -= (const iColorRec &Color)
      {
      R -= Color.R; 
      G -= Color.G; 
      B -= Color.B;
      A -= Color.A;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Decrement by a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator -= (const int k)
      {
      R -= k; 
      G -= k; 
      B -= k;
      A -= k;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Subtraction
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator - (const iColorRec &Color)
      {
      return iColorRec(R - Color.R, G - Color.G, B - Color.B, A - Color.A);
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator - (int k)
      {
      return iColorRec(R - k, G - k, B - k, A - k);
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator *= (const iColorRec &Color)
      {
      R *= Color.R; 
      G *= Color.G; 
      B *= Color.B;
      A *= Color.A;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Self multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator *= (const int k)
      {
      R *= k; 
      G *= k; 
      B *= k;
      A *= k;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Multiply
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator * (const iColorRec &Color)
      {
      return iColorRec(R * Color.R, G * Color.G, B * Color.B, A * Color.A);
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator * (const int k)
      {
      return iColorRec(R * k, G * k, B * k, A * k);
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator /= (const iColorRec &Color)
      {
      R /= Color.R; 
      G /= Color.G; 
      B /= Color.B;
      A /= Color.A;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Self divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec &operator /= (const int k)
      {
      R /= k;
      G /= k;
      B /= k;
      A /= k;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Divide
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator / (const iColorRec &Color)
      {
      return iColorRec(R / Color.R, G / Color.G, B / Color.B, A / Color.A);
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline iColorRec operator / (const int k)
      {
      return iColorRec(R / k, G / k, B / k, A / k);
      }

   /*-------------------------------------------------------------------------
      Saturates the colors
     -------------------------------------------------------------------------*/
   __forceinline iColorRec Saturate(void)
      {
      iColorRec Color = *this;

      if (Color.R > 0xFF) {Color.R = 0xFF;} else if (Color.R < 0x00) {Color.R = 0x00;}
      if (Color.G > 0xFF) {Color.G = 0xFF;} else if (Color.G < 0x00) {Color.G = 0x00;}
      if (Color.B > 0xFF) {Color.B = 0xFF;} else if (Color.B < 0x00) {Color.B = 0x00;}
      if (Color.A > 0xFF) {Color.A = 0xFF;} else if (Color.A < 0x00) {Color.A = 0x00;}

      return Color;
      }


   /*==== End of Class =======================================================*/
   };




/*==== End of file ===========================================================*/
#endif