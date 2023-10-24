/*---------------------------------------------------------------------------------
 * This file is to define the process of drawing a helicopter.
 *
 * by C. J. Tsao, Nov.  2023
 */

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#pragma once


class MyHeli
{
private:
  /*-----Define a unit box--------*/
  /* Vertices of the box */
  static float  points[8][3];
  /* face of box, each face composing of 4 vertices */
  static int  face[6][4];
  /* indices of the box faces */
  static int  cube[6];

  /*-----Define GLU quadric objects, a sphere and a cylinder----*/
  static GLUquadricObj  *sphere;
  static GLUquadricObj  *cylind;

  static void draw_cube();

  static void draw_blade();

  static void draw_main_body();

  static void draw_landing_arm();

  static void draw_landing_skid();

  static void draw_main_blade();

  static void draw_tail_boom();

  static void draw_rudder();

  static void draw_tail_blade();

public:
  static void drawHeli();
};


float  MyHeli::points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, 
                    {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, 
                    {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                    {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};

int  MyHeli::face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, 
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};

int  MyHeli::cube[6] = {0, 1, 2, 3, 4, 5};

GLUquadricObj  *MyHeli::sphere=NULL;
GLUquadricObj  *MyHeli::cylind=NULL;

void MyHeli::draw_cube(){
  /*--------------------------------------------------------
   * Procedure to draw a cube. The geometrical data of the cube
   * are defined above.
   */
  int i;
  for(i=0;i<6;i++){
    glBegin(GL_POLYGON);  /* Draw the face */
    glVertex3fv(points[face[i][0]]);
    glVertex3fv(points[face[i][1]]);
    glVertex3fv(points[face[i][2]]);
    glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

void MyHeli::draw_blade(){
  /*-------------------------------------------------------
   * Procedure to draw a polygon as a blade of the rotor.
   */
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
  glEnd();
}

void MyHeli::draw_main_body(){
  /* save the main body coord. 
   * Scale up the axes to draw the main body
   * and return to main body coord.
   */

  /* allocate a quadric object, if necessary */
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
  }

  glColor3f(0.0f, 0.4f, 0.0f);
  glPushMatrix();
  glScalef(8.0, 4.5, 6.0);
  gluSphere(sphere, 0.5, 24, 24);
  glPopMatrix();
}

void MyHeli::draw_landing_arm(){
    /* allocate a quadric object, if necessary */
  if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }

  /* save the main body coord. 
   * shift and rotate the axes to draw the arms
   * and return to main body coord.
   */
  glColor3f(0.8f, 0.8f, 0.8f);
  glPushMatrix(); // draw the front-left arm
  glTranslatef(-1.5, -1.5, 1.5);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.2, 0.2, 1.0, 12, 3);        
  glPopMatrix();
  glPushMatrix(); // draw the back-left arm
  glTranslatef(1.5, -1.5, 1.5);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.2, 0.2, 1.0, 12, 3);        
  glPopMatrix();
  glPushMatrix();  // draw the front-right arm
  glTranslatef(-1.5, -1.5, -1.5);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.2, 0.2, 1.0, 12, 3);        
  glPopMatrix();
  glPushMatrix();  // draw the back-right arm
  glTranslatef(1.5, -1.5, -1.5);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.2, 0.2, 1.0, 12, 3);        
  glPopMatrix();
}

void MyHeli::draw_landing_skid(){
  /* save the main body coord. 
   * Scale up and shift the axes to draw the landing skid
   * and return to main body coord.
   */
  glColor3f(1.0, 0.7, 0.1);
  glPushMatrix();  // draw the left landing skid
  glTranslatef(0.0, -2.75, 1.5);
  glScalef(8.0, 0.5, 1.0);
  draw_cube();
  glPopMatrix();
  glPushMatrix();  // draw the right landing skid
  glTranslatef(0.0, -2.75, -1.5);
  glScalef(8.0, 0.5, 1.0);
  draw_cube();
  glPopMatrix();
}

void MyHeli::draw_main_blade(){
  /* allocate a quadric object, if necessary */
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
  }
  glColor3f(0.95, 0.2, 0.2);
  /* draw the joint connecting main blades arm and main blades */
  glPushMatrix();
  glScalef(1.0, 0.5, 1.0);
  gluSphere(sphere, 0.5, 24, 24);
  glPopMatrix(); 
  
  /* save the main blades coord. 
   * rotate axes to draw the main blades
   * and return to main blades coord.
   */
  glColor3f(0.1, 0.1, 0.8);
  glPushMatrix();
  glRotatef(90.0, 1.0, 0.0, 0.0);
  draw_blade();  // draw the main blades 1st
  glRotatef(120.0, 0.0, 0.0, 1.0);
  draw_blade();  // draw the main blades 2nd
  glRotatef(120.0, 0.0, 0.0, 1.0);
  draw_blade();  // draw the main blades 3rd
  glPopMatrix(); 


  /* draw the arm connecting main blades joint and main body */
  glColor3f(0.8, 0.8, 0.8);
  glPushMatrix();
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.3, 0.3, 2.0, 12, 3);
  glPopMatrix(); 
}

void MyHeli::draw_tail_boom(){
  /* scale axes to draw the tail boom */
  glColor3f(0.7, 0.7, 0.7);
  glPushMatrix();
  glScalef(8.0, 1.0, 0.5);
  draw_cube();
  glPopMatrix(); 
}

void MyHeli::draw_rudder(){
  /* scale axes to draw the Rudder */
  glColor3f(0.0, 0.4, 0.0);
  glPushMatrix();
  glScalef(1.0, 3.0, 0.5);
  draw_cube();
  glPopMatrix(); 
}

void MyHeli::draw_tail_blade(){
  /* save the tail blades coord. 
   * scale axes to draw the tail blades
   * and return to tail blades coord.
   */
  glColor3f(0.1, 0.1, 0.8);
  glPushMatrix();  // draw the 1st tail blades
  glScalef(0.25, 0.25, 0.25);
  draw_blade();
  glPopMatrix(); 
  glPushMatrix();  // draw the 2nd tail blades
  glRotatef(120.0, 0.0, 0.0, 1.0);
  glScalef(0.25, 0.25, 0.25);
  draw_blade();
  glPopMatrix();
  glPushMatrix();  // draw the 3rd tail blades
  glRotatef(-120.0, 0.0, 0.0, 1.0);
  glScalef(0.25, 0.25, 0.25);
  draw_blade();
  glPopMatrix(); 

  /* scale axes to draw the joint of tail blades*/
  glPushMatrix();
  glScalef(1.0, 1.0, 0.5);
  glColor3f(0.95, 0.2, 0.2);
  gluSphere(sphere, 0.5, 24, 24);
  glPopMatrix(); 
  
  /* draw the arm connecting joint and Rudder */
  glColor3f(0.7, 0.7, 0.7);
  glPushMatrix();
  glRotatef(180.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.3, 0.3, 2.5, 12, 3);
  glPopMatrix(); 

}

void MyHeli::drawHeli(){
  
  draw_main_body();
  
  draw_landing_arm();

  draw_landing_skid();

  /* save the main body coord. 
   * and now we enter the coord. of the center of main blades.
   */
  glPushMatrix();
  glTranslatef(0.0, 3.5, 0.0);

  draw_main_blade();

  /*now we return to the coord. of main body*/
  glPopMatrix();

  /* enter the coord. of tail boom part*/
  glTranslatef(5.5, 0.0, 0.0);

  draw_tail_boom();

  /* enter the coord. of Rudder part*/
  glTranslatef(4.5, 0.0, 0.0);

  draw_rudder();

  /* enter the coord. of tail blades part*/
  glTranslatef(0.0, 0.0, 2.5);

  draw_tail_blade();

}






