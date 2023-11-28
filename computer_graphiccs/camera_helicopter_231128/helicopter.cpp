/*---------------------------------------------------------------------------------
 * This file contains a program, drawing a helicopter and its movement.
 *
 * by C. J. Tsao, Nov.  2023
 */
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#include "heli.h"
#include "heliGUI.h"
#include "camera.h"



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
  glOrtho(-15.0, 15.0 , -15.0, 15.0, -10.5, 30.0);
  //glOrtho(-30.0, 30.0 , -30.0, 30.0, -20, 50.0);
  glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

  glFlush();/*Enforce window system display the results*/
}







/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);
  #ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  #endif
  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA| GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("Helicopter");

  /*---Initialize other state varibales----*/
  //myinit();     
  //HeliGUI::setCamera(0.0, 10.0, 30.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);
  HeliGUI::Setup();
  HeliGUI::setCamera1(-10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  HeliGUI::setCamera2(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  HeliGUI::setCamera3(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  HeliGUI::setCamera4(0.0, 10.0, 30.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);

  glutMainLoop();

  HeliGUI::Shutdown();
}



