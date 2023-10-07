/******************************************************************
 * Building a drawing panel.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#define    TYPE_WINDOW  1
#define    SIZE_WINDOW  2
#define    COLOR_WINDOW  3

typedef    int   menu_t;
menu_t     properties_m;

int winHeight=512, winWidth=512;


/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * clear the window
 */
void display_func(void)
{
  /* define window background color */
  glClearColor (0.2, 0.2, 0.2, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void reshape_func(int new_w, int new_h)
{
  winHeight = new_h;
  winWidth = new_w;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) winWidth, 0.0, (double) winHeight);
  glViewport(0,0,winWidth,winHeight);
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
}

/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void keyboard_func(unsigned char key, int x, int y)
{
  if(key=='Q'||key=='q') exit(0);
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
}

/*-----------------------------------------------------------------
 * Callback function for properties menu
 */
void  properties_func(int value)
{
  switch(value){
  case TYPE_WINDOW:
    //myColor[0] = myColor[1] = myColor[2] = 1.0;
    break;

  case SIZE_WINDOW:
    //myColor[0] = 1.0;
    //myColor[1] = myColor[2] = 0.0;
    break;

  case COLOR_WINDOW:
    //myColor[0] = myColor[2] = 0.0;
    //myColor[1] = 1.0;
    break;

  default:
    break;
  }
}


/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
int main(int argc, char **argv)
{
  /* Initialization */
  glutInit(&argc, argv);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(winWidth, winHeight);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
  
  /* Create window */
  glutCreateWindow("Drawing Panel"); 
  
  /* Associate callback functions */
  glutDisplayFunc(display_func);
  glutReshapeFunc(reshape_func);
  glutKeyboardFunc(keyboard_func);
  
  /* set up menu 
     目前有屬性 可加入exit 以及 String輸入功能 save or read 加入grid-line
  */

  properties_m = glutCreateMenu(properties_func); /* Create color-menu */
  glutAddMenuEntry("Type", TYPE_WINDOW);
  glutAddMenuEntry("Size", SIZE_WINDOW);
  glutAddMenuEntry("Color", COLOR_WINDOW);


  glutCreateMenu(top_menu_func);
  glutAddSubMenu("Properties", properties_m);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  /*---Enter the event loop ----*/
  glutMainLoop();       
}
