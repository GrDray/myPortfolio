#pragma once
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>

#define PLUS_DIRECTION 1
#define MINUS_DIRECTION -1

class Camera
{
  private:
    float E[4]={0,0,0,1}; // Eye
    float F[4]={0,0,0,1}; // Focus
    float Vup[4]={0,0,0,0}; // View up
    float vmtx[16];  // store model view matrix to get u, v, w
    float mtx[16];  // store the transform matrix and multiply F and Vup

    void rotateViewUp();
    void rotateFocus();
  
  public:
    /* six DoF 
     * the direction parameter indicates the direction of the six motion
     * use MACRO {PLUS_DIRECTION} or {MINUS_DIRECTION} 
     */
    void pitch(int direction);
    void roll(int direction);
    void yaw(int direction);
    void sway(int direction);
    void surge(int direction);
    void heave(int direction);
    
    /* to set the initial position of camera */
    void initialize(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz);
    
    /* pack the glLookAt() and use in display function */ 
    void lookAt(); 
};

