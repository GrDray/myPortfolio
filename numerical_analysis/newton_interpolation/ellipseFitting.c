/*******************************************************************************
 * This file contains the main procedure of this example program. The main procedure performs
 * the following computation:
 * 1. generating the sample points,
 * 2. computing the x- and y-coordinates of NUM_INTER_PNTS interpolation points,
 * 3. displaying the results.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Define PI
#define  PI 3.14159

#define  N 8


//Sample points' space. A large chunk of memory

double  X[N+1], Y[N+1];
double  theta[N+1];
double  cx[N+1], cy[N+1];  // to store the coef of Newton.

double  px[361], py[361];  // equal to sample data (361)
double  qx[361], qy[361];  // value of interpolation
double  thetaP[361], thetaQ[361];

/************************************************************
 * Procedure to generate N sample points uniformly distributed in a circle.
 * The circel is entered at (0, 0)
 * Input:
 *    N: number of samples,
 *    r: radius of the circel.
 * Output:
 *    *t: array of parametric values (angles in radiens),
 *    *x, *y: arrays of x- & y-coordinates.
 *  Called by main() in main.c.
 */
void  gen_ellipse_pnts(double *t, double  *x, double *y, int n)
{
   int  i;
   double   dTheta, theta;

   //compute the incremental value of angle in radians.
   dTheta = 2.0*PI/((double)(n));

  //Generate the x- and y-coordinates of the sample points.
  for(i=0;i<=n;i++){
    theta = i*dTheta;
    t[i] = theta;
    x[i] = 4*cos(theta);
    y[i] = 2*sin(theta);
  }
  return;
} 


void comp_Newton_coef(double *coef, double *samplePara, double *sample, int n)
{
	int     i, j;

	//initialize the coef's.
	for(i=0;i<=n;i++) coef[i] = sample[i];

	//Recursively compute the coefficients.
	for(i=1;i<=n;i++){
		for(j=n;j>=i;j--)  // Modify c_j = (c_j-c_i)/(xj-xi)
			coef[j] = (coef[j]-coef[j-1])/(samplePara[j]-samplePara[j-i]);
	}
/*    for(i=0;i<=n;i++)
		fprintf(stderr," c%d = %lf\n", i, c[i]);
*/
}

double Newton_interpolate(double posWanted, double *samplePara, double *coef, int n)
{
  double   sum=0.0;
  int      i;

  //Compute polynomial value by using Horner's algm.
  sum = coef[n];
  for(i=n-1;i>=0;i--)
    sum = sum*(posWanted-samplePara[i]) + coef[i];
  return (sum);
}

double distance_2_norm(double *px, double *py, double *qx, double *qy){
  double sum = 0;
  for(int i=0;i<361;i++){
    sum += (px[i]-qx[i])*(px[i]-qx[i]) + (py[i]-qy[i])*(py[i]-qy[i]);
  }
  sum = sqrt(sum);
  return sum;
}

double distance_8_norm(double *px, double *py, double *qx, double *qy){
  double err = 0;
  for(int i=0;i<361;i++){
    double temp = sqrt((px[i]-qx[i])*(px[i]-qx[i]) + (py[i]-qy[i])*(py[i]-qy[i]));
    if(temp>err) err = temp;
  }
  return err;
}


//int argc, char *argv[]
int main(){
  int n=N;
  /*
  if (argc == 2) {
    n = atoi(argv[1]);
  }else{
    n = N;
  }
  */
  /*generate N+1 points to construct interpolation */
  gen_ellipse_pnts(theta, X, Y, n); // for problem II.A
  comp_Newton_coef(cx, theta, X, n);
  comp_Newton_coef(cy, theta, Y, n);

  /*generate p */
  gen_ellipse_pnts(thetaP, px, py, 360);

  /*generate q */
  gen_ellipse_pnts(thetaQ, qx, qy, 360);
  for(int i=0;i<=360;i++){
    qx[i] = Newton_interpolate(thetaQ[i], theta, cx, n);  // for problem II.B
    qy[i] = Newton_interpolate(thetaQ[i], theta, cy, n);  // for problem II.B
  }



  fprintf(stderr, "    px\t\t  qx\t\t    py\t\t  qy \n");
  fprintf(stderr, "---------------------------------------\n");
  for (int i = 0; i <= 360; i++)
  {
    fprintf(stderr, "%lf \t %lf \t %lf \t %lf \n", px[i], qx[i], py[i], qy[i]);
  }
  
  fprintf(stderr, "2norm:         %.10f\n", distance_2_norm(px, py, qx, qy));
  fprintf(stderr, "infinite norm: %.10f\n", distance_8_norm(px, py, qx, qy));
  system("pause");
  return 0;
  
}