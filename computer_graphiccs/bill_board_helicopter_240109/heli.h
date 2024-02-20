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
//#include "light.h"
#define LANDING 0
#define HOVER 1
#define FORWARD 2
#define TURN_LEFT 4
#define TURN_RIGHT 5
#define ROTATE 6

float tempX=0.0, tempY=3.0, tempZ;

float mat_body[4]={0.0,0.4,0.0,1.0};
float mat_spec[4]={1.0,1.0,1.0,1.0};
float mat_landing_arm[4]={0.8, 0.8, 0.8,1.0};
float mat_landing_skid[4]={1.0,0.7,0.1,1.0};
float mat_main_joint[4]={0.95, 0.2, 0.2,1.0};
float mat_main_blade[4]={0.1, 0.1, 0.8,1.0};
float mat_main_arm[4]={0.8,0.8,0.8,1.0};
float mat_tail_boom[4]={0.7,0.7,0.7,1.0};
float mat_rudder[4]={0.0,0.4,0.0,1.0};
float mat_tail_arm[4]={0.7, 0.7, 0.7,1.0};
float mat_tail_blade[4]={0.1, 0.1, 0.8,1.0};
float mat_tail_joint[4]={0.95, 0.2, 0.2,1.0};

class Heli
{
private:
  static float x_pos, y_pos, z_pos;
  static float direction;
  static float blade_angle;
  static float rotation_angle;
  static int state;
  static bool isRotating;
  static int turn;
  /*-----Define a unit box--------*/
  /* Vertices of the box */
  static float  points[8][3];
  /* face of box, each face composing of 4 vertices */
  static int  face[6][4];
  /* indices of the box faces */
  static int  cube[6];
  static float normals[6][3];

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
  static void moveOptions();
  static void drawHeli();
  static void takeOff();
  static void landing();
  static void upAndDown(float altitude);
  static void flyForward();
  static void hover();
  static void flyLeft();
  static void flyRight();
  static void rotate();
  static void directionPlusFive();
  static void directionMinusFive();
  static void bladeAnglePlusFive();
  static void reset();
};

float Heli::blade_angle = 0.0;
float Heli::rotation_angle = 0.0;
float Heli::direction = -90.0;
int Heli::state = LANDING ;
bool Heli::isRotating = false;
int Heli::turn = HOVER ;
float Heli::x_pos = 0.0;
float Heli::y_pos = 3.0;

float  Heli::points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, 
                    {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, 
                    {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                    {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};

int  Heli::face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, 
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};

int  Heli::cube[6] = {0, 1, 2, 3, 4, 5};

float Heli::normals[6][3] = {{0.0,  0.0,  -1.0},
                       {0.0,  -1.0, 0.0},
                       {1.0,  0.0,  0.0},
                       {0.0,  0.0,  1.0},
                       {0.0,  1.0,  0.0},
                       {-1.0, 0.0,  0.0}};

GLUquadricObj  *Heli::sphere=NULL;
GLUquadricObj  *Heli::cylind=NULL;

void Heli::draw_cube(){
  /*--------------------------------------------------------
   * Procedure to draw a cube. The geometrical data of the cube
   * are defined above.
   */
  int i;
  for(i=0;i<6;i++){
    glNormal3fv(normals[i]);
    glBegin(GL_POLYGON);  /* Draw the face */
    glVertex3fv(points[face[i][0]]);
    glVertex3fv(points[face[i][1]]);
    glVertex3fv(points[face[i][2]]);
    glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

void Heli::draw_blade(){
  /*-------------------------------------------------------
   * Procedure to draw a polygon as a blade of the rotor.
   */
  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
  glEnd();
}

void Heli::draw_main_body(){
  /* save the main body coord. 
   * Scale up the axes to draw the main body
   * and return to main body coord.
   */

  /* allocate a quadric object, if necessary */
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }

  glColor3f(0.0f, 0.4f, 0.0f);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_body);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  glPushMatrix();
  glScalef(8.0, 4.5, 6.0);
  gluSphere(sphere, 0.5, 24, 24);
  glPopMatrix();
}

void Heli::draw_landing_arm(){
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
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_landing_arm);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
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

void Heli::draw_landing_skid(){
  /* save the main body coord. 
   * Scale up and shift the axes to draw the landing skid
   * and return to main body coord.
   */
  glColor3f(1.0, 0.7, 0.1);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_landing_skid);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
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

void Heli::draw_main_blade(){
  /* allocate a quadric object, if necessary */
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }
  glColor3f(0.95, 0.2, 0.2);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_main_joint);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  /* draw the joint connecting main blades arm and main blades */
  glPushMatrix();
  glScalef(1.0, 0.5, 1.0);
  gluSphere(sphere, 0.5, 24, 24);
  glPopMatrix(); 
  
  /* we have to make the blade rotate */
  if(state){
    glRotatef(Heli::blade_angle, 0.0, 1.0, 0.0); 
  }

  /* save the main blades coord. 
   * rotate axes to draw the main blades
   * and return to main blades coord.
   */
  glColor3f(0.1, 0.1, 0.8);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_main_blade);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
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
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_main_arm);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  glPushMatrix();
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.3, 0.3, 2.0, 12, 3);
  glPopMatrix(); 
}

void Heli::draw_tail_boom(){
  /* scale axes to draw the tail boom */
  glColor3f(0.7, 0.7, 0.7);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_tail_boom);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  glPushMatrix();
  glScalef(8.0, 1.0, 0.5);
  draw_cube();
  glPopMatrix(); 
}

void Heli::draw_rudder(){
  /* scale axes to draw the Rudder */
  glColor3f(0.0, 0.4, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_rudder);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  glPushMatrix();
  glScalef(1.0, 3.0, 0.5);
  draw_cube();
  glPopMatrix(); 
}

void Heli::draw_tail_blade(){
  
  /* scale axes to draw the joint of tail blades*/
  glPushMatrix();
  glScalef(1.0, 1.0, 0.5);
  glColor3f(0.95, 0.2, 0.2);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_tail_joint);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  gluSphere(sphere, 0.5, 24, 24);
  glPopMatrix(); 
  
  /* draw the arm connecting joint and Rudder */
  glColor3f(0.7, 0.7, 0.7);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_tail_arm);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
  glPushMatrix();
  glRotatef(180.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.3, 0.3, 2.5, 12, 3);
  glPopMatrix(); 


  /* we have to make the blade rotate */
  if(state){
    glRotatef(Heli::blade_angle, 0.0, 0.0, 1.0); 
  }
  /* save the tail blades coord. 
   * scale axes to draw the tail blades
   * and return to tail blades coord.
   */
  glColor3f(0.1, 0.1, 0.8);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat_tail_blade);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_spec);
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

}

void Heli::drawHeli(){

  /* the helicopter will lean forward when forwarding*/
  if(state==FORWARD) glRotatef(20.0, 0.0, 0.0, 1.0);

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

void Heli::takeOff(){
  tempY = 5;
  state = HOVER;
}

void Heli::landing(){
  tempY = 3;
  state = LANDING;
}

void Heli::upAndDown(float altitude){
  tempY = altitude;
}

void Heli::flyForward(){
  state = FORWARD;
}

void Heli::hover(){
  state = HOVER;
  turn = HOVER;
}

void Heli::flyLeft(){
  turn = TURN_LEFT;
}

void Heli::flyRight(){
  turn = TURN_RIGHT;
}

void Heli::rotate(){
  isRotating = true;
}

void Heli::directionPlusFive(){
  direction += 0.05;
  if(direction>360.0){
    direction -= 360.0;
  }
}

void Heli::directionMinusFive(){
  direction -= 0.05;
  if(direction<-360.0){
    direction += 360.0;
  }
}

void Heli::reset(){
  direction = -90.0;
  x_pos = 0.0;
  y_pos = 3.0;
  state = LANDING;
  tempX=0.0; 
  tempY=3.0;
  turn = HOVER ;
}

void Heli::bladeAnglePlusFive(){
  blade_angle += 5.0;
  if(blade_angle>360.0){
    blade_angle -= 360.0;
  }
}



void Heli::moveOptions(){
  glRotatef(direction, 0.0, 1.0, 0.0);
  glTranslatef(x_pos, y_pos, 10.0);

  /*this is for rotation*/
  glRotatef(rotation_angle, 0.0, 1.0, 0.0);

  /* to make the heli move forward*/
  if(state==FORWARD){
    x_pos -= 0.01;
    glRotatef(20.0, 0.0, 0.0, 1.0);
  } 
  if(turn == TURN_LEFT){
    directionPlusFive();
  }else if(turn == TURN_RIGHT){
    directionMinusFive();
  }
  if(isRotating){
    rotation_angle +=0.1;
    if(rotation_angle>360.0) {
      rotation_angle = 0;
      isRotating = false;
    }
  }
  
  /*to check the current height */
  if(y_pos<tempY) y_pos += 0.01;
  if(y_pos>tempY) y_pos -= 0.01;

}
