/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                 Mathematics Related Constants and Macros                   */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __MATHCNST_H__
#define __MATHCNST_H__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*----------------------------------------------------------------------------
  Extreme values for various data types.   
  ----------------------------------------------------------------------------*/
#define float_MIN       (float)-3.4e38
#define float_MAX       (float) 3.4e38
#define double_MIN      (double)-1.7e380
#define double_MAX      (double) 1.7e380

#define char_MIN        -128
#define char_MAX         127
#define short_int_MIN   -32768
#define short_int_MAX    32767
#define int_MIN         -2147483648
#define int_MAX          2147483647
#define long_int_MIN    -9223372036854775808
#define long_int_MAX     9223372036854775807

#define byte_MIN        0x00
#define byte_MAX        0xFF
#define word_MIN        0x0000
#define word_MAX        0xFFFF
#define dword_MIN       0x00000000
#define dword_MAX       0xFFFFFFFF
#define qword_MIN       0x0000000000000000
#define qword_MAX       0xFFFFFFFFFFFFFFFF

/*----------------------------------------------------------------------------
  General purpose constants
  ----------------------------------------------------------------------------*/
float  const _NaN          = (float)sqrt(-1.0f);
float  const _3inv         = 1.0f / 3.0f;
float  const _15inv        = 1.0f / 15.0f;
float  const _16inv        = 1.0f / 16.0f;
float  const _63inv        = 1.0f / 63.0f;
float  const _64inv        = 1.0f / 64.0f;
float  const _127inv       = 1.0f / 127.0f;
float  const _128inv       = 1.0f / 128.0f;
float  const _180inv       = 1.0f / 180.0f;
float  const _255inv       = 1.0f / 255.0f;
float  const _256inv       = 1.0f / 256.0f;
float  const _65535inv     = 1.0f / 65535.0f;
float  const _65536inv     = 1.0f / 65536.0f;

double const _pi           = 3.0*acos(0.5);
float  const _pi_inv       = 1.0f / _pi;
float  const _ln2          = log(2.0f);
float  const _ln2inv       = 1.0f / _ln2;
float  const _180_div_pi   = 180.0 * _pi_inv;
float  const _pi_div_180   = _pi   * _180inv;

/*----------------------------------------------------------------------------
  Conversion macros
  ----------------------------------------------------------------------------*/
#define rad2deg(Rad) (_180_div_pi * (Rad))
#define deg2rad(Deg) (_pi_div_180 * (Deg))

#define int_to_fix1616(A)   ((A) << 16)              //Convert integer to 16.16 fixed point format
#define float_to_fix1616(A) ((int)((A) * 65536.0f))  //Convert float to 16.16 fixed point format
#define fix1616_to_int(A)   ((A) >> 16)              //16.16 fixed point format to integer
#define fix1616_to_float(A) ((float)(A) * _65536inv) //16.16 fixed point format to float

#define int_to_fix97(A)     ((A) << 7)               //Convert integer to 9.7 fixed point format
#define float_to_fix97(A)   ((int)((A) * 128.0f))    //Convert float to 9.7 fixed point format
#define fix97_to_int(A)     ((A) >> 7)               //9.7 fixed point format to integer
#define fix97_to_float(A)   ((float)(A) * _128inv)   //9.7 fixed point format to float

#define int_to_fix88(A)     ((A) << 8)               //Convert integer to 8.8 fixed point format
#define float_to_fix88(A)   ((int)((A) * 256.0f))    //Convert float to 8.8 fixed point format
#define fix88_to_int(A)     ((A) >> 8)               //8.8 fixed point format to integer
#define fix88_to_float(A)   ((float)(A) * _256inv)   //8.8 fixed point format to float

#define int_to_fix44(A)     ((A) << 4)               //Convert integer to 4.4 fixed point format
#define float_to_fix44(A)   ((int)((A) * 16.0f))     //Convert float to 4.4 fixed point format
#define fix44_to_int(A)     ((A) >> 4)               //4.4 fixed point format to integer
#define fix44_to_float(A)   ((float)(A) * _16inv)    //4.4 fixed point format to float

#define fix88_to_fix1616(A) ((A) << 8)               //8.8 fixed point format to 16.16 fixed point format
#define fix1616_to_fix88(A) ((A) >> 8)               //16.16 fixed point format to 8.8 fixed point format

/*----------------------------------------------------------------------------
   Arithmetic stuff
  ----------------------------------------------------------------------------*/
#define sqr(A) ((A) * (A))
#define ln log


/*==== End of file ===========================================================*/
#endif
