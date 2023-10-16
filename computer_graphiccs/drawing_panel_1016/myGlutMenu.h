#include <iostream>
#include <cstdlib>
#pragma once
#include <GL/freeglut.h>
#include "myState.h"
#include "myObject.h"



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



class MyGlutMenu {
  public:
    /* define variables for menu */
    static int polygon_m, draw_m, string_m, size_m, color_m, fill_m;

    static void Setup() {
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

    }
  private:
};

int MyGlutMenu::polygon_m, MyGlutMenu::draw_m, 
    MyGlutMenu::string_m, MyGlutMenu::size_m, 
    MyGlutMenu::color_m, MyGlutMenu::fill_m;