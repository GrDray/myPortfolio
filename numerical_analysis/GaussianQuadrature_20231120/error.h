/****************************************************************************
 * This program is to calculate errors.
 * with area (-2,-2) to (6,6) using 2D Gaussian Quadrature.
 * 
 * We will divide the area to 1, 4 or 16 cells.
 * and for each cell, we will use NxN legendre roots. (N=2,3,4
 */


#pragma once
#include <stdio.h>
#include <math.h>


/*output the percentage of relative error*/
double relativeError(double calValue, double exactValue){
  return fabs(100*(calValue-exactValue)/exactValue);
}








