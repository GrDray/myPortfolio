/**************************************************************
 *  Draw f(x, y) and g(x, y) for problem 3
 */
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>          
#include <GL/freeglut.h>              /* glut.h */

#define EPSILON   0.01

/*-- Width and Height of the window ---*/
int width = 500, height = 500;

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
int  is_converge(double x, double y)
{
  int  i=0;
  double  err, xnew, ynew, xold, yold;
  double  fdx, fdy, gdx, gdy, Delta;
  double  fold, gold;
  
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
  if(isnormal(xnew) && isnormal(ynew)){
    return 1;
  }else{
    return 0;
  }
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
      // make sure x and y is not nan or inf
      /*---- Setup color to red and bigger size------*/
      if((isnormal(xnew) && isnormal(ynew))){
        if(xnew>0 && ynew>0){
	        glColor3f(1.0, 0.0, 0.0);
        }else if(xnew>0 && ynew<0){
          glColor3f(0.0, 1.0, 0.0);
        }else if(xnew<0 && ynew>0){
          glColor3f(0.0, 0.0, 1.0);
        }else{
          glColor3f(0.7, 0.5, 0.5);
        }
        glVertex2f(x, y);
        glVertex2f(xnew, ynew);
      }
    }
  }
}


/*----------------------------------------------------
 * Draw mesh 
 */
void draw_mesh(float x_left, float x_right, float y_bottom, float y_top){
  /*---- Setup color to dark------*/
	glColor3f(0.1, 0.1, 0.1);

  glEnable(GL_LINE_STIPPLE);  // enable stipple line
  glLineStipple(2, 0X00FF);  // stipple mode
  glBegin(GL_LINES);
    for(float x=x_left; x<=x_right; x++){
      glVertex2f(x, y_bottom);
      glVertex2f(x, y_top);
    }
    for(float y=y_bottom; y<=y_top; y++){
      glVertex2f(x_left, y);
      glVertex2f(x_right, y);
    }
  glEnd();
  glDisable(GL_LINE_STIPPLE); // disable stipple
  glFlush();
}


/*--------------------------------------------------------------------------
 * draw x and y axis
 */
void draw_axis(float x_left, float x_right, float y_bottom, float y_top){
  /*---- Setup color to dark------*/
	glColor3f(0.1, 0.1, 0.1);

  glBegin(GL_LINES);
    glVertex2f(x_left, 0);
    glVertex2f(x_right, 0);
    glVertex2f(0, y_bottom);
    glVertex2f(0, y_top);
  glEnd();
  glFlush();
}

/*----------------------------------------------------
 * Draw f(x, y)
 */
void draw_function_f(float offset)
{
	/*---- Setup color to dark------*/
	glColor3f(0.1, 0.1, 0.1);
  /*----- Draw the function f  -----*/
  float x, y;
  for(x=-3.0; x<=3.0; x+=offset){
    for(y=-3.0; y<=3.0; y+=offset){
      if(fabs(f(x, y)) < offset){
        glVertex2f(x, y);
      }
    }
  }
}

/*----------------------------------------------------
 * Draw g(x, y)
 */
void draw_function_g(float offset)
{
	/*---- Setup color to dark------*/
	glColor3f(0.1, 0.1, 0.1);
  /*----- Draw the function f  -----*/
  float x, y;
  for(x=-3.0; x<=3.0; x+=offset){
    for(y=-3.0; y<=3.0; y+=offset){
      if(fabs(g(x, y)) < offset){
        glVertex2f(x, y);
      }
    }
  }
}


/*------------------------------------------------
 * Callback func of display event
 * When the window shows up this procedure is called.
 */
void display()
{ 
  /* set clear color to white and clear window */
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // set the matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-4.0, 4.0, -4.0, 4.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_axis(-4.0, 4.0, -4.0, 4.0); // draw x y axis
  draw_mesh(-4.0, 4.0, -4.0, 4.0); // draw mesh

  glPointSize(2.0);
  glBegin(GL_POINTS); // draw necessary
    draw_function_f(0.01);
    draw_function_g(0.01);
  glEnd();

  glPointSize(8.0);

  glBegin(GL_POINTS); // draw grid points that converge
    newton2D_find_49_converge_grid();
  glEnd();
  glFlush();
  fprintf(stderr,"display() is called \n");
}




/*--------------------------------------------
 * The main procedure
 */
int main(int argc, char** argv)
{
  /* Initialize mode and open a window in upper left corner of screen */
  glutInit(&argc,argv);             /* Make a connection to window system */
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); /* Set display mode */

  /*----set window attribute ------*/
  glutInitWindowSize(width, height);      /* window size */
  glutInitWindowPosition(0,0);      /* initial position */

  /*----create the window ---*/
  glutCreateWindow("plot_f_and_g"); 



  /*----Associate callback functions with events--*/
  glutDisplayFunc(display);         /* display event */
  
  glutMainLoop();
}
