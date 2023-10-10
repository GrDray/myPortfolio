/******************************************************************
 * Building a drawing panel.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

/*define menu input */
#define    DO_NOTHING  0
#define    POLYGON_START  1
#define    POLYGON_FINISH  2
#define    DRAW_PEN  3
#define    DRAW_LINE  4
#define    DRAW_CIRCLE  5
#define    DRAW_RECTANGLE  6
#define    INPUT_STRING  7
#define    SIZE_X2  2
#define    SIZE_X4  4
#define    SIZE_X8  8
#define    COLOR_NO_FILL 0
#define    COLOR_FILL  1
#define    COLOR_RED  2
#define    COLOR_GREEN  3
#define    COLOR_BLUE  4
#define    MAX_STRING_LEN  50
#define    SHOW  0
#define    NO_SHOW  1

/* define variables for menu */
typedef    int   menu_t;
menu_t     polygon_m, draw_m, string_m, size_m, color_m, fill_m;

/* current state */
int curMode = DO_NOTHING;
int pos_x, pos_y, first=0;
float curSize=1.0;
float curColor[3] = {1.0, 1.0, 1.0};
int curFill = 0;

/*store size of main window*/
int winHeight=640, winWidth=960;

/* define object */
typedef struct position
{
  float x, y;
}position;

typedef struct object
{
  int this_type, this_size, if_fill, if_show;
  float this_color[3];
  char string[MAX_STRING_LEN];
  position *pos;
}object;
object *first_objects=NULL;
object *cur_objects=NULL;

/* create new object and return the new location */
void newObject(int type, float size, int fill, int show, float *color) {
  object *newObject = (object *)malloc(sizeof(object));
  newObject->this_type = type;
  newObject->this_size = size;
  newObject->if_fill = fill;
  newObject->if_show = show;
  if(!first_objects) first_objects = newObject;
  cur_objects = newObject;
}








/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle()
{
  static GLUquadricObj *mycircle=NULL;
  glColor3fv(curColor);
  if(mycircle==NULL){
    mycircle = gluNewQuadric();
    gluQuadricDrawStyle(mycircle,(curFill == 0)?GL_LINE:GL_FILL);
  }
  glPushMatrix();
  glTranslatef(pos_x, pos_y, 0.0);
  gluDisk(mycircle,
    0.0,           /* inner radius=0.0 */
	  10.0,          /* outer radius=10.0 */
	  16,            /* 16-side polygon */
	  3);
  glPopMatrix();
}

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
  glColor3fv(curColor);
  if(curMode == INPUT_STRING){
    glRasterPos2i(pos_x, pos_y);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int) key);
    pos_x += 10;
    glFinish();
  }
  if(key=='Q'||key=='q') exit(0);
}

/*------------------------------------------------------------
 * Callback function handling mouse events
 */
void mouse_func(int button, int state, int x, int y)
{
  if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)
    return;
  glColor3fv(curColor);
  switch (curMode)
  {
  case POLYGON_START:
    nvertex = polygons[npolygon].nvertices += 1;
    polygons[npolygon].x[nvertex-1] = x;
    polygons[npolygon].y[nvertex-1] = winHeight - y;
    break;
  
  case INPUT_STRING:
    pos_x = x;  
    pos_y = winHeight - y;
    break;

  case DRAW_LINE:
    if(first==0){
      first = 1;
      pos_x = x;
      pos_y = winHeight - y;
	    glPointSize(curSize);
      glBegin(GL_POINTS);   /*  Draw the 1st point */
	      glVertex2f(x, winHeight-y);
      glEnd();
    }else{
      first=0;
      glLineWidth(curSize);     /* Define line width */
      glBegin(GL_LINES);    /* Draw the line */
        glVertex2f(pos_x, pos_y);
	      glVertex2f(x, winHeight - y);
      glEnd();
    }
    break;

  case DRAW_CIRCLE:
    pos_x = x; 
    pos_y = winHeight - y;
    draw_circle();
    break;
  case DRAW_RECTANGLE:
    if(first==0){
      first = 1;
      pos_x = x;
      pos_y = winHeight - y;
	    glPointSize(curSize);
      glBegin(GL_POINTS);   /*  Draw the 1st point */
	      glVertex2f(x, winHeight-y);
      glEnd();
    }else{
      first=0;
      glLineWidth(curSize);     /* Define line width */
      if(curFill == 0){
        glBegin(GL_LINE_LOOP);
      }else{
        glBegin(GL_QUADS);    /* Draw the line */
      }
        glVertex2f(pos_x, pos_y);
        glVertex2f(pos_x, winHeight - y);
	      glVertex2f(x, winHeight - y);
        glVertex2f(x, pos_y);
      glEnd();
    }
    break;
  
  default:
    break;
  }
  glFinish();
}

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
  glColor3fv(curColor);
  if(curMode!=DRAW_PEN) return;
  if(first==0){
    first = 1;
    pos_x = x; pos_y = y;
  }else{
    glLineWidth(curSize);
    glBegin(GL_LINES);
      glVertex3f(pos_x, winHeight-pos_y, 0.0);
      glVertex3f(x, winHeight - y, 0.0);
    glEnd();
    pos_x = x; pos_y = y;
  }
  glFinish();
}


/*---------------------------------------------------------------
 * Callback function for top menu.
 */
void top_menu_func(int value)
{
  if(value == INPUT_STRING){
    curMode = INPUT_STRING;
  }
}

/*-----------------------------------------------------------------
 * function for submenu
 */
void  polygon_func(int value)
{
  switch(value){
  case POLYGON_START:
    polygons[npolygon].nvertices = 0;
    curMode = POLYGON_START;
    break;

  case POLYGON_FINISH:
    npolygon++;
    glPolygonMode(GL_FRONT_AND_BACK,
      (curFill == 0)?GL_LINE:GL_FILL);
    glBegin(GL_POLYGON);
      for(int i = 0; i<polygons[npolygon-1].nvertices; i++){
        glVertex2i(polygons[npolygon-1].x[i], polygons[npolygon-1].y[i]);
      }
    glEnd();
    glFlush();
    curMode = DO_NOTHING;
    break;

  default:
    break;
  }
}

void  draw_func(int value)
{
  /*set types to draw */
  first = 0;
  switch(value){
  case DRAW_PEN:
    curMode = DRAW_PEN;
    break;
  case DRAW_LINE:
    curMode = DRAW_LINE;
    break;

  case DRAW_CIRCLE:
    curMode = DRAW_CIRCLE;
    break;
  case DRAW_RECTANGLE:
    curMode = DRAW_RECTANGLE;
    break;

  default:
    break;
  }
}

void  size_func(int value)
{
  switch (value)
  {
  case SIZE_X2:
    curSize = 2.0;
    break;
  case SIZE_X4:
    curSize = 4.0;
    break;
  case SIZE_X8:
    curSize = 8.0;
    break;
  
  default:
    break;
  }
}

void  color_func(int value)
{
  switch (value)
  {
  case COLOR_RED:
    curColor[0] = 1.0;
    curColor[1] = curColor[2] = 0.0;
    break;
  case COLOR_GREEN:
    curColor[1] = 1.0;
    curColor[0] = curColor[2] = 0.0;
    break;
  case COLOR_BLUE:
    curColor[2] = 1.0;
    curColor[1] = curColor[0] = 0.0;
    break;
  
  default:
    break;
  }
}

void  fill_func(int value)
{
  curFill = value;
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

  size_m = glutCreateMenu(size_func); /* Create size-menu */
  glutAddMenuEntry("x2", SIZE_X2);
  glutAddMenuEntry("x4", SIZE_X4);
  glutAddMenuEntry("x8", SIZE_X8);

  color_m = glutCreateMenu(color_func); /* Create size-menu */
  glutAddMenuEntry("Red", COLOR_RED);
  glutAddMenuEntry("Green", COLOR_GREEN);
  glutAddMenuEntry("Blue", COLOR_BLUE);

  fill_m = glutCreateMenu(fill_func); /* Create fill-menu */
  glutAddMenuEntry("Fill", 1);
  glutAddMenuEntry("No fill", COLOR_NO_FILL);

  glutCreateMenu(top_menu_func);
  glutAddSubMenu("Draw ", draw_m);
  glutAddMenuEntry("String ", INPUT_STRING);
  glutAddSubMenu("Size ", size_m);
  glutAddSubMenu("Color ", color_m);
  glutAddSubMenu("Fill ", fill_m);
  //glutAddSubMenu("File", );
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  /*---Enter the event loop ----*/
  glutMainLoop();       
}


/* menu目前有屬性 可加入exit  save or read 加入grid-line
 */