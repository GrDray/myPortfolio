/******************************************************************
 * Building a drawing panel.
 * WARNING pen function may be dangerous
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
#define    MENU_EXIT  8
#define    INPUT_CURSOR  9
#define    MENU_CLEAN  10
#define    COLOR_NO_FILL 0
#define    COLOR_FILL  1
#define    COLOR_RED  2
#define    COLOR_GREEN  3
#define    COLOR_BLUE  4
#define    MAX_STRING_LEN  50
#define    SHOW  1
#define    NO_SHOW  0

/* define variables for menu */
typedef    int   menu_t;
menu_t     polygon_m, draw_m, string_m, size_m, color_m, fill_m;

/* current state */
int curMode = DO_NOTHING; // macro
int exist_one=0; // check if this object has at least 1 position
float curSize=1.0;
float curColor[3] = {1.0, 1.0, 1.0};
int curFill = COLOR_NO_FILL; //macro

int pos_x, pos_y;
int pos_x_end, pos_y_end; // for DRAW_LINE to show line in real-time
float mid_x, mid_y, radius; // mid-point for DRAW_CIRCLE to show circle in real-time

/* set the current states to origin */
void stateInit(){
  curMode = DO_NOTHING;
  exist_one = 0;
}

/*store size of main window*/
int winHeight=640, winWidth=960;

/* define object */
struct Position
{
  float x, y;
  struct Position *next;
};
typedef struct Position position;

struct Object
{
  GLUquadricObj *special_type; // for complex objects like circle
  int this_type, this_size, if_fill, if_show;
  float this_color[3];
  char string[MAX_STRING_LEN];
  position *pos;
  /* set the bound of this object 
   * bound[0] for top-left, bound[1] for bottom-right
   */
  position bound[2];
  struct Object *next;
};
typedef struct Object object;
object *first_objects=NULL; // store the first object
object *cur_objects=NULL; // store the current (latest) object

/* create new object and set cur_object point to new location */
void newObject(int type, float size, int fill, int show, float *color) {
  object *newObject = (object *)malloc(sizeof(object));
  newObject->special_type = NULL;
  newObject->this_type = type;
  newObject->this_size = size;
  newObject->if_fill = fill;
  newObject->if_show = show;
  newObject->next = NULL;
  newObject->pos = NULL;
  for(int i=0; i<3; i++) newObject->this_color[i] = color[i];
  /* renew the linked list pointer */
  if(!first_objects){
    first_objects = newObject;
    cur_objects = newObject;
  }else{
    cur_objects->next = newObject;
    cur_objects = newObject;
  }
}

/* delete object and release memory */
void deleteObject(object *obj){
  object *pos = obj->pos;
  while (pos != NULL) {
      position *temp = pos;
      pos = pos->next;
      free(temp);
      temp = NULL;
    }
  free(obj);
}

/* delete all objects and release memory */
void deleteAllObjects() {
  while (first_objects != NULL) {
    position *pos = first_objects->pos;
    while (pos != NULL) {
      position *temp = pos;
      pos = pos->next;
      free(temp);
      temp = NULL;
    }
    object *temp = first_objects;
    first_objects = first_objects->next;
    free(temp);
    temp = NULL;
  }
  cur_objects = NULL;
  first_objects = NULL;
}

/* add new xy positions to given object*/
void addPosToObject(object *obj, int x, int y){
  position *newPos = (position *)malloc(sizeof(position));
  newPos->x = (float)x;
  newPos->y = (float)y;
  newPos->next = NULL;
  if (obj->pos == NULL) {
    obj->pos = newPos;
    obj->bound[0].x = obj->bound[1].x = x;
    obj->bound[0].y = obj->bound[1].y = y;
  }else{
    position *current = obj->pos;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newPos;
    if(x<obj->bound[0].x){
      obj->bound[0].x = x;
    }else if(x>obj->bound[1].x){
      obj->bound[1].x = x;
    }
    if(y>obj->bound[0].y){
      obj->bound[0].y = y;
    }else if(y<obj->bound[1].y){
      obj->bound[1].y = y;
    }
  }
}

/* for the gluNewQuadric object */
void specialTypeInit(object *obj){
  if(obj->special_type==NULL){
    obj->special_type = gluNewQuadric();
    gluQuadricDrawStyle(obj->special_type, GLU_FILL);
  }
}

/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void drawCircle(object *obj)
{
  if(!obj->special_type) return;
  position *pos = obj->pos;
  position *pos_next = pos->next;
  float radius = 0.5*sqrt(pow(pos->x-pos_next->x, 2)+pow(pos->y-pos_next->y, 2));
  glPushMatrix();
  glTranslatef(0.5*(pos->x+pos_next->x), 0.5*(pos->y+pos_next->y), 0.0);
  gluDisk(obj->special_type,
    (obj->if_fill)?0.0:(radius - obj->this_size),           /* inner radius=0.0 */
	  radius,    /* outer radius=10.0 */
	  25,            /* 16-side polygon */
	  3);
  glPopMatrix();
}


/* show objects */
void showObject(object *obj){
  /* check if we show it*/
  if(!obj->if_show) return;
  /* apply openGL states */
  glPointSize(obj->this_size);
  glColor3fv(obj->this_color);
  /* show objects depend on its type */
  switch (obj->this_type){
  case POLYGON_FINISH:
    position *pos_poly = obj->pos;
    /*set if fill the polygon*/
    glPolygonMode(GL_FRONT_AND_BACK, (obj->if_fill)?GL_FILL:GL_LINE);
    glBegin(GL_POLYGON);
      while(pos_poly){
        glVertex2i(pos_poly->x, pos_poly->y);
        pos_poly = pos_poly->next;
      }
    glEnd();
    break;
  case DRAW_LINE:
    position *pos_line = obj->pos;
    if(!pos_line || !pos_line->next) return;
    position *pos_line_next = pos_line->next;
    glLineWidth(obj->this_size);
    glBegin(GL_LINES);
      glVertex3f(pos_line->x, pos_line->y, 0.0);
      glVertex3f(pos_line_next->x, pos_line_next->y, 0.0);
    glEnd();
    break;
  case DRAW_CIRCLE:
    position *pos_circle = obj->pos;
    if(!pos_circle || !pos_circle->next) return;
    glLineWidth(obj->this_size);
    drawCircle(obj);
    break;
  case DRAW_PEN:
    position *pos_pen = obj->pos;
    position *pos_pen_next = pos_pen->next;
    glLineWidth(obj->this_size);
    glBegin(GL_LINES);
      while(pos_pen&&pos_pen_next){
        glVertex3f(pos_pen->x, pos_pen->y, 0.0);
        glVertex3f(pos_pen_next->x, pos_pen_next->y, 0.0);
        if(!pos_pen_next->next) break;
        pos_pen = pos_pen_next->next;
        pos_pen_next = pos_pen->next;
      }
    glEnd();
    break;
  default:
    break;
  }
  glFlush();
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
  /* in order to show line or circle drawing in real-time */
  if(exist_one==1){
    switch (curMode){
    case DRAW_LINE:
      glBegin(GL_LINES);
        glVertex3f(pos_x, pos_y, 0.0);
        glVertex3f(pos_x_end, pos_y_end, 0.0);
      glEnd();
      break;
    case DRAW_CIRCLE:
      glPushMatrix();
      glTranslatef(mid_x, mid_y, 0.0);
      gluDisk(cur_objects->special_type, (curFill)?0.0:(radius - curSize),
       radius, 16, 3);
      glPopMatrix();
      break;
    default:
      break;
    }
  }
  /* show all objects */
  object *temp = first_objects;
  while(temp){
    showObject(temp);
    temp = temp->next;
  }
  glFinish();
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
  if(key=='Q'||key=='q') {
    deleteAllObjects();
    exit(0);
  }
}

/*------------------------------------------------------------
 * Callback function handling mouse events
 */
void mouse_func(int button, int state, int x, int y)
{
  if(button!=GLUT_LEFT_BUTTON){
    return;
  }
  switch (curMode){
  case POLYGON_START:
    if(state!=GLUT_DOWN) return; // only when mouse press down
    if(!exist_one){    // if the object not yet have one position
      newObject(POLYGON_START, curSize, curFill, SHOW, curColor);
      exist_one = 1;
    }
    addPosToObject(cur_objects, x, winHeight - y);
    break;
  case DRAW_LINE:
    if(state==GLUT_DOWN){
      exist_one = 1;
      pos_x=x; pos_y=winHeight-y;
      newObject(DRAW_LINE, curSize, curFill, SHOW, curColor);
      addPosToObject(cur_objects, x, pos_y);
    }else if(state==GLUT_UP){
      exist_one = 0;
      addPosToObject(cur_objects, x, winHeight-y);
    }
    break;
  case DRAW_CIRCLE:
    if(state==GLUT_DOWN){
      exist_one = 1;
      pos_x=x; pos_y=winHeight-y;
      newObject(DRAW_CIRCLE, curSize, curFill, SHOW, curColor);
      specialTypeInit(cur_objects);
      addPosToObject(cur_objects, pos_x, pos_y);
    }else if(state==GLUT_UP){
      exist_one = 0;
      addPosToObject(cur_objects, x, winHeight-y);
    }
    break;
  case DRAW_RECTANGLE:
    if(exist_one==0){
      exist_one = 1;
      pos_x = x;
      pos_y = winHeight - y;
	    glPointSize(curSize);
      glBegin(GL_POINTS);   /*  Draw the 1st point */
	      glVertex2f(x, winHeight-y);
      glEnd();
    }else{
      exist_one=0;
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
  case INPUT_STRING:
    pos_x = x;  
    pos_y = winHeight - y;
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
  switch (curMode){
  case DRAW_LINE:
    pos_x_end=x; pos_y_end=winHeight - y;
    /* use display callback to show line in real-time */
    glutPostRedisplay();
    break;
  case DRAW_CIRCLE:
    pos_x_end=x; pos_y_end=winHeight - y;
    mid_x = 0.5*(pos_x+pos_x_end);
    mid_y = 0.5*(pos_y+pos_y_end);
    radius = 0.5*sqrt(pow(pos_x-pos_x_end, 2)+pow(pos_y-pos_y_end, 2));
    /* use display callback to show line in real-time */
    glutPostRedisplay();
    break;
  case DRAW_PEN:
    if(exist_one){    // if the object not yet have one position
      pos_x = x; pos_y = winHeight - y;
    }else{
      newObject(DRAW_PEN, curSize, curFill, SHOW, curColor);
      exist_one = 1;
      pos_x = x; pos_y = winHeight - y;
    }
    addPosToObject(cur_objects, pos_x, pos_y); // store current position
    /* use display callback to show in real-time */
    glutPostRedisplay();
    break;
  default:
    break;
  }
  glFinish();
}




/*---------------------------------------------------------------
 * Callback function for top menu.
 */
void top_menu_func(int value)
{
  /*make sure everytime we want to create a new object, the exist_one=0
   * and curMode=DO_NOTHING
   */
  stateInit();
  switch (value){
  /* we can exit and make sure we 'free' all memory that 'malloc' give */
  case MENU_EXIT:
    deleteAllObjects();
    exit(0);
    break;
  case MENU_CLEAN:
    deleteAllObjects();
    glutPostRedisplay();
    break;
  /* we can click anywhere and nothing would happen */
  case INPUT_CURSOR:
    break;
  case INPUT_STRING:
    curMode = INPUT_STRING;
    break;
  default:
    break;
  }
}

/*-----------------------------------------------------------------
 * function for submenu
 */
void  polygon_func(int value)
{
  /*make sure everytime we want to create a new object, the exist_one=0
   * and curMode=DO_NOTHING
   */
  stateInit();
  switch(value){
  case POLYGON_START:
    fprintf(stderr,"Start drawing polygon.\n");
    curMode = POLYGON_START;
    break;
  case POLYGON_FINISH:
    fprintf(stderr,"Finish drawing polygon.\n");
    cur_objects->this_type = POLYGON_FINISH;
    glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
    break;
  default:
    break;
  }
}

void  draw_func(int value)
{
  /*make sure everytime we want to create a new object, the exist_one=0
   * and curMode=DO_NOTHING
   */
  stateInit();
  switch(value){
  case DRAW_LINE:
    fprintf(stderr,"Start drawing lines.\n");
    curMode = DRAW_LINE;
    break;
  case DRAW_CIRCLE:
    curMode = DRAW_CIRCLE;
    break;
  case DRAW_RECTANGLE:
    curMode = DRAW_RECTANGLE;
    break;
  case DRAW_PEN:
    fprintf(stderr,"Start using pen.\n");
    curMode = DRAW_PEN;
    break;
  default:
    break;
  }
}

void  size_func(int value)
{
  switch (value){
  case 1:
    curSize = 1.0;
    break;
  case 2:
    curSize = 2.0;
    break;
  case 4:
    curSize = 4.0;
    break;
  case 6:
    curSize = 6.0;
    break;
  default:
    break;
  }
  glPointSize(curSize);
  glLineWidth(curSize);
}

void  color_func(int value)
{
  switch (value){
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
  glColor3fv(curColor);
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
  //glutAddMenuEntry("Pen", DRAW_PEN);
  glutAddMenuEntry("Line", DRAW_LINE);
  glutAddMenuEntry("Circle", DRAW_CIRCLE);
  glutAddMenuEntry("Rectangle", DRAW_RECTANGLE);

  size_m = glutCreateMenu(size_func); /* Create size-menu */
  glutAddMenuEntry("x1", 1);
  glutAddMenuEntry("x2", 2);
  glutAddMenuEntry("x4", 4);
  glutAddMenuEntry("x6", 6);

  color_m = glutCreateMenu(color_func); /* Create size-menu */
  glutAddMenuEntry("Red", COLOR_RED);
  glutAddMenuEntry("Green", COLOR_GREEN);
  glutAddMenuEntry("Blue", COLOR_BLUE);

  fill_m = glutCreateMenu(fill_func); /* Create fill-menu */
  glutAddMenuEntry("Fill", COLOR_FILL);
  glutAddMenuEntry("No fill", COLOR_NO_FILL);

  glutCreateMenu(top_menu_func);
  glutAddMenuEntry("Cursor", INPUT_CURSOR);
  glutAddSubMenu("Draw ", draw_m);
  glutAddMenuEntry("String ", INPUT_STRING);
  glutAddSubMenu("Size ", size_m);
  glutAddSubMenu("Color ", color_m);
  glutAddSubMenu("Fill ", fill_m);
  glutAddMenuEntry("Clean", MENU_CLEAN);
  glutAddMenuEntry("Exit", MENU_EXIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  /*---Enter the event loop ----*/
  glutMainLoop();       
}


/* menu目前有屬性 可加入exit  save or read 加入grid-line
 */