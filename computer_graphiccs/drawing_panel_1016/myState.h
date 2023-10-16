/* current state */
#include <iostream>
#include <cstdlib>
#pragma once
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
#define    SHOW  1
#define    NO_SHOW  0


class MyState
{
private:
  
public:
  static int winHeight;
  static int winWidth;

  static int curMode;
  static bool exist_one; // check if this object has at least 1 position
  static float curSize;
  static float curColor[3];
  static int curFill; //macro

  static int pos_x, pos_y;  // pos_x < 0 means the current position is not valid
  static int pos_x_end, pos_y_end; // for DRAW_LINE to show line in real-time
  static float mid_x, mid_y, radius; // mid-point for DRAW_CIRCLE to show circle in real-time

  static void stateInit(){
    fprintf(stderr, "state has been reset\n");
    curMode = DO_NOTHING;
    exist_one = 0;
    pos_x=-1;
  }
};

int MyState::winHeight=720, MyState::winWidth=1280;
int MyState::curMode=DO_NOTHING;
bool MyState::exist_one=false; // check if this object has at least 1 position
float MyState::curSize=1.0;
float MyState::curColor[3] = {1.0, 1.0, 1.0};
int MyState::curFill; //macro

int MyState::pos_x=-1, MyState::pos_y=-1;  // pos_x < 0 means the current position is not valid
int MyState::pos_x_end=-1, MyState::pos_y_end=-1; // for DRAW_LINE to show line in real-time
float MyState::mid_x=-1, MyState::mid_y=-1, MyState::radius=-1; // mid-point for DRAW_CIRCLE to show circle in real-time


/*store size of main window*/
/*
int winHeight=720, winWidth=1280;
*/
/*
int curMode=DO_NOTHING;
int exist_one=0; // check if this object has at least 1 position
float curSize=1.0;
float curColor[3] = {1.0, 1.0, 1.0};
int curFill; //macro
*/
/*
int pos_x=-1, pos_y;  // pos_x < 0 means the current position is not valid
int pos_x_end, pos_y_end; // for DRAW_LINE to show line in real-time
float mid_x, mid_y, radius; // mid-point for DRAW_CIRCLE to show circle in real-time
*/
/* reset the current states */
/*
void stateInit(){
  fprintf(stderr, "state has been reset\n");
  curMode = DO_NOTHING;
  exist_one = 0;
  pos_x=-1;
}
*/
