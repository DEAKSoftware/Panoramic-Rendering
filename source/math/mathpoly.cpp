/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                  Plane/Polygon Related Mathematics                         */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __MATHPOLY_CPP__
#define __MATHPOLY_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../math/mathcnst.h"
#include "../mem_data/polygon.cpp"


/*----------------------------------------------------------------------------
   Test if an INFINITE line intersects an infinite plane. If the line intersects
   the plane, the function returns true, the intersection point is returned in
   I and the constant t.

   
      O        : Origin of the line.
      P        : Pointer to a point that lies on the plane.
      N        : Normal of the plane (unit vector).

      I        : Intersection point.
      I->t     : Intersection constant.
      TestOnly : If set true, only a test is performed, no intersection 
                 calculations are done, but the intersection constant is 
                 returned.
  ----------------------------------------------------------------------------*/
bool inline Plane_InfLine_Intersect(PointRec* I, PointRec* O, PointRec* D, PointRec* P, PointRec* N, bool TestOnly)
   {
   float c = N->Dot(*D);                     //Get the dot product between the N and the L2-L1 vector
   if (c == 0.0f)                            //If C is 0, the line runs parallel with the plane
      {I->t = float_MIN; return false;}

   float t = -(N->Dot(*O) - N->Dot(*P)) / c; //Find the intersection constant
   if (t <= 0.00001f)                        //Intersection is behind the origin?
      {I->t = t; return false;} 
   
   if (TestOnly) {I->t = t; return true;}    //No intersection calculations for tests

   *I = *O + *D*t;
   I->t = t; 

   return true;
   }

/*----------------------------------------------------------------------------
   Test if a BOUNDED line intersects an infinite plane. If the line intersects
   the plane, the function returns true, the intersection point is returned in
   I and the constant t.

      L1, L2   : Pointer to the two points that define the line.
      P        : Pointer to a point that lies on the plane.
      N        : Normal of the plane (unit vector).

      I        : Intersection point (if TestOnly is false).
      I->t     : Intersection constant.
      TestOnly : If set true, only a test is performed, no intersection 
                 calculations are done, but the intersection constant is 
                 returned.
  ----------------------------------------------------------------------------*/
bool inline Plane_Line_Intersect(PointRec* I, PointRec* L1, PointRec* L2, PointRec* P, PointRec* N, bool TestOnly)
   {
   PointRec dL = *L2 - *L1;                  //Find the difference for the line
   float c = N->Dot(dL);                     //Get the dot product between the N and the L2-L1 vector
   if (c == 0.0f) {return false;}            //If C is 0, L1-L2 runs parrallel to the plane

   float a = N->Dot(*P - *L1);               //Get the dot product between the N and the P-L1 vector
   //If A is 0, P-L1 runs parrallel to the plane, L1 lies on the plane

   float b = N->Dot(*P - *L2);               //Get the dot product between the N and the P-L2 vector
   //If B is 0, P-L2 runs parrallel to the plane, L2 lies on the plane

   //If ((sign)a == (sign)b) then bouded line does not cross the plane
   if ((a < 0.0f) == (b < 0.0f)) {return false;}

   float t = a / c;                          //(a / c) is the intersection constant
   if (TestOnly) {I->t = t; return true;}    //No intersection calculations for tests
   
   *I = *L1 + dL*t;          
   I->t = t; 

   return true;
   }

/*----------------------------------------------------------------------------
   Test if an infinite line with an origin intersects a trianglular Polyon. If 
   intersection occurs, the function returns true, the intersection point is 
   returned in I and the constant t. If the line intersects the Polygon behind
   the origin (O), the test fails. The barycetric coordinates of the polygon
   is also saved to Polygon->BaryCent. The BaryCent(X,Y,Z) values correspond 
   to Vertex[0], Vertex[1], Vertex[2] respectively.

      I        : Intersection point.
      I->t     : Intersection constant.

      O        : Origin of the line.
      D        : Direction (unit) vector of the line.
      Polygon  : Polygon to test.
      TestOnly : If set true, only a test is performed, no intersection 
                 calculations are done, but the intersection constant is 
                 returned.
  ----------------------------------------------------------------------------*/
bool inline Poly_InfLine_Intersect(PointRec* I, PointRec* O, PointRec* D, PolygonRec* Polygon, bool TestOnly)
   {
   /*
   //Calculate the cross product between the ray direction and Edge[1]
   PointRec P = D->Cross(Polygon->Edge[1]);
   
   //Find the determinant. If it's near zero, the ray lies in plane of triangle.
   float Det = Polygon->Edge[0].Dot(P);
   if (fabs(Det) <= 0.00001f) {return false;}
   Det = 1.0f / Det;

   //Find the distance between the ray origin and Vertex[0]
   PointRec dOV = *O - Polygon->Vertex[0]->Coord;
   
   //Find the barycentric coordinate for vertex 1 and test it
   Polygon->BaryCent[1] = dOV.Dot(P) * Det;
   if ((Polygon->BaryCent[1] < 0.0f) || (Polygon->BaryCent[1] > 1.0f)) {return false;}

   //Calculate the cross product dOV and Edge[0]
   PointRec Q = dOV.Cross(Polygon->Edge[0]);
   
   //Find the barycentric coordinates for vertex 2 and 0 and test it
   Polygon->BaryCent[2] = D->Dot(Q) * Det;
   Polygon->BaryCent[0] = 1.0f - Polygon->BaryCent[1] - Polygon->BaryCent[2];
   if ((Polygon->BaryCent[2] < 0.0f) || (Polygon->BaryCent[0] < 0.0f)) {return false;}

   //Find the intersection constant, and reject if it lies behind the origin
   float t = Polygon->Edge[1].Dot(Q) * Det;
   if (t <= 0.00001f) {return false;}

   //No intersection calculation for shadow tests
   if (TestOnly) {I->t = t; return true;}
   
   //Find the intersection point
   *I = *O + *D*t;
   I->t = t;

   return true;
   /**/

  
   //Calculate the cross product between the ray direction and Edge[1]
   PointRec P = D->Cross(Polygon->Edge[1]);
   
   //Find the determinant. If it's near zero, the ray lies in plane of triangle.
   float Det = Polygon->Edge[0].Dot(P);
   if (fabs(Det) <= 0.00001f) {return false;}
   Det = 1.0f / Det;

   //Find the distance between the ray origin and Vertex[0]
   PointRec dOV = *O - Polygon->Vertex[0]->Coord;
   
   //Find the barycentric coordinate for vertex 1 and test it
   Polygon->BaryCent[1] = dOV.Dot(P) * Det;
   if ((Polygon->BaryCent[1] < 0.0f) || (Polygon->BaryCent[1] > 1.0f)) {return false;}

   //Calculate the cross product dOV and Edge[0]
   PointRec Q = dOV.Cross(Polygon->Edge[0]);
   
   //Find the barycentric coordinates for vertex 2 and 0 and test it
   Polygon->BaryCent[2] = D->Dot(Q) * Det;
   Polygon->BaryCent[0] = 1.0f - Polygon->BaryCent[1] - Polygon->BaryCent[2];
   if ((Polygon->BaryCent[2] < 0.0f) || (Polygon->BaryCent[0] < 0.0f)) {return false;}

   //Find the intersection constant, and reject if it lies behind the origin
   float t = Polygon->Edge[1].Dot(Q) * Det;
   if (t <= 0.00001f) {return false;}

   //No intersection calculation for shadow tests
   if (TestOnly) {I->t = t; return true;}
   
   //Find the intersection point
   *I = *O + *D*t;
   I->t = t;

   return true;
   }

/*----------------------------------------------------------------------------
   Test if a BOUNDED line intersects a trianglular Polyon. If intersection 
   occurs, the function returns true, the intersection point is 
   returned in I and the constant t.  The barycetric coordinates of the polygon
   is also saved to Polygon->BaryCent. The BaryCent(X,Y,Z) values correspond 
   to Vertex[0], Vertex[1], Vertex[2] respectively.

      I        : Intersection point.
      I->t     : Intersection constant.

      L1, L2   : Pointer to the two points that define the line.
      Polygon  : Polygon to test.
      TestOnly : If set true, only a test is performed, no intersection 
                 calculations are done, but the intersection constant is 
                 returned.
  ----------------------------------------------------------------------------*/
bool inline Poly_Line_Intersect(PointRec* I, PointRec* L1, PointRec* L2, PolygonRec* Polygon, bool TestOnly)
   {
   //Find the unit vector of the line
   PointRec dL = *L2 - *L1;
   PointRec D  = dL.Unit();

   //Calculate the cross product between the ray direction and Edge[1]
   PointRec P = D.Cross(Polygon->Edge[1]);
   
   //Find the determinant. If it's near zero, the ray lies in plane of triangle.
   float Det = Polygon->Edge[0].Dot(P);
   if (fabs(Det) <= 0.00001f) {return false;}
   Det = 1.0f / Det;

   //Find the distance between the ray origin and Vertex[0] 
   PointRec dOV = *L1 - Polygon->Vertex[0]->Coord;
   
   //Find the barycentric coordinate for vertex 1 and test it
   Polygon->BaryCent[1] = dOV.Dot(P) * Det;
   if ((Polygon->BaryCent[1] < 0.0f) || (Polygon->BaryCent[1] > 1.0f)) {return false;}

   //Calculate the cross product dOV and Edge[0]
   PointRec Q = dOV.Cross(Polygon->Edge[0]);
   
   //Find the barycentric coordinate for vertex 2 and 0 and test it
   Polygon->BaryCent[2] = D.Dot(Q) * Det;
   Polygon->BaryCent[0] = 1.0f - Polygon->BaryCent[1] - Polygon->BaryCent[2];
   if ((Polygon->BaryCent[2] < 0.0f) || (Polygon->BaryCent[0] < 0.0f)) {return false;}

   //Find the intersection constant, and reject if it lies outside the bounded line
   float t = Polygon->Edge[1].Dot(Q) * Det;
   if ((t <= 0.00001f) || (t > dL.Mag())) {return false;}

   //No intersection calculation for shadow tests
   if (TestOnly) {I->t = t; return true;}
   
   //Find the intersection point
   *I = *L1 + D*t;
   I->t = t;

   return true;
   }

/*----------------------------------------------------------------------------
   Calculates the reflection of an incident ray on a Polygon.

   R = ||I - N*(N dot I)*2||

   R       : Relfection ray (unit vector).
   I       : Incident ray (unit vector).
   Polygon : Polygon surface.
  ----------------------------------------------------------------------------*/
void inline Poly_Reflect(PointRec* R, PointRec* I, PolygonRec* Polygon)
   {
   PointRec N = Polygon->GetNormal();

   *R = *I - N*(N.Dot(*I))*2.0f;
   *R = R->Unit();
   }

/*----------------------------------------------------------------------------
   Calculates the refraction of an incident ray through a transparent Polygon.


   T       : Transmitted and refrated ray (unit vector).
   I       : Incident ray (unit vector).
   Polygon : Polygon surface.
   Inside  : If the flag is set, the ray goes into the Polygon.
  ----------------------------------------------------------------------------*/
void inline Poly_Refract(PointRec* T, PointRec* I, PolygonRec* Polygon, bool Inside)
   {
   if (Polygon->IdxRefr == 1.0f) {*T = *I; return;} //Don't compute for air

   PointRec N  = Polygon->GetNormal();
   float    n  = Polygon->IdxRefr;     //Index of refraction ratio: n = n1/n2, assume IdxRefr/Air
   float    in = n;                    //Assume inverse of n = n (assume the ray is inside the object)

   //The ratio for n is different, depending if we're inside or outside
   if (Inside) {n = 1.0f / n;}         //If inside, n1 = Air, n2 = IdxRefr, n = Air/IdxRefr, in = n/1
   else {in = 1.0f / in;}              //Else, n1 = IdxRefr, n2 = Air, n = IdxRefr/Air, in = 1/n

   float Angi = I->COS(N);             //Angle between incident ray and normal
   float Angt = sqrt(1.0f - sqr(in)*(1.0f - sqr(Angi))); //Angle between transitted ray and normal
            
   //Transmitted ray equation: T = 1/n*I - (Angi - 1/n*Angt)*N
   Angi = Angt - in*Angi;              //Calculate Angi - 1/n * Angt
   *T   = *I*in - N*Angi;
   }


/*==== End of file ===========================================================*/
#endif