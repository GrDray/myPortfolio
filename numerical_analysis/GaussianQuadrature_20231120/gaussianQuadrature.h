/****************************************************************************
 * This program is to integrate f(x,y)=6(sin2x)^2 + 9(cos3y)^2
 * with area (-2,-2) to (6,6) using 2D Gaussian Quadrature.
 * 
 * We will divide the area to 1, 4 or 16 cells.
 * and for each cell, we will use NxN legendre roots. (N=2,3,4
 */


#pragma once
#include <stdio.h>
#include <math.h>



#define MAX_SAMPLE_NUM 4

double sample[MAX_SAMPLE_NUM][MAX_SAMPLE_NUM]={
    {0.0, 0.0, 0.0, 0.0}, 
    {-0.5773502691896257, 0.5773502691896257, 0.0, 0.0}, 
    {-0.7745966692414834, 0.0, 0.7745966692414834, 0.0}, 
    {-0.8611363115940526, -0.3399810435848563, 0.3399810435848563, 0.8611363115940526}};

double weight[MAX_SAMPLE_NUM][MAX_SAMPLE_NUM]={
    {2.0, 0.0, 0.0, 0.0}, 
    {1.0, 1.0, 0.0, 0.0}, 
    {0.555555555555555556, 0.888888888888888888888, 0.555555555555555556, 0.0},
    {0.3478548451374538, 0.6521451548625461, 0.6521451548625461, 0.3478548451374538}};

/* The target function f(x,y)=6(sin2x)^2 + 9(cos3y)^2 */
double fxy(double x, double y){
  return 6*sin(2*x)*sin(2*x) + 9*cos(3*y)*cos(3*y);
}

/* The determinent of Jacobian matrix */
double jacobian(double w, double h){
  return w*h/4;
}

/* The integration of one cell
 * x0,y0 for left-bottom coord.
 * w,h for width and height
 * sampleNum for number of legendre root you want to use. (N=2,3,4
 */
double cell_integrate(double x0, double y0, double w, double h, int smapleNum){
  int i, j;
  double sum = 0.0;
  double Jacob=jacobian(w,h);
  double xi=0.0, yj=0.0;

  for(i=0; i<smapleNum;i++){
    // mapping the -1~1 legendre roots to x0~x0+w
    xi = 0.5*w*(1+sample[smapleNum-1][i]) + x0;
    for(j=0; j<smapleNum;j++){
      // mapping the -1~1 legendre roots to y0~y0+h
      yj = 0.5*h*(1+sample[smapleNum-1][j]) + y0;
      sum += weight[smapleNum-1][i]*weight[smapleNum-1][j]*fxy(xi,yj)*Jacob;
    }
  }
  return sum;
}

/* The integration of all cell.
 * x0,y0 for left-bottom coord.
 * w,h for width and height.
 * divNum divide the whole area to divNum*divNum.
 * sampleNum for number of legendre root you want to use. (N=2,3,4
 */
double gaussianQuadrature2D(double x0, double y0, double w, double h, int divNum, int smapleNum){
  int i, j;
  double x, y;
  double sum = 0.0;
  
  for(i=0; i<divNum;i++){
    // update the new cell position
    x = x0 + i*divNum;
    for(j=0; j<divNum;j++){
      // update the new cell position
      y = y0 + j*divNum;
      sum += cell_integrate(x, y, w/divNum, h/divNum, smapleNum);
    }
  }
  return sum;
}


