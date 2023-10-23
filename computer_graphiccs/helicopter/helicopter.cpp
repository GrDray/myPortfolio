/*---------------------------------------------------------------------------------
 * This file contains a program, drawing a helicopter and its movement.
 *
 * by C. J. Tsao, Nov.  2023
 */
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include "myHeli.h"

/*-----Define window size----*/
int width=1280, height=720;


/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  /*set the background color BLACK */
  glClearColor(0.2, 0.2, 0.2, 1.0);
  /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, 1280, 720);

  /*-----Set a parallel projection mode-----*/
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-15.0, 15.0 , -15.0, 15.0, -10.5, 30.0);
  glOrtho(-30.0, 30.0 , -30.0, 30.0, -20, 50.0);
  glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

  glFlush();/*Enforce window system display the results*/
}




/*-------------------------------------------------------
 * Display and Idle callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();           /* Initialize modelview matrix */
  gluLookAt( 0.0, 5, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  /* enter the main body coord. origin. 
   * and draw the helicopter.
   */
  glTranslatef(0.0, 3.0, 10.0);
  glRotatef(30.0, 0.0, 1.0, 0.0);
  MyHeli::drawHeli();

  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  glFlush(); /*--Display the results----*/
}



/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{
  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w>h)
    glOrtho(-15.0, 15.0, -15.0*(float)h/(float)w, 15.0*(float)h/(float)w, 
            -10.5, 30.0);
  else
    glOrtho(-15.0*(float)w/(float)h, 15.0*(float)w/(float)h, -15.0, 15.0, 
            -10.5, 30.0);
}










/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA| GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("Helicopter");

  /*---Initialize other state varibales----*/
  myinit();     
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);
  //glutIdleFunc(idle_func);    /* Add idle event */
  glutReshapeFunc(my_reshape);
  //glutKeyboardFunc(my_keyboard);

  glutMainLoop();
}



