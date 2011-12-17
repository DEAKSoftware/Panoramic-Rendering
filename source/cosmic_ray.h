/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __COSMIC_RAY_H__
#define __COSMIC_RAY_H__



//-- Select the target machine --
#if defined (WIN32) || defined (WIN32_NT)

#if !defined (INTEL_x86)
   #define INTEL_x86                       //Intel 80x86 compatible processor
#endif
#if !defined (INTEL_x86_SSE)
//   #define INTEL_x86_SSE                   //Intel 80x86 compatible processor with SSE instructions
#endif

#endif


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
//-- Common stuff --
#include "_common/std_inc.h"
#include "_common/std_3d.h"
#include "_common/std_str.cpp"
#include "_common/list.cpp"
#include "_common/stack.h"

//-- Data and memory management --
#include "mem_data/sysflags.cpp"
#include "mem_data/cfg_data.cpp"
#include "mem_data/bitmap.cpp"
#include "mem_data/vertex.cpp"
#include "mem_data/polygon.cpp"
#include "mem_data/nurb.cpp"
#include "mem_data/light.cpp"
#include "mem_data/entity.cpp"
#include "mem_data/world.cpp"

//-- Maths functions --
#include "math/mathcnst.h"
#include "math/colorrec.h"
#include "math/pointrec.h"
#include "math/texpointrec.h"
#include "math/mathpoly.cpp"
#include "math/primitive.cpp"
#include "math/equsolver.cpp"

//-- Bitmap editing --
#include "bmp_edit/pixel.h"

//-- Video interface --
#include "video_io/video.cpp"
#include "video_io/video_null.cpp"
#include "video_io/video_ddraw.cpp"
#include "video_io/video_opengl.cpp"

//-- Rendering interface --
#include "render/record.cpp"
#include "render/render.cpp"
#include "render/render_null.cpp"
#include "render/render_ddraw.cpp"
#include "render/render_opengl.cpp"
#include "render/render_ray.cpp"
#include "render/transform.cpp"
#include "render/shade.cpp"

//-- Disk interface --
#include "disk_io/tga_fmt.cpp"
#include "disk_io/ppm_fmt.cpp"
#include "disk_io/cob_fmt.cpp"
#include "disk_io/scr_fmt.cpp"
#include "disk_io/asc_fmt.cpp"

//-- System routines --
#include "system/systimer.cpp"

//-- Contol interface --
#include "ctrl_io/keyboard.cpp"
#include "ctrl_io/mouse.cpp"


/*==== End of file ===========================================================*/
#endif
