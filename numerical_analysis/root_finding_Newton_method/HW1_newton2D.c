/***********************************************************
 * This program demonstrate an example of root finding by using
 * Newton's method.
 */
#include <stdio.h>
#include <math.h>

#define EPSILON   0.000001


/*---------------------------------------------------------------------------
 * The function f(x, y) for the root-finding problem.
 *  f(x,y) = x^2/4 + y^2/4 - 1
 */
double f(double x, double y)
{
    return ((x*x + y*y)/4.0 - 1.0);
}

/*---------------------------------------------------------------------------
 * The function g(x, y) for the root-finding problem.
 *  g(x,y)= x^2 - y^2  - 2
 */
double g(double x, double y)
{
    return (x*x - y*y - 2.0);
}

/*-------------------------------------------------
 * Derivatives of f() and g().
 */
double fx(double x, double y)
{
	return x/2.0;
}

double fy(double x, double y)
{
	return y/2.0;
}

double gx(double x, double y)
{
	return 2.0*x;
}

double gy(double x, double y)
{
	return (-2.0*y);
}


/*--------------------------------------------------------------------------
 * Use 2D Newton to check if a point make function converge
 * if converge return 1, if diverge return 0
 */
int  is_converge(double *x, double *y)
{
  int  i=0;
  double  err, xnew, ynew, xold, yold;
  double  fdx, fdy, gdx, gdy, Delta;
  double  fold, gold;
  
  //Set up initial conditions.
  xold = *x;
  yold = *y;
//Function values & derivatives of the functions
  fold = f(xold, yold);
  gold = g(xold, yold);
  fdx = fx(xold, yold);
  fdy = fy(xold, yold);
  gdx = gx(xold, yold);
  gdy = gy(xold, yold);
//Determinant of the Jacobian matrix, no negative value.
  Delta = fdx*gdy - fdy*gdx;
//Update the root, Don't miss the minus operator!
  xnew = xold - (gdy*fold - fdy*gold)/Delta;
  ynew = yold - (-gdx*fold + fdx*gold)/Delta;
//Compute the 2-norm of the error vector.
  err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));
  
  i = 1;
  while(err>EPSILON){
   

   xold = xnew; // Save current value.
   yold = ynew;
   //Update the root.
   fold = f(xold, yold);
   gold = g(xold, yold);
   fdx = fx(xold, yold);
   fdy = fy(xold, yold);
   gdx = gx(xold, yold);
   gdy = gy(xold, yold);

   Delta = fdx*gdy - fdy*gdx;

   xnew = xold - (gdy*fold - fdy*gold)/Delta;
   ynew = yold - (-gdx*fold + fdx*gold)/Delta;
   //Compute the 1-norm of the error vector.
   err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));
    i++;
  }   
  *x = xnew;
  *y = ynew;
  if(isnormal(xnew) && isnormal(ynew)){
    return 1;
  }else{
    return 0;
  }
}

/*--------------------------------------------------------------------------
 * The 2D Newton's method.
 *     *x, *y: the initial values of the root.
 */
void  newton2D(double *x, double *y)
{
  int  i=0;
  double  err, xnew, ynew, xold, yold;
  double  fdx, fdy, gdx, gdy, Delta;
  double  fold, gold;
  FILE   *fp;
  
  //Set up initial conditions.
  xold = *x;
  yold = *y;
//Function values & derivatives of the functions
  fold = f(xold, yold);
  gold = g(xold, yold);
  fdx = fx(xold, yold);
  fdy = fy(xold, yold);
  gdx = gx(xold, yold);
  gdy = gy(xold, yold);
//Determinant of the Jacobian matrix, no negative value.
  Delta = fdx*gdy - fdy*gdx;
//Update the root, Don't miss the minus operator!
  xnew = xold - (gdy*fold - fdy*gold)/Delta;
  ynew = yold - (-gdx*fold + fdx*gold)/Delta;
//Compute the 2-norm of the error vector.
  err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));
  fp = fopen("newton2D_result.txt","w");
  fprintf(fp, "      i          xn                   yn              error\n");
  fprintf(fp,"------------------------------------------------------------\n");
  fprintf(fp,"     %3d \t%lf \t%lf \t---\n", i, xold, yold);
  i = 1;
  while(err>EPSILON){
   fprintf(fp,"     %3d \t%lf \t%lf \t%lf\n", i, xnew, ynew, err);

   xold = xnew; // Save current value.
   yold = ynew;
   //Update the root.
   fold = f(xold, yold);
   gold = g(xold, yold);
   fdx = fx(xold, yold);
   fdy = fy(xold, yold);
   gdx = gx(xold, yold);
   gdy = gy(xold, yold);

   Delta = fdx*gdy - fdy*gdx;

   xnew = xold - (gdy*fold - fdy*gold)/Delta;
   ynew = yold - (-gdx*fold + fdx*gold)/Delta;
   //Compute the 1-norm of the error vector.
   err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));
    i++;
  }   
  fprintf(fp,"------------------------------------------------------------\n"); 
  fprintf(fp,"     %3d \t%lf \t%lf \t%lf\n", i, xnew, ynew, err);
 *x = xnew;
 *y = ynew;
}





/*--------------------------------------------------------------------------
 * The 2D Newton's method. And list all grid-points that converge
 *     x from -3 to 3
 *     y from -3 to 3
 */
void  newton2D_find_49_converge_grid()
{
  int i;
  double  x, y;
  double  err, xnew, ynew, xold, yold;
  double  fdx, fdy, gdx, gdy, Delta;
  double  fold, gold;
  FILE   *fp;

  fp = fopen("newton2D_49_grid.txt","w");
  fprintf(fp, "      iteration          xn           yn \n");
  fprintf(fp,"------------------------------------------------------------\n");

  for(x=-3.0; x<4.0; x++){
    for(y=-3.0; y<4.0; y++){
      i=0;

      //Set up initial conditions.
      xold = x;
      yold = y;

      //Function values & derivatives of the functions
      fold = f(xold, yold);
      gold = g(xold, yold);
      fdx = fx(xold, yold);
      fdy = fy(xold, yold);
      gdx = gx(xold, yold);
      gdy = gy(xold, yold);

      //Determinant of the Jacobian matrix, no negative value.
      Delta = fdx*gdy - fdy*gdx;
      //Update the root, Don't miss the minus operator!
      xnew = xold - (gdy*fold - fdy*gold)/Delta;
      ynew = yold - (-gdx*fold + fdx*gold)/Delta;

      //Compute the 2-norm of the error vector.
      err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));

      i = 1;
      while(err>EPSILON){

        xold = xnew; // Save current value.
        yold = ynew;
        //Update the root.
        fold = f(xold, yold);
        gold = g(xold, yold);
        fdx = fx(xold, yold);
        fdy = fy(xold, yold);
        gdx = gx(xold, yold);
        gdy = gy(xold, yold);

        Delta = fdx*gdy - fdy*gdx;

        xnew = xold - (gdy*fold - fdy*gold)/Delta;
        ynew = yold - (-gdx*fold + fdx*gold)/Delta;
        //Compute the 1-norm of the error vector.
        err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));
        i++;
      }
      // make sure x and y is not nan or inf
      if(isnormal(xnew) && isnormal(ynew)){
        fprintf(fp,"     %3d \t%lf \t%lf \n", i, x, y);
      }
    }
  }
  fclose(fp);
}



/*--------------------------------------------------------------------------
 * The 2D Newton's method. And list all grid-points that converge
 *     x from -3 to 3
 *     y from -3 to 3
 */
void  newton2D_find_49_diverge_grid()
{
  int i;
  double  x, y;
  double  err, xnew, ynew, xold, yold;
  double  fdx, fdy, gdx, gdy, Delta;
  double  fold, gold;
  FILE   *fp;

  fp = fopen("newton2D_49_grid_diverge.txt","w");
  fprintf(fp, "      iteration          xn           yn \n");
  fprintf(fp,"------------------------------------------------------------\n");

  for(x=-3.0; x<4.0; x++){
    for(y=-3.0; y<4.0; y++){
      i=0;

      //Set up initial conditions.
      xold = x;
      yold = y;

      //Function values & derivatives of the functions
      fold = f(xold, yold);
      gold = g(xold, yold);
      fdx = fx(xold, yold);
      fdy = fy(xold, yold);
      gdx = gx(xold, yold);
      gdy = gy(xold, yold);

      //Determinant of the Jacobian matrix, no negative value.
      Delta = fdx*gdy - fdy*gdx;
      //Update the root, Don't miss the minus operator!
      xnew = xold - (gdy*fold - fdy*gold)/Delta;
      ynew = yold - (-gdx*fold + fdx*gold)/Delta;

      //Compute the 2-norm of the error vector.
      err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));

      i = 1;
      while(err>EPSILON){

        xold = xnew; // Save current value.
        yold = ynew;
        //Update the root.
        fold = f(xold, yold);
        gold = g(xold, yold);
        fdx = fx(xold, yold);
        fdy = fy(xold, yold);
        gdx = gx(xold, yold);
        gdy = gy(xold, yold);

        Delta = fdx*gdy - fdy*gdx;

        xnew = xold - (gdy*fold - fdy*gold)/Delta;
        ynew = yold - (-gdx*fold + fdx*gold)/Delta;
        //Compute the 2-norm of the error vector.
        err = sqrt(pow(fabs(xnew - xold), 2) + pow(fabs(ynew-yold), 2));
        i++;
      }
      // make sure x and y is  nan or inf
      if(!(isnormal(xnew) && isnormal(ynew))){
        fprintf(fp,"     %3d \t%lf \t%lf \n", i, x, y);
      }
    }
  }
  fclose(fp);
}





/*----------------------------------------------------------------------------
  * The main procedure.
  */
int main(int argc, char **argv)
{
  // for 1.(a)
  /*double    x, y;
  fprintf(stderr," Input the 2 initial values x0, y0= ");
  fscanf(stdin,"%lf %lf", &x, &y);
  newton2D(&x, &y); 
  fgetchar(); */

  // for 1.(b), 1.(c)
  // newton2D_find_49_converge_grid();

  // for 1.(d)
  newton2D_find_49_diverge_grid();

  return 0;
}
