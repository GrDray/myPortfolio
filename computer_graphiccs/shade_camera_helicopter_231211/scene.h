#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#pragma once

float scene_mat_building[4]={0.0, 0.6, 1.0,1.0};
float scene_mat_spec[4]={1.0,1.0,1.0,1.0};
float scene_mat_no_spec[4]={0.0,0.0,0.0,1.0};
float scene_mat_ground[4]={0.8, 0.9, 0.9,1.0};
float scene_mat_no_emission[4]={0.0,0.0,0.0,1.0};

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
  static void drawGround();
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
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,scene_mat_building);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,8);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,scene_mat_spec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,scene_mat_no_emission);
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

void Scene::drawGround(){
  glColor4f(0.2, 0.5, 0.5, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,scene_mat_ground);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,scene_mat_no_spec);
  glBegin(GL_POLYGON);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-80.0, 0, -80.0);
    glVertex3f(-80.0, 0, 80.0);
    glVertex3f(80.0, 0, 80.0);
    glVertex3f(80.0, 0, -80.0);
  glEnd();
  glFlush();

}

