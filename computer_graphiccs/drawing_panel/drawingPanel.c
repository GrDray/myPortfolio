/******************************************************************
 * Building a drawing panel.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

/*define menu input */
#define    POLYGON_START  1
#define    POLYGON_FINISH  2
#define    DRAW_PEN  1
#define    DRAW_LINE  2
#define    DRAW_CIRCLE  3
#define    DRAW_RECTANGLE  4
#define    MENU_STRING  1

typedef    int   menu_t;
menu_t     polygon_m, draw_m, string_m;

int winHeight=640, winWidth=960; // the size of main window

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

/*------------------------------------------------------------
 * Callback function handling mouse events
 */
void mouse_func(int button, int state, int x, int y)
{
  if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)
    return;
}

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
  
}


/*---------------------------------------------------------------
 * Callback function for top menu.
 */
void top_menu_func(int value)
{
}

/*-----------------------------------------------------------------
 * function for submenu
 */
void  polygon_func(int value)
{

}

void  draw_func(int value)
{
  /*set types to draw */

  switch(value){
  case 1:
    //myColor[0] = myColor[1] = myColor[2] = 1.0;
    break;

  case 2:
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

void  string_func(int value)
{
  /*set properties to input string */

  switch(value){
  case 1:
    //myColor[0] = myColor[1] = myColor[2] = 1.0;
    break;

  case 2:
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
  glutMouseFunc(mouse_func);
  glutMotionFunc(motion_func);
  
  /* set up menu */
  polygon_m = glutCreateMenu(polygon_func); /* Create polygon-menu */
  glutAddMenuEntry("Start", POLYGON_START);
  glutAddMenuEntry("Finish", POLYGON_FINISH);

  draw_m = glutCreateMenu(draw_func); /* Create draw-menu */
  glutAddSubMenu("Polygon", polygon_m);
  glutAddMenuEntry("Pen", DRAW_PEN);
  glutAddMenuEntry("Line", DRAW_LINE);
  glutAddMenuEntry("Circle", DRAW_CIRCLE);
  glutAddMenuEntry("Rectangle", DRAW_RECTANGLE);

  glutCreateMenu(top_menu_func);
  glutAddSubMenu("Draw", draw_m);
  glutAddMenuEntry("String", MENU_STRING);
  glutAddSubMenu("Size", );
  glutAddSubMenu("Color", );
  glutAddSubMenu("Fill w/", );
  glutAddSubMenu("File", );
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  /*---Enter the event loop ----*/
  glutMainLoop();       
}


/* menu目前有屬性 可加入exit  save or read 加入grid-line
 */