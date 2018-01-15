# Panoramic Rendering for Concave Surfaces of Revolution

Dominik Deák


## 1 Introduction

This project is the source code for my thesis, *Panoramic Rendering for Concave Surfaces of Revolution*, submitted in 2001. The thesis investigated methods for generating panoramic images, based on a 2D profile curve, or a mathematical expression.

A surface of revolution can be constructed by revolving a 2D curve around a line, the principal axis. The geometric shape of the symmetrical surface is governed by the 2D function, the profile curve. Since most curved displays can be considered to be symmetrical about its principal axis, 2D profile curves provide a convenient way for modelling a display shape.

The software implementation includes a ray-tracer and a real-time rendering system.


## 2 Source Code

The code is not exactly a shining example of good C++ programming practices, but it did the job. The project is organised into the following structure:

Path               | Description
---                | ---
object/            | 3D models needed to load the test scenes
source/            | C++ source code for the project
thesis/            | Collection of trial programs and scripts used for the thesis
__scene00.scr      | Test scene
__test0.scr        | The cube scene
__test1.scr        | The rubber duck scene
__test2.scr        | Hardware scene
__test3.scr        | The toy scene
__test4.scr        | Biology scene
__toys.scr         | Toy scene
cosmic_ray_tau.dsp | Visual Studio 6.0 project file
cosmic_ray_tau.dsw | Visual Studio 6.0 project file
cosmic_ray_tau.mak | Visual Studio 6.0 makefile
glut32.dll         | GLUT runtime library
license.md         | MIT license
readme.md          | This file
thesis.pdf         | The full thesis


## 3 Supporting Resources

### 3.1 Documentation

* [Panoramic Rendering](https://deaksoftware.com.au/articles/panoramic_rendering) - Main article
* [Panoramic Rendering for Concave Surfaces of Revolution](./thesis.pdf) - PDF thesis, 3.2 MB

### 3.2 Video Clips

* [Fourth Degree Polynomial Panorama With Cosine Perturbation](https://www.youtube.com/watch?v=MCSk8av_H7Q) - YouTube video
* [Hyperbolic Panorama](https://www.youtube.com/watch?v=RVNAbg3a7mA) - YouTube video
* [Logarithmic Curve Panorama](https://www.youtube.com/watch?v=fmpXRi-tAOo) - YouTube video
* [Spherical Panorama](https://www.youtube.com/watch?v=hjgnWKOimcQ) - YouTube video


## 4 Legal and Copyright

Released under the [MIT License](./license.md).

Copyright 2018, DEAK Software
