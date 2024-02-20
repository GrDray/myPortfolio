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
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);// to normalize the normal vectors
  glEnable(GL_LIGHTING);// to shade
  glShadeModel(GL_SMOOTH);// Gouraud shading
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  glEnable(GL_LIGHT2);
  /*-----Enable face culling -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
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
  myinit();     
  //HeliGUI::setCamera(0.0, 10.0, 30.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);
  HeliGUI::Setup();
  HeliGUI::setCamera1(-10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  HeliGUI::setCamera2(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  HeliGUI::setCamera3(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  HeliGUI::setCamera4(0.0, 10.0, 30.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);

  glutMainLoop();

  HeliGUI::Shutdown();
}



