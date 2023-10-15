/* define all object that use in drawing_panel
 *   and relative functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

/*define menu input */
#define    DO_NOTHING  0
#define    POLYGON_START  1
#define    POLYGON_FINISH  2
#define    DRAW_PEN  3
#define    DRAW_LINE  4
#define    DRAW_CIRCLE  5
#define    DRAW_RECTANGLE  6
#define    INPUT_STRING  7
#define    MENU_EXIT  8
#define    INPUT_CURSOR  9
#define    MENU_CLEAN  10
#define    DRAW_POINT  11
#define    COLOR_NO_FILL 0
#define    COLOR_FILL  1
#define    COLOR_RED  2
#define    COLOR_GREEN  3
#define    COLOR_BLUE  4
#define    COLOR_WHITE  5
#define    COLOR_YELLOW  6
#define    COLOR_CYAN  7
#define    COLOR_PURPLE  8
#define    MAX_STRING_LEN  50
#define    SHOW  1
#define    NO_SHOW  0


/* define object */
struct Position
{
  float x, y;
  struct Position *next;
};
typedef struct Position position;

struct Object
{
  GLUquadricObj *special_type; // for complex objects like circle
  int this_type, this_size, if_fill, if_show;
  float this_color[3];
  position *pos;
  /* set the bound of this object 
   * bound[0] for top-left, bound[1] for bottom-right
   */
  position bound[2];
  /*if it is string type, then use bound[1] to store start point
   *   and use if_fill as the length of string 
   */
  char string[MAX_STRING_LEN];
  struct Object *next;
};
typedef struct Object object;

object *first_objects=NULL; // store the first object
object *cur_objects=NULL; // store the current (latest) object

/* create new object and set cur_object point to new location */
void newObject(int type, float size, int fill, int show, float *color) {
  object *newObject = (object *)malloc(sizeof(object));
  if(!newObject){
    fprintf(stderr, "newObject() failed\n");
    return;
  }else{
    fprintf(stderr, "newObject() successed\n");
  }
  newObject->special_type = NULL;
  newObject->this_type = type;
  newObject->this_size = size;
  newObject->if_fill = fill;
  newObject->if_show = show;
  newObject->next = NULL;
  newObject->pos = NULL;
  for(int i=0; i<3; i++) newObject->this_color[i] = color[i];
  /* renew the linked list pointer */
  if(!first_objects){
    first_objects = newObject;
    cur_objects = newObject;
  }else{
    cur_objects->next = newObject;
    cur_objects = newObject;
  }
}

/* delete object and release memory */
void deleteObject(object *obj){
  position *pos = obj->pos;
  while (pos != NULL) {
      position *temp = pos;
      pos = pos->next;
      free(temp);
      temp = NULL;
    }
  free(obj);
}

/* delete all objects and release memory */
void deleteAllObjects() {
  fprintf(stderr, "deleteAllObjects() successed\n");
  while (first_objects != NULL) {
    position *pos = first_objects->pos;
    while (pos != NULL) {
      position *temp = pos;
      pos = pos->next;
      free(temp);
      temp = NULL;
    }
    object *temp = first_objects;
    first_objects = first_objects->next;
    free(temp);
    temp = NULL;
  }
  cur_objects = NULL;
  first_objects = NULL;
}

/* add new xy positions to given object*/
void addPosToObject(object *obj, int x, int y){
  position *newPos = (position *)malloc(sizeof(position));
  newPos->x = (float)x;
  newPos->y = (float)y;
  newPos->next = NULL;
  if (obj->pos == NULL) {
    obj->pos = newPos;
    obj->bound[0].x = obj->bound[1].x = x;
    obj->bound[0].y = obj->bound[1].y = y;
  }else{
    position *current = obj->pos;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newPos;
    if(x<obj->bound[0].x){
      obj->bound[0].x = x;
    }else if(x>obj->bound[1].x){
      obj->bound[1].x = x;
    }
    if(y>obj->bound[0].y){
      obj->bound[0].y = y;
    }else if(y<obj->bound[1].y){
      obj->bound[1].y = y;
    }
  }
}

/* for the gluNewQuadric object */
void specialTypeInit(object *obj){
  if(obj->special_type==NULL){
    obj->special_type = gluNewQuadric();
    gluQuadricDrawStyle(obj->special_type, GLU_FILL);
  }
}

/* Procedure to draw a circle */
void drawCircle(object *obj)
{
  if(!obj->special_type) return;
  position *pos = obj->pos;
  position *pos_next = pos->next;
  float radius = 0.5*sqrt(pow(pos->x-pos_next->x, 2)+pow(pos->y-pos_next->y, 2));
  glPushMatrix();
  glTranslatef(0.5*(pos->x+pos_next->x), 0.5*(pos->y+pos_next->y), 0.0);
  gluDisk(obj->special_type,
    (obj->if_fill)?0.0:(radius - obj->this_size),   /* inner radius */
	  radius,    /* outer radius */
	  25,            /* 25-side polygon */
	  3);
  glPopMatrix();
}

/* show objects */
void showObject(object *obj){
  /* check if we show it*/
  if(!obj->if_show) return;
  /* apply openGL states */
  glColor3fv(obj->this_color);
  /* show objects depend on its type */
  switch (obj->this_type){
  case POLYGON_FINISH:
    {
    position *pos_poly = obj->pos;
    /*set if fill the polygon*/
    glPolygonMode(GL_FRONT_AND_BACK, (obj->if_fill)?GL_FILL:GL_LINE);
    glLineWidth(obj->this_size);
    glBegin(GL_POLYGON);
      while(pos_poly){
        glVertex2i(pos_poly->x, pos_poly->y);
        pos_poly = pos_poly->next;
      }
    glEnd();
    break;
    }
  case DRAW_CIRCLE:
    {
    position *pos_circle = obj->pos;
    if(!pos_circle || !pos_circle->next) return;
    drawCircle(obj);
    break;
    }
  case DRAW_LINE:
    {
    position *pos_line = obj->pos;
    if(!pos_line || !pos_line->next) return;
    position *pos_line_next = pos_line->next;
    glLineWidth(obj->this_size);
    glBegin(GL_LINES);
      glVertex3f(pos_line->x, pos_line->y, 0.0);
      glVertex3f(pos_line_next->x, pos_line_next->y, 0.0);
    glEnd();
    break;
    }
  case INPUT_STRING:
    {
    int pos_str_x = obj->bound[1].x;
    int pos_str_y = obj->bound[1].y;
    for(int i=0; i<obj->if_fill; i++){
      glRasterPos2i(pos_str_x, pos_str_y);
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int) obj->string[i]);
      pos_str_x += 10;
    }
    break;
    }
  case DRAW_POINT:
    {
    if(!obj->pos) return;
    glPointSize(obj->this_size);
    glBegin(GL_POINTS);     /*  Draw a point */
      glVertex2f(obj->pos->x, obj->pos->y);
    glEnd();
    break;
    }
  default:
    break;
  }
  glFlush();
}


