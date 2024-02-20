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
#include "camera.h"
#include "scene.h"
#include "light.h"

#define PROJECT_ORTHO_X 1
#define PROJECT_ORTHO_Y 2
#define PROJECT_ORTHO_Z 3
#define PROJECT_PERSPECTIVE 4
#define PROJECT_ALL 5

static int g_Time = 0;          // Current time, in milliseconds
#endif // #ifndef IMGUI_DISABLE

/*-----Define window size----*/
int width=1280, height=720;


Camera C1, C2, C3, C4;

// Here I temporarily store the view volume parameters
float left=-30.0, right=30.0, bottom=-30.0, top=30.0;

void zoomIn(){
  float a=0.9;
  float uCenter = (left+right)/2;
  float vCenter = (bottom+top)/2;
  left -= uCenter;
  right -= uCenter;
  bottom -= vCenter;
  top -= vCenter;
  left *= a;
  right *= a;
  bottom *= a;
  top *= a;
  left -= uCenter;
  right -= uCenter;
  bottom -= vCenter;
  top -= vCenter;
}

void zoomOut(){
  float a=1.1;
  float uCenter = (left+right)/2;
  float vCenter = (bottom+top)/2;
  left -= uCenter;
  right -= uCenter;
  bottom -= vCenter;
  top -= vCenter;
  left *= a;
  right *= a;
  bottom *= a;
  top *= a;
  left -= uCenter;
  right -= uCenter;
  bottom -= vCenter;
  top -= vCenter;
}

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

void projectionMethods(int method);

void CheckboxCallback(bool* option, bool* other1, bool* other2, bool* other3, bool* other4) {
    if (*option) {
        *other1 = false;
        *other2 = false;
        *other3 = false;
        *other4 = false;
    }
}

void CheckboxCallback3(bool* option, bool* other1, bool* other2) {
    if (*option) {
        *other1 = false;
        *other2 = false;
    }
}


class HeliGUI {
  public:
    // Our state
    static ImVec4 point_color;
    static ImVec4 sun_color;
    static ImVec4 fog_color_gui;
    static bool show_demo_window;
    static bool show_another_window;
    static int projectMethod;

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
      Scene::textureInitialize();
      Scene::fogInitialize();
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

    static void setCamera1(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz){
      C1.initialize(ex, ey, ez, fx, fy, fz, vx, vy, vz);
    }
    static void setCamera2(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz){
      C2.initialize(ex, ey, ez, fx, fy, fz, vx, vy, vz);
    }
    static void setCamera3(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz){
      C3.initialize(ex, ey, ez, fx, fy, fz, vx, vy, vz);
    }
    static void setCamera4(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz){
      C4.initialize(ex, ey, ez, fx, fy, fz, vx, vy, vz);
    }
    
    static void drawScene(){
      //Scene::drawXYZaxisWithRGB();
      Scene::drawOrigin();
      glPushMatrix();
      glTranslatef(20.0, 20.0, -20.0);
      Light::drawEmissionSphere();
      glPopMatrix();
      Scene::drawBuilding();
      Scene::drawGroundSky();
      Scene::drawBillBoard();
      Heli::moveOptions();
      glPushMatrix();
      glTranslatef(0.0, -2.0, 0.0);
      Light::spotLight();
      glPopMatrix();
      Heli::drawHeli();
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
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
      }
/****************************************************************************************
 * window to switch between different projection methods
*/
      {
        static float f = 0.0f;
        static bool orthoX=0, orthoY=0, orthoZ=0;
        static bool perspective=1, showAll=0;

        ImGui::Begin("projection method");                  

        ImGui::Text("Select a projection method.");               
        ImGui::Checkbox("Ortho x-axis", &orthoX); 
        CheckboxCallback(&orthoX,&orthoY,&orthoZ,&perspective,&showAll);
        ImGui::Checkbox("Ortho y-axis", &orthoY);
        CheckboxCallback(&orthoY,&orthoX,&orthoZ,&perspective,&showAll);
        ImGui::Checkbox("Ortho z-axis", &orthoZ);
        CheckboxCallback(&orthoZ,&orthoX,&orthoY,&perspective,&showAll);
        ImGui::Checkbox("Perspective", &perspective);
        CheckboxCallback(&perspective,&orthoX,&orthoY,&orthoZ,&showAll);
        ImGui::Checkbox("Show all", &showAll);
        CheckboxCallback(&showAll,&orthoX,&orthoY,&orthoZ,&perspective);

        if(orthoX){
          HeliGUI::projectMethod = PROJECT_ORTHO_X;
        }else if(orthoY){
          HeliGUI::projectMethod = PROJECT_ORTHO_Y;
        }else if(orthoZ){
          HeliGUI::projectMethod = PROJECT_ORTHO_Z;
        }else if(perspective){
          HeliGUI::projectMethod = PROJECT_PERSPECTIVE;
        }else if(showAll){
          HeliGUI::projectMethod = PROJECT_ALL;
        }
        

        if (ImGui::Button("Zoom-in")) zoomIn();
        ImGui::SameLine();
        if (ImGui::Button("Zoom-out")) zoomOut();
        ImGui::End();
      }

      /**********************
       * allow point light to change color/intensity
       * allow sun light to change direction/color/intensity
       */
      {
        ImGuiIO& io = ImGui::GetIO();
        static float slider_point_intensity = 10.0;
        static float slider_sun_intensity = 10.0;
        static float slider_spot_intensity = 10.0;
        static float slider_spot_cutoff = 91.0;
        static float slider_sun_Xdirection = 1.0;
        static float slider_sun_Ydirection = -1.0;
        static float slider_sun_Zdirection = 1.0;
        static float slider_spot_Xdirection = 0.0;
        static float slider_spot_Ydirection = -1.0;
        static float slider_spot_Zdirection = 0.0;
        static int point_light_on = 1;
        static int sun_light_on = 1;
        static int spot_light_on = 1;
        static bool fog_on = 0;
        static bool fog_mode_linear = 1;
        static bool fog_mode_exp = 0;
        static bool fog_mode_exp2 = 0;
        static float fog_density = 0.15;

        ImGui::Begin("Light Control");
        ImGui::Text("Adjust the point light."); 
        ImGui::SameLine();
        if (ImGui::Button("Turn point light On/Off")){
          if(point_light_on){
            Light::turnLight(LIGHT_1, LIGHT_OFF);
            point_light_on = 0;
          }else{
            Light::turnLight(LIGHT_1, LIGHT_ON);
            point_light_on = 1;
          }
        }
        ImGui::SliderFloat("Intensity of point light", &slider_point_intensity, 1.0, 10.0);
        ImGui::ColorEdit3("Color of point light", (float*)&point_color);
        Light::light1_diffuse[0] = point_color.x * point_color.w * 0.1*slider_point_intensity;
        Light::light1_diffuse[1] = point_color.y * point_color.w * 0.1*slider_point_intensity;
        Light::light1_diffuse[2] = point_color.z * point_color.w * 0.1*slider_point_intensity;
        
        ImGui::Text("-----------------------------------------------------------------------------"); 

        ImGui::Text("Adjust the sun light."); 
        ImGui::SameLine();
        if (ImGui::Button("Turn sun light On/Off")){
          if(sun_light_on){
            Light::turnLight(LIGHT_0, LIGHT_OFF);
            sun_light_on = 0;
          }else{
            Light::turnLight(LIGHT_0, LIGHT_ON);
            sun_light_on = 1;
          }
        }
        ImGui::SliderFloat("Intensity of sun light", &slider_sun_intensity, 1.0, 10.0);
        ImGui::ColorEdit3("Color of sun light", (float*)&sun_color);
        Light::light0_diffuse[0] = sun_color.x * sun_color.w * 0.1*slider_sun_intensity;
        Light::light0_diffuse[1] = sun_color.y * sun_color.w * 0.1*slider_sun_intensity;
        Light::light0_diffuse[2] = sun_color.z * sun_color.w * 0.1*slider_sun_intensity;
        ImGui::SliderFloat("X direction of sun light", &slider_sun_Xdirection, -1.0, 1.0);
        ImGui::SliderFloat("Y direction of sun light", &slider_sun_Ydirection, -1.0, 1.0);
        ImGui::SliderFloat("Z direction of sun light", &slider_sun_Zdirection, -1.0, 1.0);
        Light::light0_dir[0] = slider_sun_Xdirection;
        Light::light0_dir[1] = slider_sun_Ydirection;
        Light::light0_dir[2] = slider_sun_Zdirection;
        
        ImGui::Text("-----------------------------------------------------------------------------"); 

        ImGui::Text("Adjust the spot light."); 
        ImGui::SameLine();
        if (ImGui::Button("Turn spot light On/Off")){
          if(spot_light_on){
            fprintf(stderr, "spot off");
            Light::turnLight(LIGHT_2, LIGHT_OFF);
            spot_light_on = 0;
          }else{
            fprintf(stderr, "spot on");
            Light::turnLight(LIGHT_2, LIGHT_ON);
            spot_light_on = 1;
          }
        }
        ImGui::SliderFloat("Intensity of spot light", &slider_spot_intensity, 1.0, 10.0);
        Light::light2_diffuse[0] *= 0.1*slider_sun_intensity;
        Light::light2_diffuse[1] *= 0.1*slider_sun_intensity;
        Light::light2_diffuse[2] *= 0.1*slider_sun_intensity;
        ImGui::SliderFloat("Cutoff angle of spot light", &slider_spot_cutoff, 0.0, 180.0);
        Light::light2_cutoff_angle = slider_spot_cutoff;
        ImGui::SliderFloat("X direction of spot light", &slider_spot_Xdirection, -1.0, 1.0);
        ImGui::SliderFloat("Y direction of spot light", &slider_spot_Ydirection, -1.0, 1.0);
        ImGui::SliderFloat("Z direction of spot light", &slider_spot_Zdirection, -1.0, 1.0);
        Light::spot_light_direction[0] = slider_spot_Xdirection;
        Light::spot_light_direction[1] = slider_spot_Ydirection;
        Light::spot_light_direction[2] = slider_spot_Zdirection;

        ImGui::Text("-----------------------------------------------------------------------------"); 

        ImGui::Checkbox("Fog On/Off", &fog_on); 
        Scene::fogTurn(fog_on);

        ImGui::Checkbox("Linear", &fog_mode_linear); 
        CheckboxCallback3(&fog_mode_linear,&fog_mode_exp,&fog_mode_exp2);
        ImGui::SameLine();
        ImGui::Checkbox("EXP", &fog_mode_exp);
        CheckboxCallback3(&fog_mode_exp,&fog_mode_linear,&fog_mode_exp2);
        ImGui::SameLine();
        ImGui::Checkbox("EXP2", &fog_mode_exp2);
        CheckboxCallback3(&fog_mode_exp2,&fog_mode_linear,&fog_mode_exp);
        
        if(fog_mode_linear){
          Scene::fogMode(1);
        }else if(fog_mode_exp){
          Scene::fogMode(2);
        }else if(fog_mode_exp2){
          Scene::fogMode(3);
        }

        ImGui::SliderFloat("Density of fog", &fog_density, 0.0, 1.0);
        Scene::fogDensity(fog_density);

        ImGui::ColorEdit3("Color of fog", (float*)&fog_color_gui);
        Scene::fogColor(fog_color_gui.x * fog_color_gui.w, fog_color_gui.y * fog_color_gui.w, fog_color_gui.z * fog_color_gui.w);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
      }
    }


  private:
};


ImVec4 HeliGUI::point_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 HeliGUI::sun_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 HeliGUI::fog_color_gui = ImVec4(0.15, 0.20, 0.20, 0.50);
bool HeliGUI::show_demo_window = false;
bool HeliGUI::show_another_window = false;
int HeliGUI::projectMethod = PROJECT_ALL;


/*********************************************
 * Here I implement those functions
 *
 *********************************************
 */


/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * clear the window
 */
void My_DisplayFunc(void)
{
//std::cerr << "display called\n" << std::endl;
  ImGuiIO& io = HeliGUI::GetFrameIO();
  if (HeliGUI::show_demo_window) ImGui::ShowDemoWindow(&HeliGUI::show_demo_window);
  HeliGUI::CreatePropertyWindow();
  HeliGUI::Render();
  glClearColor (0.2, 0.2, 0.2, 0.0);
  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  projectionMethods(HeliGUI::projectMethod);

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
  glOrtho(left, right, bottom*(float)h/(float)w, top*(float)h/(float)w, -100, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
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

  //move the camera
  //C1.sway(MINUS_DIRECTION);
  switch (key)
  {
  case 'a':
    C1.sway(MINUS_DIRECTION);
    C2.sway(MINUS_DIRECTION);
    C3.sway(MINUS_DIRECTION);
    C4.sway(MINUS_DIRECTION);
    break;
  case 'd':
    C1.sway(PLUS_DIRECTION);
    C2.sway(PLUS_DIRECTION);
    C3.sway(PLUS_DIRECTION);
    C4.sway(PLUS_DIRECTION);
    break;
  case 'w':
    C1.surge(MINUS_DIRECTION);
    C2.surge(MINUS_DIRECTION);
    C3.surge(MINUS_DIRECTION);
    C4.surge(MINUS_DIRECTION);
    break;
  case 's':
    C1.surge(PLUS_DIRECTION);
    C2.surge(PLUS_DIRECTION);
    C3.surge(PLUS_DIRECTION);
    C4.surge(PLUS_DIRECTION);
    break;
  case 'r':
    C1.heave(PLUS_DIRECTION);
    C2.heave(PLUS_DIRECTION);
    C3.heave(PLUS_DIRECTION);
    C4.heave(PLUS_DIRECTION);
    break;
  case 'f':
    C1.heave(MINUS_DIRECTION);
    C2.heave(MINUS_DIRECTION);
    C3.heave(MINUS_DIRECTION);
    C4.heave(MINUS_DIRECTION);
    break;
  case 'g':
    C1.yaw(PLUS_DIRECTION);
    C2.yaw(PLUS_DIRECTION);
    C3.yaw(PLUS_DIRECTION);
    C4.yaw(PLUS_DIRECTION);
    break;
  case 'j':
    C1.yaw(MINUS_DIRECTION);
    C2.yaw(MINUS_DIRECTION);
    C3.yaw(MINUS_DIRECTION);
    C4.yaw(MINUS_DIRECTION);
    break;
  case 'h':
    C1.pitch(MINUS_DIRECTION);
    C2.pitch(MINUS_DIRECTION);
    C3.pitch(MINUS_DIRECTION);
    C4.pitch(MINUS_DIRECTION);
    break;
  case 'y':
    C1.pitch(PLUS_DIRECTION);
    C2.pitch(PLUS_DIRECTION);
    C3.pitch(PLUS_DIRECTION);
    C4.pitch(PLUS_DIRECTION);
    break;
  case 't':
    C1.roll(PLUS_DIRECTION);
    C2.roll(PLUS_DIRECTION);
    C3.roll(PLUS_DIRECTION);
    C4.roll(PLUS_DIRECTION);
    break;
  case 'u':
    C1.roll(MINUS_DIRECTION);
    C2.roll(MINUS_DIRECTION);
    C3.roll(MINUS_DIRECTION);
    C4.roll(MINUS_DIRECTION);
    break;
  
  default:
    break;
  }

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



void projectionMethods(int method){
  switch (method)
  {
  case PROJECT_ORTHO_X:
    /* first scene ortho along x-axis*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C1.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, -100, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
    HeliGUI::drawScene();
    break;
  case PROJECT_ORTHO_Y:
    /* 2nd scene ortho along y-axis*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C2.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, -100, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
    HeliGUI::drawScene();
    break;
  case PROJECT_ORTHO_Z:
    /* 3rd scene ortho along z-axis*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C3.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, -100, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
    HeliGUI::drawScene();
    break;
  case PROJECT_PERSPECTIVE:
    /* 4th scene perspective projection*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width, height);///////////////
    C4.lookAt();
    Scene::computeBillBoardAxes();
    Light::initialize();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90.0, (float)height/(float)width, 10.0, 50.0);
    glFrustum(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, 20, 100.0);
    glMatrixMode(GL_MODELVIEW);
    //glViewport(0, 0, width, height);
    HeliGUI::drawScene();
    break;
  case PROJECT_ALL:
    /* first scene ortho along x-axis*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C1.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, -100, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 360, width/2, height/2);
    HeliGUI::drawScene();
    /* 2nd scene ortho along y-axis*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C2.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, -100, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width/2, height/2);
    HeliGUI::drawScene();
    /* 3rd scene ortho along z-axis*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C3.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, -100, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(640, 360, width/2, height/2);
    HeliGUI::drawScene();
    /* 4th scene perspective projection*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    C4.lookAt();
    Scene::computeBillBoardAxes();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90.0, (float)height/(float)width, 10.0, 50.0);
    glFrustum(left, right, bottom*(float)height/(float)width, top*(float)height/(float)width, 20.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(640, 0, width/2, height/2);
    HeliGUI::drawScene();
    break;
  
  default:
    break;
  }
}








