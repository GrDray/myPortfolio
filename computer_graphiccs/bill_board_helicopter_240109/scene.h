#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#pragma once

#include "bmptype.h"

#define TSIZE 64 /* define texture dimension */

float scene_mat_building[4]={0.0, 0.6, 1.0,1.0};
float scene_mat_spec[4]={1.0,1.0,1.0,1.0};
float scene_mat_no_spec[4]={0.0,0.0,0.0,1.0};
float scene_mat_ground[4]={0.8, 0.9, 0.9,1.0};
float scene_mat_no_emission[4]={0.0,0.0,0.0,1.0};

/*-----Create image space for textures -----*/
unsigned char texture_ground[TSIZE][TSIZE][4];    /* ground textures */
unsigned char texture_brick[TSIZE][TSIZE][4];      /* brick wall textures */
unsigned char texture_tree[TSIZE][TSIZE][4];      /* tree textures */
unsigned char texture_cloud[TSIZE][TSIZE][4];      /* tree textures */
unsigned char texture_moon[TSIZE][TSIZE][4];      /* tree textures */
unsigned int textName[6];                  /* declare two texture maps*/

char CH_OIL[20] = "../../CHOIL2.bmp";

/*---- the axes of billboard ----*/
float a[3], b[3];

float mtx[16]; /* the modelview matrix */

/*---Define fog color----*/
float fog_color[]={0.15, 0.20, 0.20, 0.50};

/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_texture_ground() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            c = (((i & 0x4) == 0) ^ ((j & 0x4) == 0)) * 255;
            texture_ground[i][j][0] = c;
            texture_ground[i][j][1] = c;
            texture_ground[i][j][2] = c;
            texture_ground[i][j][3] = 255;
        }
}

/*----------------------------------------------------------
 * Procedure to maKe a brick wall texture map
 */
void make_texture_brick() {
    int y, x, i;

    for (y = 0; y < 12; y++) { /*----Create the first row of bricks----*/
        for (x = 0; x < 2; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++) {
            texture_brick[y][x][0] = 200;
            texture_brick[y][x][1] = 0;
            texture_brick[y][x][2] = 0;
            texture_brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++) {
            texture_brick[y][x][0] = 200;
            texture_brick[y][x][1] = 0;
            texture_brick[y][x][2] = 0;
            texture_brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++) /*--Create the cement between two breick layers */
        for (x = 0; x < 64; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++) { /*--Create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++) {
            texture_brick[y][x][0] = 200;
            texture_brick[y][x][1] = 0;
            texture_brick[y][x][2] = 0;
            texture_brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++) {
            texture_brick[y][x][0] = 200;
            texture_brick[y][x][1] = 0;
            texture_brick[y][x][2] = 0;
            texture_brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++) {
            texture_brick[y][x][0] = 200;
            texture_brick[y][x][1] = 0;
            texture_brick[y][x][2] = 0;
            texture_brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---Add another layer of cement----*/
        for (x = 0; x < 64; x++) {
            texture_brick[y][x][0] = 140;
            texture_brick[y][x][1] = 140;
            texture_brick[y][x][2] = 140;
            texture_brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                texture_brick[y][x][i] = texture_brick[y - 32][x][i];

  fprintf(stderr, "brick \n");
}


/*--------------------------------------------------------
 * Procedure to draw a billboard, center=[x,z], width=w,
 * height = h;
 */
void draw_billboard(float x, float z, float w, float h) {
    float v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0];
    v0[1] = 0.0;
    v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0];
    v1[1] = 0.0;
    v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0];
    v2[1] = h;
    v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0];
    v3[1] = h;
    v3[2] = z - (w / 2) * a[2];

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_texture_tree() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            texture_tree[i][j][0] = c / 8;
            texture_tree[i][j][1] = c / 2;
            texture_tree[i][j][2] = c / 4;
            if (c == 255)
                texture_tree[i][j][3] = 255;
            else
                texture_tree[i][j][3] = 0;
        }
    // Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++)
            texture_tree[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++)
            texture_tree[i][j][3] = 0;
    }
}


/*----------------------------------------------------------
 * Simple smoothstep function for smooth transitions
 */
float smoothstep(float edge0, float edge1, float x) {
    // Scale, and clamp x to 0..1 range
    x = fmaxf(0.0, fminf((x - edge0) / (edge1 - edge0), 1.0));
    // Evaluate polynomial
    return x * x * (3 - 2 * x);
}

/*----------------------------------------------------------
 * Procedure to make a cloud texture.
 */
void make_texture_cloud() {
    int i, j;
    float center_x = TSIZE / 2.0;
    float center_y = TSIZE / 2.0;
    float radius = TSIZE / 4.0;

    for (i = 0; i < TSIZE; i++) {
        for (j = 0; j < TSIZE; j++) {
            // Calculate distance from the center
            float distance = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));
            
            // Use a simple smoothstep-like function to create a circular shape
            float alpha = 1.0 - smoothstep(0.0, radius, distance);
            
            // Add some randomness to simulate cloud-like shapes
            float randomness = ((float)rand() / RAND_MAX) * 0.1;
            
            // Set RGBA values for the cloud texture
            texture_cloud[i][j][0] = 255;
            texture_cloud[i][j][1] = 255;
            texture_cloud[i][j][2] = 255;
            texture_cloud[i][j][3] = (unsigned char)(alpha * 255.0 * (1.0 - randomness));
        }
    }
}


/*-------------------------------------------------------
 * Procedure to compute the a[] and b[] axes of billboard
 * after eye parameters are specified.
 */
void compute_ab_axes(void) {
    float w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2];
    w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0;
    b[1] = 1.0;
    b[2] = 0.0;
    a[0] = w2 / len;
    a[1] = 0.0;
    a[2] = -w0 / len;
}





void drawSky() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);

    // 這裡使用淺藍色作為天空的顏色
    glColor4f(0.5, 0.7, 1.0, 1.0);

    glBegin(GL_POLYGON);
    glVertex3f(-50.0, -100.0, -50.0);
    glVertex3f(50.0, -100.0, -50.0);
    glVertex3f(50.0, 100.0, -50.0);
    glVertex3f(-50.0, 100.0, -50.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-50.0, 100.0, -50.0);
    glVertex3f(50.0, 100.0, -50.0);
    glVertex3f(50.0, 100.0, 50.0);
    glVertex3f(-50.0, 100.0, 50.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-50.0, -100.0, 50.0);
    glVertex3f(50.0, -100.0, 50.0);
    glVertex3f(50.0, 100.0, 50.0);
    glVertex3f(-50.0, 100.0, 50.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-50.0, -100.0, -50.0);
    glVertex3f(-50.0, 100.0, -50.0);
    glVertex3f(-50.0, 100.0, 50.0);
    glVertex3f(-50.0, -100.0, 50.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(50.0, -100.0, -50.0);
    glVertex3f(50.0, -100.0, 50.0);
    glVertex3f(50.0, 100.0, 50.0);
    glVertex3f(50.0, 100.0, -50.0);
    
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}


/*----------------------------------------------------------
 * Procedure to make a crescent moon texture.
 */
void make_texture_crescent_moon() {
    int i, j;
    float center_x = TSIZE / 2.0;
    float center_y = TSIZE / 2.0;
    float radius = TSIZE / 4.0;

    for (i = 0; i < TSIZE; i++) {
        for (j = 0; j < TSIZE; j++) {
            // Calculate distance from the center
            float distance = sqrt(pow(i - center_x, 2) + pow(j - center_y, 2));

            // Use a smoothstep-like function to create a circular shape
            float alpha = 1.0 - smoothstep(radius - 1.0, radius + 1.0, distance);

            // Ensure the left half is dark
            if (i > center_x) {
                alpha = 0.0;
            }

            // Set RGBA values for the moon texture
            texture_moon[i][j][0] = 255;
            texture_moon[i][j][1] = 255;
            texture_moon[i][j][2] = 255;
            texture_moon[i][j][3] = (unsigned char)(alpha * 255.0);
        }
    }
}





class Scene
{
private:
  /*-----Define GLU quadric objects, a sphere and a cylinder----*/
  static GLUquadricObj  *sphere;
  static GLUquadricObj  *cylind;
  static void startTextureGround();
  static void startTextureBrick();
  static void startTextureTree();
  static void startTextureCloud();
  static void startTextureMoon();
  static void stopTexture();
public:
  static void drawBuilding();
  static void drawBillBoard();
  static void drawXYZaxisWithRGB();
  static void drawOrigin();
  static void drawGroundSky();
  static void textureInitialize();
  static void fogInitialize();
  static void computeBillBoardAxes();
  static void fogTurn(bool);
  static void fogMode(int);
  static void fogColor(float, float, float);
  static void fogDensity(float);
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
  //glColor4f(0.0, 0.6, 1.0, 0.5);
  
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,scene_mat_building);
  //glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,8);
  //glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,scene_mat_spec);
  //glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,scene_mat_no_emission);

  
  
  startTextureBrick();
  gluQuadricTexture(cylind, GL_TRUE);
  glTranslatef(10.0, 0.0, -15.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);

  gluCylinder(cylind, 5.0, 5.0, 20.0, 12, 3);     
  //gluCylinder(cylind, 1.0, 1.0, 5.0, 12, 3);     
  glPopMatrix();

  stopTexture();
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

void Scene::drawGroundSky(){

  drawSky();

  glColor4f(0.2, 0.5, 0.5, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,scene_mat_ground);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,scene_mat_no_spec);
  startTextureGround();
  glBegin(GL_POLYGON);
    glTexCoord2f(0,1);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-80.0, 0, -80.0);
    glTexCoord2f(0,0);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-80.0, 0, 80.0);
    glTexCoord2f(1,0);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(80.0, 0, 80.0);
    glTexCoord2f(1,1);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(80.0, 0, -80.0);
  glEnd();
  stopTexture();
  glFlush();

}

void Scene::textureInitialize(){

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


  make_texture_ground(); // create the ground texture

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glGenTextures(6, textName);

  
  glBindTexture(GL_TEXTURE_2D, textName[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ground);
  

  make_texture_brick(); // create the brick pattern
  glBindTexture(GL_TEXTURE_2D, textName[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_brick);
  

  
  make_texture_tree();
  glBindTexture(GL_TEXTURE_2D, textName[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_tree);
  
  make_texture_cloud();
  glBindTexture(GL_TEXTURE_2D, textName[3]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_cloud);
  

  make_texture_crescent_moon();
  glBindTexture(GL_TEXTURE_2D, textName[4]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_moon);
  
}

void Scene::fogInitialize(){
  /*----Set up fog conditions ---*/
  glEnable(GL_FOG);                /*enable fog fade */
  glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
  glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
  glFogf(GL_FOG_START, 1.0);       /*Setup two ends for GL_LINEAR*/
  glFogf(GL_FOG_END, 36.0);
  glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
}

void Scene::startTextureGround(){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textName[0]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
}

void Scene::startTextureBrick(){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textName[1]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
}

void Scene::startTextureTree(){
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textName[2]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
}

void Scene::startTextureCloud(){
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textName[3]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
}

void Scene::startTextureMoon(){
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textName[4]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
}


void Scene::fogTurn(bool action){
  if(action){
    glEnable(GL_FOG);
  }else{
    glDisable(GL_FOG);
  }
}

void Scene::fogMode(int action){
  if(action==1){
    glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
  }else if(action==2){
    glFogi(GL_FOG_MODE, GL_EXP);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
  }else if(action==3){
    glFogi(GL_FOG_MODE, GL_EXP2);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
  }
}

void Scene::fogColor(float r, float g, float b){
  fog_color[0] = r;
  fog_color[1] = g;
  fog_color[2] = b;

  glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
}

void Scene::fogDensity(float d){
  glFogf(GL_FOG_DENSITY, d);    /*fog opacity(density)= 0.25*/
}



void Scene::stopTexture(){
  glDisable(GL_TEXTURE_2D);
}

void Scene::drawBillBoard(){

  startTextureTree();
  draw_billboard(5.0 * 4.0, 3.0, 10.0, 15.0);
  draw_billboard(6.0 * 4.0, -5.0, 10.0, 15.0);
  draw_billboard(-3.0 * 4.0, -10.0, 10.0, 15.0);
  draw_billboard(-2.0 * 4.0, -15.0, 10.0, 15.0);
  draw_billboard(-7.0 * 4.0, 2.0 * 4.0, 10.0, 15.0);
  stopTexture();
  fprintf(stderr, "tree \n");

  startTextureCloud();
  glMatrixMode(GL_MODELVIEW); // go to sky coord.
  glPushMatrix();
  glTranslatef(0.0, 20.0, 0.0);

  glMatrixMode(GL_TEXTURE); // rotate the texture
  glPushMatrix();
  glLoadIdentity();
  //glRotatef(90.0, 0.0, 0.0, 1.0);

  draw_billboard(23.0, -16.0, 30.0, 20.0);
  draw_billboard(25.0, -15.0, 30.0, 15.0);
  draw_billboard(-3.0, -10.0, 30.0, 15.0);
  draw_billboard(-5.0, -11.0, 30.0, 15.0);
  draw_billboard(10.0, -12.0, 10.0, 15.0);
  draw_billboard(12.0, -13.0, 10.0, 15.0);
  draw_billboard(-10.0, -10.0, 25.0, 15.0);
  draw_billboard(-13.0, -11.0, 10.0, 15.0);
  draw_billboard(13.0, -17.0, 25.0, 15.0);
  draw_billboard(15.0, -8.0, 15.0, 10.0);
  draw_billboard(18.0, -7.0, 20.0, 15.0);
  draw_billboard(19.0, -8.0, 15.0, 10.0);
  stopTexture();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix(); // cancel the texture rotation

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix(); // leave the sky coord.

  startTextureMoon();
  glMatrixMode(GL_MODELVIEW); // go to moon coord.
  glPushMatrix();
  glTranslatef(40.0, 30.0, -20.0);
  glRotatef(90.0, 0.0, 0.0, 1.0);

  draw_billboard(0.0, 0.0, 30.0, 20.0);

  stopTexture();

  glPopMatrix(); // leave the moon coord.
}

void Scene::computeBillBoardAxes(){
  glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
  compute_ab_axes();
  fprintf(stderr, "computer axes \n");
}
