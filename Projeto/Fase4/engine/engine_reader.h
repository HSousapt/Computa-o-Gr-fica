#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

using namespace std;

//Struct that represents a point
struct Point
{
    float x;
    float y;
    float z;
};

//Geometric Transformations types
enum type
{
    ROTATE,
    TRANSLATE,
    SCALE,
    ROTATE_ANIM,
    TRANSLATE_ANIM
};

//Struct that represents a Geometric Transformation
struct gt
{
    struct Point p;
    enum type gt_type;
    float r_angle;
    float rotate_time;
    float translate_time;
    vector<Point> control_points;
};

enum l_type
{
    DIREC,    // Directional Light
    SPOT,     // Spotlight
    AMBIENT,  //Ambient light
    DIFFUSE,  // Diffuse light
    SPECULAR, // Specular light
};

struct light
{
    enum l_type type;
    GLfloat pos[4];
    GLfloat color[4];
};

enum material_type
{
    DIF,
    SPEC,
    EMI,
    AMB,
    SHINE,
};

struct material
{
    GLfloat color[4];
    GLfloat shininess;
    enum material_type type;
};

struct model
{
    vector<struct Point> points;
    vector<struct Point> normals;
    vector<struct Point> textcords;
    bool hastexture = false;
    bool hasmaterial = false;
    GLuint textID;
    const char *textName;
    vector<struct material> materials;
};

//Struct that represents a group and its subgroups of models and their associated geometric transformations
struct group
{
    vector<struct model> models;
    vector<struct gt> gt;
    vector<struct group> child;
};

//Struct that represents a scene as a vector of groups and and int that tells us how many models the scene has
struct scene
{
    vector<struct group> groups;
    vector<vector<struct light>> lights;
    int nModels;
    int nTextures;
};

//Fuction that draws the geometric transformations
void draw_gt(struct group group, int);

//Loads the data from the .xml file and stores it in the struct scene
void load_groups(struct scene *, TiXmlElement *);

void load_scene(struct scene *, TiXmlElement *);

//Draws the models without VBO's
void draw_models(struct group);

//Draws the color
void draw_color(struct Point);

//Draws the lights
void render_lighting(vector<vector<struct light>> lights);

//loads the textures of the scene
void load_textures(vector<struct group> *);

void draw_materials(vector<struct material>);