/******************************************************************
 * Building a drawing panel.
 * WARNING pen function may be dangerous
 */
#include "myObject.h"

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
      {
      glBegin(GL_LINES);
        glVertex3f(pos_x, pos_y, 0.0);
        glVertex3f(pos_x_end, pos_y_end, 0.0);
      glEnd();
      break;
      }
    case DRAW_CIRCLE:
      {
      glPushMatrix();
      glTranslatef(mid_x, mid_y, 0.0);
      gluDisk(cur_objects->special_type, (curFill)?0.0:(radius - curSize),
       radius, 16, 3);
      glPopMatrix();
      break;
      }
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
    {
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
    }
  case DO_NOTHING:
    {
    if(key=='Q'||key=='q') {
    deleteAllObjects();
    exit(0);
    }
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
    {
    if(state!=GLUT_DOWN) return; // only when mouse press down
    if(!exist_one){    // if the object not yet have one position
      newObject(POLYGON_START, curSize, curFill, SHOW, curColor);
      exist_one = 1;
    }
    addPosToObject(cur_objects, x, winHeight - y);
    break;
    }
  case DRAW_LINE:
    {
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
    }
  case DRAW_CIRCLE:
    {
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
    }
  case DRAW_POINT:
    {
    if(state==GLUT_DOWN){
      newObject(DRAW_POINT, curSize, curFill, SHOW, curColor);
      addPosToObject(cur_objects, x, winHeight-y);
      glutPostRedisplay();
    }
    break;
    }
  case INPUT_STRING:
    {
    if(!exist_one && state==GLUT_DOWN){
      pos_x = x;  
      pos_y = winHeight - y;
    }else if(exist_one && state==GLUT_DOWN){
      stateInit();
    }
    break;
    }
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
  glutAddMenuEntry("Point", DRAW_POINT);
  glutAddMenuEntry("Line", DRAW_LINE);
  glutAddMenuEntry("Circle", DRAW_CIRCLE);

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


/* 
  加入grid-line
  idle event or timer
 */