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
#define    DRAW_POINT  11
#define    COLOR_NO_FILL 0
#define    COLOR_FILL  1
#define    COLOR_RED  2
#define    COLOR_GREEN  3
#define    COLOR_BLUE  4
#define    COLOR_WHITE  5
#define    COLOR_YELLOW  6
#define    COLOR_CYAN  7
#define    COLOR_PURPLE  8
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

int pos_x=-1, pos_y;  // pos_x < 0 means the current position is not valid
int pos_x_end, pos_y_end; // for DRAW_LINE to show line in real-time
float mid_x, mid_y, radius; // mid-point for DRAW_CIRCLE to show circle in real-time

/* reset the current states */
void stateInit(){
  fprintf(stderr, "state has been reset\n");
  curMode = DO_NOTHING;
  exist_one = 0;
  pos_x=-1;
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
  position *pos;
  /* set the bound of this object 
   * bound[0] for top-left, bound[1] for bottom-right
   */
  position bound[2];
  /*if it is string type, then use bound[1] to store start point
   *   and use if_fill as the length of string 
   */
  char string[MAX_STRING_LEN];
  struct Object *next;
};
typedef struct Object object;
object *first_objects=NULL; // store the first object
object *cur_objects=NULL; // store the current (latest) object

/* create new object and set cur_object point to new location */
void newObject(int type, float size, int fill, int show, float *color) {
  object *newObject = (object *)malloc(sizeof(object));
  if(!newObject){
    fprintf(stderr, "newObject() failed\n");
    return;
  }else{
    fprintf(stderr, "newObject() successed\n");
  }
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
  fprintf(stderr, "deleteAllObjects() successed\n");
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

/* Procedure to draw a circle */
void drawCircle(object *obj)
{
  if(!obj->special_type) return;
  position *pos = obj->pos;
  position *pos_next = pos->next;
  float radius = 0.5*sqrt(pow(pos->x-pos_next->x, 2)+pow(pos->y-pos_next->y, 2));
  glPushMatrix();
  glTranslatef(0.5*(pos->x+pos_next->x), 0.5*(pos->y+pos_next->y), 0.0);
  gluDisk(obj->special_type,
    (obj->if_fill)?0.0:(radius - obj->this_size),   /* inner radius */
	  radius,    /* outer radius */
	  25,            /* 25-side polygon */
	  3);
  glPopMatrix();
}

/* show objects */
void showObject(object *obj){
  /* check if we show it*/
  if(!obj->if_show) return;
  /* apply openGL states */
  glColor3fv(obj->this_color);
  /* show objects depend on its type */
  switch (obj->this_type){
  case POLYGON_FINISH:
    position *pos_poly = obj->pos;
    /*set if fill the polygon*/
    glPolygonMode(GL_FRONT_AND_BACK, (obj->if_fill)?GL_FILL:GL_LINE);
    glLineWidth(obj->this_size);
    glBegin(GL_POLYGON);
      while(pos_poly){
        glVertex2i(pos_poly->x, pos_poly->y);
        pos_poly = pos_poly->next;
      }
    glEnd();
    break;
  case DRAW_CIRCLE:
    position *pos_circle = obj->pos;
    if(!pos_circle || !pos_circle->next) return;
    drawCircle(obj);
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
  case INPUT_STRING:
    int pos_str_x = obj->bound[1].x;
    int pos_str_y = obj->bound[1].y;
    for(int i=0; i<obj->if_fill; i++){
      glRasterPos2i(pos_str_x, pos_str_y);
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int) obj->string[i]);
      pos_str_x += 10;
    }
    break;
  case DRAW_POINT:
    if(!obj->pos) return;
    glPointSize(obj->this_size);
    glBegin(GL_POINTS);     /*  Draw a point */
      glVertex2f(obj->pos->x, obj->pos->y);
    glEnd();
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
    glColor3fv(curColor);
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
  switch (curMode){
  case INPUT_STRING:
    if(pos_x<0) return;
    if(!exist_one){
      exist_one = 1;
      newObject(INPUT_STRING, curSize, curFill, SHOW, curColor);
      cur_objects->bound[1].x = pos_x;
      cur_objects->bound[1].y = pos_y;
      cur_objects->if_fill = 0;
    }
    int str_len = cur_objects->if_fill;
    if(key==8){
      if(str_len) {
        cur_objects->if_fill -= 1;
        fprintf(stderr, "Backspace for word cancellation.\n");
      }
    }else if(str_len<MAX_STRING_LEN-1){
      cur_objects->string[str_len] = key;
      cur_objects->if_fill += 1;
    }else{
      fprintf(stderr, "over the length limit.\n");
    }
    break;
  case DO_NOTHING:
    if(key=='Q'||key=='q') {
    deleteAllObjects();
    exit(0);
  }
  default:
    break;
  }
  glutPostRedisplay();
  glFinish();
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
  case DRAW_POINT:
    if(state==GLUT_DOWN){
      newObject(DRAW_POINT, curSize, curFill, SHOW, curColor);
      addPosToObject(cur_objects, x, winHeight-y);
      glutPostRedisplay();
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
    if(!exist_one && state==GLUT_DOWN){
      pos_x = x;  
      pos_y = winHeight - y;
    }else if(exist_one && state==GLUT_DOWN){
      stateInit();
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
  switch (curMode){
  case DRAW_LINE:
    pos_x_end=x; pos_y_end=winHeight - y;
    break;
  case DRAW_CIRCLE:
    pos_x_end=x; pos_y_end=winHeight - y;
    mid_x = 0.5*(pos_x+pos_x_end);
    mid_y = 0.5*(pos_y+pos_y_end);
    radius = 0.5*sqrt(pow(pos_x-pos_x_end, 2)+pow(pos_y-pos_y_end, 2));
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
    break;
  default:
    break;
  }
  /* use display callback to show line in real-time */
  glutPostRedisplay();
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
  case INPUT_STRING:
    curMode = INPUT_STRING;
    break;
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
   * and curMode=DO_NOTHING and pos_x < 0
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
  case DRAW_POINT:
    curMode = DRAW_POINT;
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
  case COLOR_WHITE:
    curColor[0] = curColor[1] = curColor[2] = 1.0;
    break;
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
  case COLOR_YELLOW:
    curColor[2] = 0.0;
    curColor[0] = curColor[1] = 1.0;
    break;
  case COLOR_CYAN:
    curColor[0] = 0.0;
    curColor[1] = curColor[2] = 1.0;
    break;
  case COLOR_PURPLE:
    curColor[1] = 0.0;
    curColor[0] = curColor[2] = 0.5;
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
  glutAddMenuEntry("Point", DRAW_POINT);
  glutAddMenuEntry("Line", DRAW_LINE);
  glutAddMenuEntry("Circle", DRAW_CIRCLE);
  glutAddMenuEntry("Rectangle", DRAW_RECTANGLE);

  size_m = glutCreateMenu(size_func); /* Create size-menu */
  glutAddMenuEntry("x1", 1);
  glutAddMenuEntry("x2", 2);
  glutAddMenuEntry("x4", 4);
  glutAddMenuEntry("x6", 6);

  color_m = glutCreateMenu(color_func); /* Create size-menu */
  glutAddMenuEntry("White", COLOR_WHITE);
  glutAddMenuEntry("Red", COLOR_RED);
  glutAddMenuEntry("Green", COLOR_GREEN);
  glutAddMenuEntry("Blue", COLOR_BLUE);
  glutAddMenuEntry("Yellow", COLOR_YELLOW);
  glutAddMenuEntry("Cyan", COLOR_CYAN);
  glutAddMenuEntry("Purple", COLOR_PURPLE);

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


/*加入grid-line
  idle event or timer
 */