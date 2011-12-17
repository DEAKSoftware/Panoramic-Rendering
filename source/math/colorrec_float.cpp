/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                          The Color Data Structure                          */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __COLORREC_FLOAT_CPP__
#define __COLORREC_FLOAT_CPP__


/*---------------------------------------------------------------------------
  The ColorRec class.
  ---------------------------------------------------------------------------*/
class ColorRec
   {
   /*==== Public Declarations ================================================*/
   public:

   float R, G, B, A;

   /*-------------------------------------------------------------------------
      Default constructor and destructor
     -------------------------------------------------------------------------*/
   ColorRec(void) {}
   ~ColorRec(void) {}

   /*-------------------------------------------------------------------------
      Copy constant constructor
   /*-------------------------------------------------------------------------*/
   __forceinline ColorRec (const float k)// : R(k), G(k), B(k)
      {
      R = G = B = A = k;
      }

   /*-------------------------------------------------------------------------
      Copy [R,G,B,A] constructor
     -------------------------------------------------------------------------*/
   __forceinline ColorRec (const float _R, const float _G, const float _B, const float _A)// : R(_R), G(_G), B(_B), A(_A)
      {
      R = _R;
      G = _G;
      B = _B;
      A = _A;
      }

   /*-------------------------------------------------------------------------
      Copy ColorRec constructor
     -------------------------------------------------------------------------*/
   __forceinline ColorRec (const ColorRec &Color)// : R(Color.R), G(Color.G), B(Color.B), A(Color.A)
      {
      R = Color.R;
      G = Color.G;
      B = Color.B;
      A = Color.A;
      }

   /*-------------------------------------------------------------------------
      Increment
     -------------------------------------------------------------------------*/
   __forceinline ColorRec &operator += (const ColorRec &Color)
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
   __forceinline ColorRec &operator += (const float k)
      {               
      R += k;
      G += k;
      B += k;
      A += k;
      return *this;
      }

   /*-------------------------------------------------------------------------
      Absolute value. Example, the absolute value of B:

      ColorRec F = B.Abs();
     -------------------------------------------------------------------------*/
   __forceinline ColorRec Abs(void)
      {
      return ColorRec(fabs(R), fabs(G), fabs(B), fabs(A));
      }

   /*-------------------------------------------------------------------------
      Addition
     -------------------------------------------------------------------------*/
   __forceinline ColorRec operator + (const ColorRec &Color)
      {
      return ColorRec(R + Color.R, G + Color.G, B + Color.B, A + Color.A);
      }

   /*-------------------------------------------------------------------------
      Add constant
     -------------------------------------------------------------------------*/
   __forceinline ColorRec operator + (const float k)
      {
      return ColorRec(R + k, G + k, B + k, A + k);
      }

   /*-------------------------------------------------------------------------
      Neagate
     -------------------------------------------------------------------------*/
   __forceinline ColorRec operator - (void)
      {
      return ColorRec(-R, -G, -B, -A);
      }

   /*-------------------------------------------------------------------------
      Decrement
     -------------------------------------------------------------------------*/
   __forceinline ColorRec &operator -= (const ColorRec &Color)
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
   __forceinline ColorRec &operator -= (const float k)
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
   __forceinline ColorRec operator - (const ColorRec &Color)
      {
      return ColorRec(R - Color.R, G - Color.G, B - Color.B, A - Color.A);
      }
   
   /*-------------------------------------------------------------------------
      Subtraction by a constant
     -------------------------------------------------------------------------*/
   __forceinline ColorRec operator - (float k)
      {
      return ColorRec(R - k, G - k, B - k, A - k);
      }
   
   /*-------------------------------------------------------------------------
      Self multiply
     -------------------------------------------------------------------------*/
   __forceinline ColorRec &operator *= (const ColorRec &Color)
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
   __forceinline ColorRec &operator *= (const float k)
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
   __forceinline ColorRec operator * (const ColorRec &Color)
      {
      return ColorRec(R * Color.R, G * Color.G, B * Color.B, A * Color.A);
      }
   
   /*-------------------------------------------------------------------------
      Multiply with a constant
     -------------------------------------------------------------------------*/
   __forceinline ColorRec operator * (const float k)
      {
      return ColorRec(R * k, G * k, B * k, A * k);
      }
   
   /*-------------------------------------------------------------------------
      Self divide
     -------------------------------------------------------------------------*/
   __forceinline ColorRec &operator /= (const ColorRec &Color)
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
   __forceinline ColorRec &operator /= (const float k)
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
   __forceinline ColorRec operator / (const ColorRec &Color)
      {
      return ColorRec(R / Color.R, G / Color.G, B / Color.B, A / Color.A);
      }
   
   /*-------------------------------------------------------------------------
      Divide by a constant
     -------------------------------------------------------------------------*/
   __forceinline ColorRec operator / (const float k)
      {
      return ColorRec(R / k, G / k, B / k, A / k);
      }

   /*-------------------------------------------------------------------------
      Find the magnitude squared. The Alpha component is ignored! 
      Example, the magnitude squared of C:  
      
      float A = C.MagSqr();
     -------------------------------------------------------------------------*/
   __forceinline float MagSqr(void) const
      {
      return (R*R + G*G + B*B);
      }
   
   /*-------------------------------------------------------------------------
      Find the magnitude. The Alpha component is ignored! 
      Example, the magnitude of C:  
      
      float A = C.Mag();
     -------------------------------------------------------------------------*/
   __forceinline float Mag(void) const
      {
      return (float)sqrt(R*R + G*G + B*B);
      }
   
   /*-------------------------------------------------------------------------
      Saturates the colors.
     -------------------------------------------------------------------------*/
   __forceinline ColorRec Saturate(void)
      {
      ColorRec Color = *this;

      if (Color.R > 1.0f) {Color.R = 1.0f;} else if (Color.R < 0.0f) {Color.R = 0.0f;}
      if (Color.G > 1.0f) {Color.G = 1.0f;} else if (Color.G < 0.0f) {Color.G = 0.0f;}
      if (Color.B > 1.0f) {Color.B = 1.0f;} else if (Color.B < 0.0f) {Color.B = 0.0f;}
      if (Color.A > 1.0f) {Color.A = 1.0f;} else if (Color.A < 0.0f) {Color.A = 0.0f;}

      return Color;
      }

   /*==== End of Class =======================================================*/
   };




/*==== End of file ===========================================================*/
#endif