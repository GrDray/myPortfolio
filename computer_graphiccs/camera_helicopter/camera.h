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
    void row(int direction);
    void yaw(int direction);
    void sway(int direction);
    void surge(int direction);
    void heave(int direction);
    
    /* to set the initial position of camera */
    void initialize(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz);
    
    /* pack the glLookAt() and use in display function */ 
    void lookAt(); 
};



void Camera::lookAt(){
  gluLookAt(E[0], E[1], E[2], F[0], F[1], F[2], Vup[0], Vup[1], Vup[2]);
  glGetFloatv(GL_MODELVIEW_MATRIX, vmtx);
}

void Camera::initialize(float ex, float ey, float ez, float fx, float fy, float fz, float vx, float vy, float vz){
  E[0]=ex; E[1]=ey; E[2]=ez;
  F[0]=fx; F[1]=fy; F[2]=fz;
  Vup[0]=vx; Vup[1]=vy; Vup[2]=vz;
}

void Camera::sway(int direction){
  float rate = (float)direction*0.05;
  E[0]+=rate*vmtx[0];
  F[0]+=rate*vmtx[0];
  E[1]+=rate*vmtx[4];
  F[1]+=rate*vmtx[4];
  E[2]+=rate*vmtx[8];
  F[2]+=rate*vmtx[8];
}

void Camera::surge(int direction){
  float rate = (float)direction*0.05;
  E[0]+=rate*vmtx[2];
  F[0]+=rate*vmtx[2];
  E[1]+=rate*vmtx[6];
  F[1]+=rate*vmtx[6];
  E[2]+=rate*vmtx[10];
  F[2]+=rate*vmtx[10];
}

void Camera::heave(int direction){
  float rate = (float)direction*0.05;
  E[0]+=rate*vmtx[1];
  F[0]+=rate*vmtx[1];
  E[1]+=rate*vmtx[5];
  F[1]+=rate*vmtx[5];
  E[2]+=rate*vmtx[9];
  F[2]+=rate*vmtx[9];
}

void Camera::row(int direction){
  float rate = direction*0.05;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
    glRotatef(rate, vmtx[2], vmtx[6], vmtx[10]);
  glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
  glPopMatrix();
  rotateViewUp();
}

void Camera::yaw(int direction){
  float rate = direction*0.05;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
    glTranslatef(E[0], E[1], E[2]);
    glRotatef(rate, vmtx[1], vmtx[5], vmtx[9]);
    glTranslatef(-E[0], -E[1], -E[2]);
  glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
  glPopMatrix();
  rotateViewUp();
  rotateFocus();
}

void Camera::pitch(int direction){
  float rate = direction*0.05;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
    glTranslatef(E[0], E[1], E[2]);
    glRotatef(rate, vmtx[0], vmtx[4], vmtx[8]);
    glTranslatef(-E[0], -E[1], -E[2]);
  glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
  glPopMatrix();
  rotateViewUp();
  rotateFocus();
}

void Camera::rotateViewUp(){
  float tempVup[4] = {Vup[0], Vup[1], Vup[2], Vup[3]};
  for (int col = 0; col < 4; col++) {
    for (int inner = 0; inner < 4; inner++) {
      Vup[col] += tempVup[inner] * mtx[inner+4*col];
    }
  }
}

void Camera::rotateFocus(){
  float tempF[4] = {F[0], F[1], F[2], F[3]};
  for (int col = 0; col < 4; col++) {
    for (int inner = 0; inner < 4; inner++) {
      F[col] += tempF[inner] * mtx[inner+4*col];
    }
  }
}

