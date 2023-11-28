#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#pragma once

class Scene
{
private:
  /*-----Define GLU quadric objects, a sphere and a cylinder----*/
  static GLUquadricObj  *sphere;
  static GLUquadricObj  *cylind;
public:
  static void drawBuilding();
  static void drawXYZaxisWithRGB();
  static void drawOrigin();
};

GLUquadricObj  *Scene::sphere=NULL;
GLUquadricObj  *Scene::cylind=NULL;

void Scene::drawBuilding(){
  if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }
  glPushMatrix(); 
  glColor4f(0.0, 0.6, 1.0, 0.5);
  glTranslatef(10.0, 0.0, -15.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 5.0, 5.0, 15.0, 12, 3);        
  glPopMatrix();
}

void Scene::drawXYZaxisWithRGB(){
  if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }
  // draw Z axis with blue
  glPushMatrix(); 
  glColor4f(0.0, 0.0, 1.0, 1.0);
  glTranslatef(0.0, 0.0, -40.0);
  gluCylinder(cylind, 0.5, 0.5, 80.0, 12, 3);
  glPopMatrix();
  // draw Y axis with green
  glPushMatrix(); 
  glColor4f(0.0, 1.0, 0.0, 1.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glTranslatef(0.0, 0.0, -40.0);
  gluCylinder(cylind, 0.5, 0.5, 80.0, 12, 3);
  glPopMatrix();

  // draw X axis with red
  glPushMatrix(); 
  glColor4f(1.0, 0.0, 0.0, 1.0);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 0.0, -40.0);
  gluCylinder(cylind, 0.5, 0.5, 80.0, 12, 3);
  glPopMatrix();
}

void Scene::drawOrigin(){
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
  }
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
  gluSphere(sphere, 1.0, 24, 24);
  glPopMatrix();
}



