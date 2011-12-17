-------------------------------------------------------------------------
 Cosmic Ray Tau - Panoramic Rendering for Concave Surfaces of Revolution
-------------------------------------------------------------------------


Dominik Deak

This project is the source code for my thesis, "Panoramic Rendering for 
Concave Surfaces of Revolution". The thesis investigated methods for
generating panoramic images, based on a 2D profile curve, or a mathematical
expression.

A surface of revolution can be constructed by revolving a 2D curve around a 
line, the principal axis. The geometric shape of the symmetrical surface is
governed by the 2D function, the profile curve. Since most curved displays 
can be considered to be symmetrical about its principal axis, 2D profile 
curves provide a convenient way for modelling a display's shape.

The software implementation includes a ray-tracer and a real-time rendering 
system.


2.0 Source Code

The code is not exactly a shining example of good C++ programming practices, 
but it did the job. The project is organised into the following structure:

   object/              - 3D models needed to load the test scenes
   source/              - C++ source code for the project
   thesis/              - Collection of trial programs and scrips used for the thesis
   __scene00.scr        - Test scene
   __test0.scr          - The cube scene
   __test1.scr          - The rubber duck scene
   __test2.scr          - Hardware scene
   __test3.scr          - The toy scene
   __test4.scr          - Biology scene
   __toys.scr           - Toy scene
   cosmic_ray_tau.dsp   - Visual Studio 6.0 project file
   cosmic_ray_tau.dsw   - Visual Studio 6.0 project file
   cosmic_ray_tau.mak   - Visual Studio 6.0 makefile
   glut32.dll           - GLUT runtime library
   license.txt          - GPL 2.0 license
   readme.txt           - This file


3.0 Additional Information

3.1 Thesis

http://www.csse.monash.edu.au/hons/projects/2001/Deak.pdf


3.2 Project Overview

http://dominikdeak.com.au/index.php?page=panoramic-rendering


3.3 Video Demonstration of the Algorithm

http://www.youtube.com/watch?v=MCSk8av_H7Q
http://www.youtube.com/watch?v=RVNAbg3a7mA
http://www.youtube.com/watch?v=fmpXRi-tAOo
http://www.youtube.com/watch?v=hjgnWKOimcQ


4.0 License

Copyright 2003, Dominik Deak

This program is free software; you can redistribute it and/or modify it under the 
terms of the GNU General Public License as published by the Free Software 
Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License (https://www.gnu.org/licenses/gpl-2.0.html) for more 
details.