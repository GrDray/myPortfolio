/******************************************************************
 * Building a drawing panel.
 * WARNING pen function may be dangerous
 */
#include "myObject.h"
#include "myState.h"

/* define variables for menu */
typedef    int   menu_t;
menu_t     polygon_m, draw_m, string_m, size_m, color_m, fill_m;

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
  if(MyState::exist_one==1){
    glColor3fv(MyState::curColor);
    switch (MyState::curMode){
    case DRAW_LINE:
      {
      glBegin(GL_LINES);
        glVertex3f(MyState::pos_x, MyState::pos_y, 0.0);
        glVertex3f(MyState::pos_x_end, MyState::pos_y_end, 0.0);
      glEnd();
      break;
      }
    case DRAW_CIRCLE:
      {
      glPushMatrix();
      glTranslatef(MyState::mid_x, MyState::mid_y, 0.0);
      gluDisk(MyObject::cur_objects->special_type, (MyState::curFill)?0.0:(MyState::radius - MyState::curSize),
       MyState::radius, 16, 3);
      glPopMatrix();
      break;
      }
    default:
      break;
    }
  }
  /* show all objects */
  object *temp = MyObject::first_objects;
  while(temp){
    MyObject::showObject(temp);
    temp = temp->next;
  }
  glFinish();
}

/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void reshape_func(int new_w, int new_h)
{
  MyState::winHeight = new_h;
  MyState::winWidth = new_w;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) MyState::winWidth, 0.0, (double) MyState::winHeight);
  glViewport(0,0,MyState::winWidth,MyState::winHeight);
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
  switch (MyState::curMode){
  case INPUT_STRING:
    {
    if(MyState::pos_x<0) return;
    if(!MyState::exist_one){
      MyState::exist_one = 1;
      MyObject::newObject(OBJ_STRING, MyState::curSize, MyState::curFill, SHOW, MyState::curColor);
      MyObject::cur_objects->bound[1].x = MyState::pos_x;
      MyObject::cur_objects->bound[1].y = MyState::pos_y;
      MyObject::cur_objects->if_fill = 0;
    }
    int str_len = MyObject::cur_objects->if_fill;
    if(key==8){
      if(str_len) {
        MyObject::cur_objects->if_fill -= 1;
        fprintf(stderr, "Backspace for word cancellation.\n");
      }
    }else if(str_len<MAX_STRING_LEN-1){
      MyObject::cur_objects->string[str_len] = key;
      MyObject::cur_objects->if_fill += 1;
    }else{
      fprintf(stderr, "over the length limit.\n");
    }
    break;
    }
  case DO_NOTHING:
    {
    if(key=='Q'||key=='q') {
    MyObject::deleteAllObjects();
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
  switch (MyState::curMode){
  case POLYGON_START:
    {
    if(state!=GLUT_DOWN) return; // only when mouse press down
    if(!MyState::exist_one){    // if the object not yet have one position
      MyObject::newObject(POLYGON_START, MyState::curSize, MyState::curFill, SHOW, MyState::curColor);
      MyState::exist_one = 1;
    }
    MyObject::addPosToObject(MyObject::cur_objects, x, MyState::winHeight - y);
    break;
    }
  case DRAW_LINE:
    {
    if(state==GLUT_DOWN){
      MyState::exist_one = 1;
      MyState::pos_x=x; MyState::pos_y=MyState::winHeight-y;
      MyObject::newObject(OBJ_LINE, MyState::curSize, MyState::curFill, SHOW, MyState::curColor);
      MyObject::addPosToObject(MyObject::cur_objects, x, MyState::pos_y);
    }else if(state==GLUT_UP){
      MyState::exist_one = 0;
      MyObject::addPosToObject(MyObject::cur_objects, x, MyState::winHeight-y);
    }
    break;
    }
  case DRAW_CIRCLE:
    {
    if(state==GLUT_DOWN){
      MyState::exist_one = 1;
      MyState::pos_x=x; MyState::pos_y=MyState::winHeight-y;
      MyObject::newObject(OBJ_CIRCLE, MyState::curSize, MyState::curFill, SHOW, MyState::curColor);
      MyObject::specialTypeInit(MyObject::cur_objects);
      MyObject::addPosToObject(MyObject::cur_objects, MyState::pos_x, MyState::pos_y);
    }else if(state==GLUT_UP){
      MyState::exist_one = 0;
      MyObject::addPosToObject(MyObject::cur_objects, x, MyState::winHeight-y);
    }
    break;
    }
  case DRAW_POINT:
    {
    if(state==GLUT_DOWN){
      MyObject::newObject(OBJ_POINT, MyState::curSize, MyState::curFill, SHOW, MyState::curColor);
      MyObject::addPosToObject(MyObject::cur_objects, x, MyState::winHeight-y);
      glutPostRedisplay();
    }
    break;
    }
  case INPUT_STRING:
    {
    if(!MyState::exist_one && state==GLUT_DOWN){
      MyState::pos_x = x;  
      MyState::pos_y = MyState::winHeight - y;
    }else if(MyState::exist_one && state==GLUT_DOWN){
      MyState::stateInit();
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
  switch (MyState::curMode){
  case DRAW_LINE:
    MyState::pos_x_end=x; MyState::pos_y_end=MyState::winHeight - y;
    break;
  case DRAW_CIRCLE:
    MyState::pos_x_end=x; MyState::pos_y_end=MyState::winHeight - y;
    MyState::mid_x = 0.5*(MyState::pos_x+MyState::pos_x_end);
    MyState::mid_y = 0.5*(MyState::pos_y+MyState::pos_y_end);
    MyState::radius = 0.5*sqrt(pow(MyState::pos_x-MyState::pos_x_end, 2)+pow(MyState::pos_y-MyState::pos_y_end, 2));
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
  MyState::stateInit();
  switch (value){
  case INPUT_STRING:
    MyState::curMode = INPUT_STRING;
    break;
  /* we can exit and make sure we 'free' all memory that 'malloc' give */
  case MENU_EXIT:
    MyObject::deleteAllObjects();
    exit(0);
    break;
  case MENU_CLEAN:
    MyObject::deleteAllObjects();
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
  MyState::stateInit();
  switch(value){
  case POLYGON_START:
    fprintf(stderr,"Start drawing polygon.\n");
    MyState::curMode = POLYGON_START;
    break;
  case POLYGON_FINISH:
    fprintf(stderr,"Finish drawing polygon.\n");
    MyObject::cur_objects->this_type = OBJ_POLYGON;
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
  MyState::stateInit();
  switch(value){
  case DRAW_LINE:
    fprintf(stderr,"Start drawing lines.\n");
    MyState::curMode = DRAW_LINE;
    break;
  case DRAW_CIRCLE:
    MyState::curMode = DRAW_CIRCLE;
    break;
  case DRAW_POINT:
    MyState::curMode = DRAW_POINT;
    break;
  default:
    break;
  }
}

void  size_func(int value)
{
  switch (value){
  case 1:
    MyState::curSize = 1.0;
    break;
  case 2:
    MyState::curSize = 2.0;
    break;
  case 4:
    MyState::curSize = 4.0;
    break;
  case 6:
    MyState::curSize = 6.0;
    break;
  default:
    break;
  }
  glPointSize(MyState::curSize);
  glLineWidth(MyState::curSize);
}

void  color_func(int value)
{
  switch (value){
  case COLOR_WHITE:
    MyState::curColor[0] = MyState::curColor[1] = MyState::curColor[2] = 1.0;
    break;
  case COLOR_RED:
    MyState::curColor[0] = 1.0;
    MyState::curColor[1] = MyState::curColor[2] = 0.0;
    break;
  case COLOR_GREEN:
    MyState::curColor[1] = 1.0;
    MyState::curColor[0] = MyState::curColor[2] = 0.0;
    break;
  case COLOR_BLUE:
    MyState::curColor[2] = 1.0;
    MyState::curColor[1] = MyState::curColor[0] = 0.0;
    break;
  case COLOR_YELLOW:
    MyState::curColor[2] = 0.0;
    MyState::curColor[0] = MyState::curColor[1] = 1.0;
    break;
  case COLOR_CYAN:
    MyState::curColor[0] = 0.0;
    MyState::curColor[1] = MyState::curColor[2] = 1.0;
    break;
  case COLOR_PURPLE:
    MyState::curColor[1] = 0.0;
    MyState::curColor[0] = MyState::curColor[2] = 0.5;
    break;
  default:
    break;
  }
  glColor3fv(MyState::curColor);
}

void  fill_func(int value)
{
  MyState::curFill = value;
}

/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
int main(int argc, char **argv)
{
  /* Initialization */
  glutInit(&argc, argv);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(MyState::winWidth, MyState::winHeight);
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