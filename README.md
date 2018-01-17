# LHS-Maximin

This repository provides several programs by some methods in order to
improve Latin Hypercube Sampling by criteria of maximin.  Totally,
this repository includes 4 different points of views to solve the
problem. For example:

* Method of iteration
  * In this method, the program check all the possibilities to find
    out the results, you can check it in the folder: LHS_EXAUSTIF
* Method of testing distance
  * Generally when we have got the maximin distance, we can try to
    detect the result directly. In the folder: LHS_D2TEST
* Method of sum combination
  * When the number of dimension >= number of points, the maximin
    distance is close to the theoretical limit. In the folder: LHS_SUM
* Method of checking bound
  * When the number of points and dimensions are too big to find out
    the result, the repository provides a method to find out the
    bound. In the folder: LHS_BOUND

## Requirements

You need an interpreter of the Matlab/Octave langage, that is, either
Mathworks's [Matlab](http://www.mathworks.com/products/matlab/
"Matlab") or [GNU Octave](https://www.gnu.org/software/octave/ "GNU
Octave").

The results presented in the repository were obtained using Matlab
R2015a on a computer equipped with Inter i7 3.4GHZ and RAM
16g. Besides, you’d better pay attention to your version of visual
studio, which should match well with Matlab version.

## Installation

Simply run init.m to compile all the MEX-files and add subdirectories
to the path.

## Licence

Copyright (C) 2016 Yuan Gao <gaoyuantim@gmail.com> and CentraleSupelec

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA
