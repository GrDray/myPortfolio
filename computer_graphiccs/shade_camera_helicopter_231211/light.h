#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#define LIGHT_OFF 0
#define LIGHT_ON 1

#define LIGHT_0 0 // directional light from sun
#define LIGHT_1 1
#define LIGHT_2 2
#define LIGHT_3 3
#define LIGHT_4 4
#define LIGHT_5 5
#define LIGHT_6 6
#define LIGHT_7 7


class Light
{
private:
    static float global_ambient[4];
    static GLUquadricObj  *sphere;
    static GLUquadricObj  *cylind;
public:

    static float light0_dir[4], light0_diffuse[4], light0_specular[4];
    static float light1_pos[4], light1_diffuse[4], light1_specular[4];
    static float light2_pos[4], light2_diffuse[4], light2_specular[4];
    static float light2_cutoff_angle;
    static float sphere_light_emission[4], sphere_light_specular[4], sphere_light_diffuse[4];
    static float spot_light_direction[4];

    static void turnLight(int lightNum, int action);
    static void drawEmissionSphere();
    static void initialize();
    static void spotLight();
};

GLUquadricObj  *Light::sphere=NULL;
GLUquadricObj  *Light::cylind=NULL;

float Light::global_ambient[] = {0.2, 0.2, 0.2, 1.0};
float Light::light0_dir[4]={1.0, -1.0, 1.0, 0.0};
float Light::light0_diffuse[4]={1.0, 1.0, 1.0, 1.0};
float Light::light0_specular[4]={1.0, 1.0, 1.0, 1.0};
float Light::light1_pos[4]={20.0, 20.0, -20.0, 1.0};
float Light::light1_diffuse[4]={1.0, 1.0, 1.0, 1.0};
float Light::light1_specular[4]={1.0, 1.0, 1.0, 1.0};
float Light::light2_pos[4]={0.0, 0.0, 0.0, 1.0};
float Light::light2_diffuse[4]={1.0, 1.0, 1.0, 1.0};
float Light::light2_specular[4]={1.0, 1.0, 1.0, 1.0};
float Light::light2_cutoff_angle= 91 ;
float Light::sphere_light_emission[4]={0.1,0.1,0.1,1.0};
float Light::sphere_light_specular[4]={1.0, 1.0, 1.0, 1.0};
float Light::sphere_light_diffuse[4]={1.0, 1.0, 1.0, 1.0};
float Light::spot_light_direction[4]={0.0, 1.0, 0.0, 0.0};

void Light::turnLight(int lightNum, int action)
{
  switch (lightNum)
  {
  case LIGHT_0:
    if(action){
      glEnable(GL_LIGHT0);
    }else{
      glDisable(GL_LIGHT0);
    }
    break;
  case LIGHT_1:
    if(action){
      glEnable(GL_LIGHT1);
    }else{
      glDisable(GL_LIGHT1);
    }
    break;
  case LIGHT_2:
    if(action){
      glEnable(GL_LIGHT2);
    }else{
      glDisable(GL_LIGHT2);
    }
    break;
  case LIGHT_3:
    if(action){
      glEnable(GL_LIGHT3);
    }else{
      glDisable(GL_LIGHT3);
    }
    break;
  case LIGHT_4:
    if(action){
      glEnable(GL_LIGHT4);
    }else{
      glDisable(GL_LIGHT4);
    }
    break;
  case LIGHT_5:
    if(action){
      glEnable(GL_LIGHT5);
    }else{
      glDisable(GL_LIGHT5);
    }
    break;
  case LIGHT_6:
    if(action){
      glEnable(GL_LIGHT6);
    }else{
      glDisable(GL_LIGHT6);
    }
    break;
  case LIGHT_7:
    if(action){
      glEnable(GL_LIGHT7);
    }else{
      glDisable(GL_LIGHT7);
    }
    break;
  
  default:
    break;
  }






  
}

void Light::initialize(){
  //turnLight(LIGHT_0, LIGHT_ON);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  //turnLight(LIGHT_1, LIGHT_ON);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  /*
  glPushMatrix();
    glTranslatef(0.0, 10, -5.0);
    Light::spotLight();
  glPopMatrix();
  */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

void Light::drawEmissionSphere(){
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }
  glColor4f(1.0, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,sphere_light_diffuse);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,8);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,sphere_light_specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,sphere_light_emission);
  gluSphere(sphere,1.0,64,64);        
  
}

void Light::spotLight(){
  drawEmissionSphere();
  glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 8);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2_cutoff_angle);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_light_direction);
  //glEnable(GL_LIGHT2);
}







