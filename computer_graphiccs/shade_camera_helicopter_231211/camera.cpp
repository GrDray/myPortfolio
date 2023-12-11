#pragma once
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>

#include "camera.h"



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
  float rate = (float)direction*0.1;
  E[0]+=rate*vmtx[0];
  F[0]+=rate*vmtx[0];
  E[1]+=rate*vmtx[4];
  F[1]+=rate*vmtx[4];
  E[2]+=rate*vmtx[8];
  F[2]+=rate*vmtx[8];
}

void Camera::surge(int direction){
  float rate = (float)direction*0.1;
  E[0]+=rate*vmtx[2];
  F[0]+=rate*vmtx[2];
  E[1]+=rate*vmtx[6];
  F[1]+=rate*vmtx[6];
  E[2]+=rate*vmtx[10];
  F[2]+=rate*vmtx[10];
}

void Camera::heave(int direction){
  float rate = (float)direction*0.1;
  E[0]+=rate*vmtx[1];
  F[0]+=rate*vmtx[1];
  E[1]+=rate*vmtx[5];
  F[1]+=rate*vmtx[5];
  E[2]+=rate*vmtx[9];
  F[2]+=rate*vmtx[9];
}

void Camera::roll(int direction){
  float rate = direction*1.0;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
    glRotatef(rate, vmtx[2], vmtx[6], vmtx[10]);
  glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
  glPopMatrix();
  rotateViewUp();
}

void Camera::yaw(int direction){
  float rate = direction*1.0;
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
  float rate = direction*1.0;
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
    float sum = 0.0;
    for (int inner = 0; inner < 4; inner++) {
      sum += tempVup[inner] * mtx[4*inner+col];
    }
    Vup[col] = sum;
    //fprintf(stderr, "Vup %d is %f\n",col, Vup[col] );
  }
}

void Camera::rotateFocus(){
  float tempF[4] = {F[0], F[1], F[2], F[3]};
  for (int col = 0; col < 4; col++) {
    float sum = 0.0;
    for (int inner = 0; inner < 4; inner++) {
      sum += tempF[inner] * mtx[4*inner+col];
    }
    F[col] = sum;
    //fprintf(stderr, "F %d is %f\n",col, F[col] );
  }
}

