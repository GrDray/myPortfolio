/*******************************************************************************
 * This file contains the main procedure of Newton's interpolation. The main procedure performs
 * the following computation:
 * 1. generating the sample points,
 * 2. computing the divided differences
 * 3. computing the results.
 * 4. computing the errors.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Define PI
#define  PI 3.14159

//#define  N 8
//#define  N 16
//#define  N 24
#define  N 32
//#define  N 48

//Sample points' space. A large chunk of memory
double  px[N+1], py[N+1], pt[N+1];  // equal to sample data
double  qx[N+1], qy[N+1], qt[N+1];  // value of interpolation
double  c[N+1];  // to store the coef of Newton from divided differences.

/************************************************************
 * Input:
 *    n_sample: number of samples,
 *    t: array to store angles.
 *    x,y: the array to store the sample data.
 */
void  gen_ellipse_pnts(double *t, double  *x, double *y, int n_sample)
{
   int  i;
   double   dTheta, theta;

   //compute the incremental value of angle in radians.
   dTheta = 2.0*PI/((double)(n_sample));

  //Generate the x- and y-coordinates of the sample points.
  for(i=0;i<=n_sample;i++){
    theta = i*dTheta;
    t[i] = theta;
    x[i] = 4*cos(theta);
    y[i] = 2*sin(theta);
  }
  return;
} 

double Newton_interpolate(double t, double *sampleX, int n_sample)
{
  double   sum=0.0;
  int      i;

  //Compute polynomial value by using Horner's algm.
  sum = c[n_sample];
  for(i=n_sample-1;i>=0;i--)
    sum = sum*(t-sampleX[i]) + c[i];
  return (sum);
}

void comp_Newton_coef(double *sampleX, double *sampleY, int n_sample)
{
	int     i, j;
	
	//initialize the coef's.
	for(i=0;i<=n_sample;i++) c[i] = sampleY[i];
	
	//Recursively compute the coefficients.
	for(i=1;i<=n_sample;i++){
		for(j=n_sample;j>=i;j--)  // Modify c_j = (c_j-c_i)/(xj-xi)
			c[j] = (c[j]-c[j-1])/(sampleX[j]-sampleX[j-i]);
	}

}

double distance_2_norm(double *px, double *py, double *qx, double *qy){
  double sum = 0;
  for(int i=0;i<N+1;i++){
    sum += (px[i]-qx[i])*(px[i]-qx[i]) + (py[i]-qy[i])*(py[i]-qy[i]);
  }
  sum = sqrt(sum);
  return sum;
}

double distance_8_norm(double *px, double *py, double *qx, double *qy){
  double err = 0;
  for(int i=0;i<N+1;i++){
    double temp = sqrt((px[i]-qx[i])*(px[i]-qx[i]) + (py[i]-qy[i])*(py[i]-qy[i]));
    if(temp>err) err = temp;
  }
  return err;
}



int main(){
  gen_ellipse_pnts(pt, px, py, N); // generate the sapmle data.

  comp_Newton_coef(px, py, N);
  for(int i=0;i<=N;i++){
    qx[i] = px[i];
    qy[i] = Newton_interpolate(qx[i], px, N);  // interpolation
  }


  /* print all results */
  fprintf(stderr, "    px\t\t  py\t\t  qy \n");
  fprintf(stderr, "---------------------------------------\n");
  for (int i = 0; i <= N; i++)
  {
    fprintf(stderr, "%lf \t %lf \t %lf \n", px[i], py[i], qy[i]);
  }
  
  fprintf(stderr, "2norm:         %lf\n", distance_2_norm(px, py, qx, qy));
  fprintf(stderr, "infinite norm: %lf\n", distance_8_norm(px, py, qx, qy));
  system("pause");
  return 0;
  
}
