/* this is created to replace the glut callback install part 
 *    ==> ImGui_ImplGLUT_InstallFuncs()
 * of imgui_impl_glut.h and imgui_impl_glut.cpp 
 *
 * Here I replace ImGui_ImplGLUT_XXX to My_XXX
 *
 *
 *
 */
#include <iostream>
#include <cstdlib>
#pragma once
#ifndef IMGUI_DISABLE
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#else
#include <GL/freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

#include "heli.h"

static int g_Time = 0;          // Current time, in milliseconds
#endif // #ifndef IMGUI_DISABLE


/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * clear the window
 */
void My_DisplayFunc(void);

/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void My_ReshapeFunc(int new_w, int new_h);

/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void My_KeyboardFunc(unsigned char key, int x, int y);

/*------------------------------------------------------------
 * Callback function handling mouse events
 */
void My_MouseFunc(int button, int state, int x, int y);

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void My_MotionFunc(int  x, int y);

void My_idleFunc();

#ifdef __FREEGLUT_EXT_H__
void My_MouseWheelFunc(int button, int dir, int x, int y);
#endif

void My_KeyboardUpFunc(unsigned char c, int x, int y);

void My_SpecialFunc(int key, int x, int y);

void My_SpecialUpFunc(int key, int x, int y);

static ImGuiKey ImGui_ImplGLUT_KeyToImGuiKey(int key);

static void ImGui_ImplGLUT_UpdateKeyModifiers();

static void ImGui_ImplGLUT_AddKeyEvent(ImGuiKey key, bool down, int native_keycode);

void My_InstallFuncs();


class HeliGUI {
  public:
    // Our state
    static ImVec4 clear_color;
    static bool show_demo_window;
    static bool show_another_window;

    static void Setup() {
      // Setup Dear ImGui context
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
      // Setup Dear ImGui style to light
      ImGui::StyleColorsLight();
      //ImGui::StyleColorsDark();

      // Setup Platform/Renderer backends
      // FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
      ImGui_ImplGLUT_Init();
      ImGui_ImplOpenGL2_Init();
      /* use my own callback functions */
      My_InstallFuncs();
      fprintf(stderr, "myGUI setup\n");
    }

    static void Shutdown(){
      // Cleanup
      ImGui_ImplOpenGL2_Shutdown();
      ImGui_ImplGLUT_Shutdown();
      ImGui::DestroyContext();
    }
  
    static ImGuiIO& GetFrameIO(){
      /* Start the Dear ImGui frame 
       * and return the reference of IO info
       */
      ImGui_ImplOpenGL2_NewFrame();
      ImGui_ImplGLUT_NewFrame();
      ImGui::NewFrame();
      return ImGui::GetIO();
    }
  
    static void Render(){
      // Render
      ImGui::Render();
    }

    static void RenderDrawData(){
      ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }
    
    /* 還未處理好 */
    static void CreatePropertyWindow(){
      {
        ImGuiIO& io = ImGui::GetIO();
        static float slider_altitude = 5.0;

        ImGui::Begin("Properties");                          // Create a window called "Hello, world!" and append into it.
        /*
        ImGui::Text("Setup Size, color and fill mode for your object.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Fill", &checkbox_fill);      // Edit bools storing our window open/close state
        */
        if (ImGui::Button("Reset"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          Heli::reset();
        if (ImGui::Button("Take-off"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          Heli::takeOff();
        if (ImGui::Button("Landing"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          Heli::landing();
        ImGui::SliderFloat("Altitude", &slider_altitude, 5.0, 15.0);            // Edit 1 float using a slider from 0.0f to 1.0f
        if(slider_altitude>5.0){
            Heli::upAndDown(slider_altitude);
        }
        /* control rotating */
        
        if (ImGui::Button("Rotate")){
          Heli::rotate();
        }
        

        //ImGui::SameLine();
        //ImGui::Text("counter = %d", counter);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
      }
    }


  private:
};


ImVec4 HeliGUI::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool HeliGUI::show_demo_window = false;
bool HeliGUI::show_another_window = false;




void My_InstallFuncs(){
  glutDisplayFunc(My_DisplayFunc);
  glutReshapeFunc(My_ReshapeFunc);
  glutIdleFunc(My_idleFunc);
  glutMotionFunc(My_MotionFunc);
  glutPassiveMotionFunc(My_MotionFunc);
  glutMouseFunc(My_MouseFunc);
#ifdef __FREEGLUT_EXT_H__
  glutMouseWheelFunc(My_MouseWheelFunc);
#endif
  glutKeyboardFunc(My_KeyboardFunc);
  glutKeyboardUpFunc(My_KeyboardUpFunc);
  glutSpecialFunc(My_SpecialFunc);
  glutSpecialUpFunc(My_SpecialUpFunc);
}


/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * clear the window
 */
void My_DisplayFunc(void)
{
  ImGuiIO& io = HeliGUI::GetFrameIO();
  if (HeliGUI::show_demo_window) ImGui::ShowDemoWindow(&HeliGUI::show_demo_window);
  HeliGUI::CreatePropertyWindow();
  HeliGUI::Render();

  glClearColor (0.2, 0.2, 0.2, 0.0);
  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
  
  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();           /* Initialize modelview matrix */
  //gluLookAt( 0.0, 10, 10.0, 0.0, 0.0, 8.0, 0.0, 1.0, 0.0); // upper view
  gluLookAt( 0.0, 20, 50.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0); // normal view

  Heli::drawBuilding();
  Heli::moveOptions();
  Heli::drawHeli();

  HeliGUI::RenderDrawData();
  glutSwapBuffers();
  glutPostRedisplay();
}




/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void My_ReshapeFunc(int w, int h)
{
std::cerr << "reshape called\n" << std::endl;

  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)w, (float)h);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  if(w>h)
//    glOrtho(-15.0, 15.0, -15.0*(float)h/(float)w, 15.0*(float)h/(float)w, 
//            -10.5, 30.0);

    glOrtho(-30.0, 30.0, -30.0*(float)h/(float)w, 30.0*(float)h/(float)w, 
            -80, 80.0);
  else
    glOrtho(-15.0*(float)w/(float)h, 15.0*(float)w/(float)h, -15.0, 15.0, 
            -10.5, 30.0);

  glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
}



/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void My_KeyboardFunc(unsigned char key, int x, int y)
{
  // Send character to imgui
  //printf("char_down_func %d '%c'\n", c, c);
  ImGuiIO& io = ImGui::GetIO();
  if (key >= 32)
      io.AddInputCharacter((unsigned int)key);

  ImGuiKey c = ImGui_ImplGLUT_KeyToImGuiKey(key);
  ImGui_ImplGLUT_AddKeyEvent(c, true, key);
  ImGui_ImplGLUT_UpdateKeyModifiers();
  (void)x; (void)y; // Unused

  glutPostRedisplay();
  glFinish();
}

/*------------------------------------------------------------
 * Callback function handling mouse events
 */
void My_MouseFunc(int button, int state, int x, int y)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent((float)x, (float)y);

  int glut_button = -1;
  if (button == GLUT_LEFT_BUTTON) glut_button = 0;
  if (button == GLUT_RIGHT_BUTTON) glut_button = 1;
  if (button == GLUT_MIDDLE_BUTTON) glut_button = 2;
  if (glut_button != -1 && (state == GLUT_DOWN || state == GLUT_UP))
      io.AddMouseButtonEvent(glut_button, state == GLUT_DOWN);

  glFinish();
}

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void My_MotionFunc(int  x, int y)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent((float)x, (float)y);

  
  glutPostRedisplay();
  glFinish();
}


#ifdef __FREEGLUT_EXT_H__
void My_MouseWheelFunc(int button, int dir, int x, int y)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddMousePosEvent((float)x, (float)y);
  if (dir != 0)
    io.AddMouseWheelEvent(0.0f, dir > 0 ? 1.0f : -1.0f);
  (void)button; // Unused
}
#endif


void My_KeyboardUpFunc(unsigned char c, int x, int y)
{
    //printf("char_up_func %d '%c'\n", c, c);
    ImGuiKey key = ImGui_ImplGLUT_KeyToImGuiKey(c);
    ImGui_ImplGLUT_AddKeyEvent(key, false, c);
    ImGui_ImplGLUT_UpdateKeyModifiers();
    (void)x; (void)y; // Unused
}

void My_SpecialFunc(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_UP:
      Heli::flyForward();
      break;
    case GLUT_KEY_DOWN:
      Heli::hover();
      break;
    case GLUT_KEY_LEFT:
      Heli::flyLeft();
      break;
    case GLUT_KEY_RIGHT:
      Heli::flyRight();
      break;
    default:
     break;
    }

  ImGuiKey imgui_key = ImGui_ImplGLUT_KeyToImGuiKey(key + 256);
  ImGui_ImplGLUT_AddKeyEvent(imgui_key, true, key + 256);
  ImGui_ImplGLUT_UpdateKeyModifiers();
  (void)x; (void)y; // Unused

  
}

void My_SpecialUpFunc(int key, int x, int y)
{
    //printf("key_up_func %d\n", key);
    ImGuiKey imgui_key = ImGui_ImplGLUT_KeyToImGuiKey(key + 256);
    ImGui_ImplGLUT_AddKeyEvent(imgui_key, false, key + 256);
    ImGui_ImplGLUT_UpdateKeyModifiers();
    (void)x; (void)y; // Unused
}


/*---------------------------------------------
 * Idle event callback function, increase blade and 
 * rotation angles.
 */
void My_idleFunc()
{
  Heli::bladeAnglePlusFive();
  My_DisplayFunc(); /* show the scene again */
}



// Glut has 1 function for characters and one for "special keys". We map the characters in the 0..255 range and the keys above.
static ImGuiKey ImGui_ImplGLUT_KeyToImGuiKey(int key)
{
    switch (key)
    {
        case '\t':                      return ImGuiKey_Tab;
        case 256 + GLUT_KEY_LEFT:       return ImGuiKey_LeftArrow;
        case 256 + GLUT_KEY_RIGHT:      return ImGuiKey_RightArrow;
        case 256 + GLUT_KEY_UP:         return ImGuiKey_UpArrow;
        case 256 + GLUT_KEY_DOWN:       return ImGuiKey_DownArrow;
        case 256 + GLUT_KEY_PAGE_UP:    return ImGuiKey_PageUp;
        case 256 + GLUT_KEY_PAGE_DOWN:  return ImGuiKey_PageDown;
        case 256 + GLUT_KEY_HOME:       return ImGuiKey_Home;
        case 256 + GLUT_KEY_END:        return ImGuiKey_End;
        case 256 + GLUT_KEY_INSERT:     return ImGuiKey_Insert;
        case 127:                       return ImGuiKey_Delete;
        case 8:                         return ImGuiKey_Backspace;
        case ' ':                       return ImGuiKey_Space;
        case 13:                        return ImGuiKey_Enter;
        case 27:                        return ImGuiKey_Escape;
        case 39:                        return ImGuiKey_Apostrophe;
        case 44:                        return ImGuiKey_Comma;
        case 45:                        return ImGuiKey_Minus;
        case 46:                        return ImGuiKey_Period;
        case 47:                        return ImGuiKey_Slash;
        case 59:                        return ImGuiKey_Semicolon;
        case 61:                        return ImGuiKey_Equal;
        case 91:                        return ImGuiKey_LeftBracket;
        case 92:                        return ImGuiKey_Backslash;
        case 93:                        return ImGuiKey_RightBracket;
        case 96:                        return ImGuiKey_GraveAccent;
        //case 0:                         return ImGuiKey_CapsLock;
        //case 0:                         return ImGuiKey_ScrollLock;
        case 256 + 0x006D:              return ImGuiKey_NumLock;
        //case 0:                         return ImGuiKey_PrintScreen;
        //case 0:                         return ImGuiKey_Pause;
        //case '0':                       return ImGuiKey_Keypad0;
        //case '1':                       return ImGuiKey_Keypad1;
        //case '2':                       return ImGuiKey_Keypad2;
        //case '3':                       return ImGuiKey_Keypad3;
        //case '4':                       return ImGuiKey_Keypad4;
        //case '5':                       return ImGuiKey_Keypad5;
        //case '6':                       return ImGuiKey_Keypad6;
        //case '7':                       return ImGuiKey_Keypad7;
        //case '8':                       return ImGuiKey_Keypad8;
        //case '9':                       return ImGuiKey_Keypad9;
        //case 46:                        return ImGuiKey_KeypadDecimal;
        //case 47:                        return ImGuiKey_KeypadDivide;
        case 42:                        return ImGuiKey_KeypadMultiply;
        //case 45:                        return ImGuiKey_KeypadSubtract;
        case 43:                        return ImGuiKey_KeypadAdd;
        //case 13:                        return ImGuiKey_KeypadEnter;
        //case 0:                         return ImGuiKey_KeypadEqual;
        case 256 + 0x0072:              return ImGuiKey_LeftCtrl;
        case 256 + 0x0070:              return ImGuiKey_LeftShift;
        case 256 + 0x0074:              return ImGuiKey_LeftAlt;
        //case 0:                         return ImGuiKey_LeftSuper;
        case 256 + 0x0073:              return ImGuiKey_RightCtrl;
        case 256 + 0x0071:              return ImGuiKey_RightShift;
        case 256 + 0x0075:              return ImGuiKey_RightAlt;
        //case 0:                         return ImGuiKey_RightSuper;
        //case 0:                         return ImGuiKey_Menu;
        case '0':                       return ImGuiKey_0;
        case '1':                       return ImGuiKey_1;
        case '2':                       return ImGuiKey_2;
        case '3':                       return ImGuiKey_3;
        case '4':                       return ImGuiKey_4;
        case '5':                       return ImGuiKey_5;
        case '6':                       return ImGuiKey_6;
        case '7':                       return ImGuiKey_7;
        case '8':                       return ImGuiKey_8;
        case '9':                       return ImGuiKey_9;
        case 'A': case 'a':             return ImGuiKey_A;
        case 'B': case 'b':             return ImGuiKey_B;
        case 'C': case 'c':             return ImGuiKey_C;
        case 'D': case 'd':             return ImGuiKey_D;
        case 'E': case 'e':             return ImGuiKey_E;
        case 'F': case 'f':             return ImGuiKey_F;
        case 'G': case 'g':             return ImGuiKey_G;
        case 'H': case 'h':             return ImGuiKey_H;
        case 'I': case 'i':             return ImGuiKey_I;
        case 'J': case 'j':             return ImGuiKey_J;
        case 'K': case 'k':             return ImGuiKey_K;
        case 'L': case 'l':             return ImGuiKey_L;
        case 'M': case 'm':             return ImGuiKey_M;
        case 'N': case 'n':             return ImGuiKey_N;
        case 'O': case 'o':             return ImGuiKey_O;
        case 'P': case 'p':             return ImGuiKey_P;
        case 'Q': case 'q':             return ImGuiKey_Q;
        case 'R': case 'r':             return ImGuiKey_R;
        case 'S': case 's':             return ImGuiKey_S;
        case 'T': case 't':             return ImGuiKey_T;
        case 'U': case 'u':             return ImGuiKey_U;
        case 'V': case 'v':             return ImGuiKey_V;
        case 'W': case 'w':             return ImGuiKey_W;
        case 'X': case 'x':             return ImGuiKey_X;
        case 'Y': case 'y':             return ImGuiKey_Y;
        case 'Z': case 'z':             return ImGuiKey_Z;
        case 256 + GLUT_KEY_F1:         return ImGuiKey_F1;
        case 256 + GLUT_KEY_F2:         return ImGuiKey_F2;
        case 256 + GLUT_KEY_F3:         return ImGuiKey_F3;
        case 256 + GLUT_KEY_F4:         return ImGuiKey_F4;
        case 256 + GLUT_KEY_F5:         return ImGuiKey_F5;
        case 256 + GLUT_KEY_F6:         return ImGuiKey_F6;
        case 256 + GLUT_KEY_F7:         return ImGuiKey_F7;
        case 256 + GLUT_KEY_F8:         return ImGuiKey_F8;
        case 256 + GLUT_KEY_F9:         return ImGuiKey_F9;
        case 256 + GLUT_KEY_F10:        return ImGuiKey_F10;
        case 256 + GLUT_KEY_F11:        return ImGuiKey_F11;
        case 256 + GLUT_KEY_F12:        return ImGuiKey_F12;
        default:                        return ImGuiKey_None;
    }
}

static void ImGui_ImplGLUT_UpdateKeyModifiers()
{
    ImGuiIO& io = ImGui::GetIO();
    int glut_key_mods = glutGetModifiers();
    io.AddKeyEvent(ImGuiMod_Ctrl, (glut_key_mods & GLUT_ACTIVE_CTRL) != 0);
    io.AddKeyEvent(ImGuiMod_Shift, (glut_key_mods & GLUT_ACTIVE_SHIFT) != 0);
    io.AddKeyEvent(ImGuiMod_Alt, (glut_key_mods & GLUT_ACTIVE_ALT) != 0);
}

static void ImGui_ImplGLUT_AddKeyEvent(ImGuiKey key, bool down, int native_keycode)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, -1); // To support legacy indexing (<1.87 user code)
}










