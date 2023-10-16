/* define all object that use in drawing_panel
 *   and relative functions.
 */
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>


#define    OBJ_POLYGON  21
#define    OBJ_CIRCLE  22
#define    OBJ_LINE  23
#define    OBJ_STRING  24
#define    OBJ_POINT  25
#define    MAX_STRING_LEN  50


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


class MyObject {
  public:
    static object *first_objects;  // store the first object
    static object *cur_objects;  // store the current (latest) object

    /* create new object and set cur_object point to new location */
    static void newObject(int type, float size, int fill, int show, float *color) {
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
    static void deleteObject(object *obj){
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
    static void deleteAllObjects() {
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
        fprintf(stderr, "deleteAllObjects() successed\n");
      }
      cur_objects = NULL;
      first_objects = NULL;
    }

    
    /* add new xy positions to given object*/
    static void addPosToObject(object *obj, int x, int y){
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
      fprintf(stderr, "addNewPos() called\n");
    }

    
    /* for the gluNewQuadric object */
    static void specialTypeInit(object *obj){
      if(obj->special_type==NULL){
        obj->special_type = gluNewQuadric();
        gluQuadricDrawStyle(obj->special_type, GLU_FILL);
      }
    }

    
    /* Procedure to draw a circle */
    static void drawCircle(object *obj)
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
    static void showObject(object *obj){
      /* check if we show it*/
      if(!obj->if_show) return;
      /* apply openGL states */
      glColor3fv(obj->this_color);
      /* show objects depend on its type */
      switch (obj->this_type){
      case OBJ_POLYGON:
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
      case OBJ_CIRCLE:
        {
          fprintf(stderr, "circle show1\n");
        position *pos_circle = obj->pos;
        if(!pos_circle || !pos_circle->next) return;
        drawCircle(obj);
        fprintf(stderr, "circle show2\n");
        break;
        }
      case OBJ_LINE:
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
      case OBJ_STRING:
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
      case OBJ_POINT:
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


  private:
};

object *MyObject::first_objects = NULL;
object *MyObject::cur_objects = NULL;
