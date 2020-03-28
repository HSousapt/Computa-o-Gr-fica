#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

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
};

//Struct that represents a Geometric Transformation
struct gt
{
    struct Point p;
    enum type gt_type;
    float r_angle;
};

//Struct that represents a group and its subgroups of models and their associated geometric transformations
struct group
{
    vector<struct gt> gt;
    vector<vector<struct Point>> models;
    vector<struct group> child;
};

//Struct that represents a scene has a vector of groups
struct scene
{
    vector<struct group> groups;
    int nModels;
};

/*Function that returns the data from the .xml(received as a parameter) and stores them in a "struct group"
ready to be used*/
struct group process_groups(TiXmlElement *);

/*Fuction that draws the scene stored in a vector of groups received as its parameter*/
void draw_scene(vector<struct group>);

void load_scene(struct scene *, TiXmlElement *);