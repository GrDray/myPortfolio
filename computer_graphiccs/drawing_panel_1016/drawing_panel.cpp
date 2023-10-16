/******************************************************************
 * Building a drawing panel.
 * WARNING pen function may be dangerous
 */
#include "myObject.h"
#include "myState.h"
#include "myGUI.h"
#include "myGlutMenu.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif


/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
int main(int argc, char **argv)
{
  /* Initialization */
  glutInit(&argc, argv);
  #ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  #endif
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(1280, 720);
  glutCreateWindow("Drawing Panel"); // Create window
  
  MyGUI::Setup();
  MyGlutMenu::Setup();
  /*---Enter the event loop ----*/
  glutMainLoop();    

  MyGUI::Shutdown();

  return 0;   
}


/* 
  加入grid-line
  idle event or timer
 */