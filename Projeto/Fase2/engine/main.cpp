#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "engine_reader.h"

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

using namespace std;

int polMode = 0;

float px, py, pz;
float radius = 25;
float lx = 0.0, ly = 0.0, lz = 0.0;
float alpha = 45.0, beta = 45.0;
int frame = 0, timebase = 0;

struct scene scene;

GLuint *buffers;

int *n_verteces;

int buffer_counter, draw_counter;

//refresh the values of  px,py,pz of the camera
void refreshCam()
{
    px = radius * cos(beta) * sin(alpha);
    py = radius * sin(beta);
    pz = radius * cos(beta) * cos(alpha);
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    double ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void draw_vbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, buffers[draw_counter]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, n_verteces[draw_counter++]);
}

void draw_scene(vector<struct group> groups)
{
    for (int i = 0; i < groups.size(); i++)
    {
        struct group group = groups[i];
        glPushMatrix();
        {
            draw_gt(group);
            draw_scene(group.child);
            draw_vbo();
            //draw_models(group);
        }
        glPopMatrix();
    }
}

void renderScene(void)
{
    int time;
    char s[64];
    float fps;
    draw_counter = 0;
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(px, py, pz,
              lx, ly, lz,
              0.0f, 1.0f, 0.0f);

    // put the geometric transformations here

    draw_scene(scene.groups);

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000)
    {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %6.2f", fps);
        glutSetWindowTitle(s);
    }

    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events
void change_mode(int button, int state, int x, int y)
{
    polMode = (polMode + 1) % 3;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (polMode == 0)
            glPolygonMode(GL_FRONT, GL_LINE);
        if (polMode == 1)
            glPolygonMode(GL_FRONT, GL_POINT);
        if (polMode == 2)
            glPolygonMode(GL_FRONT, GL_FILL);
        glutPostRedisplay();
    }
}

void processSpecialKeys(int key, int xx, int yy)
{
    // put code to process special keys in here
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        alpha -= 0.1;
        break;

    case GLUT_KEY_LEFT:
        alpha += 0.1;
        break;

    case GLUT_KEY_UP:
        beta += 0.1f;
        if (beta > 1.5f)
            beta = 1.5f;
        break;

    case GLUT_KEY_DOWN:
        beta -= 0.1f;
        if (beta < -1.5f)
            beta = -1.5f;
        break;
    }
    refreshCam();
    glutPostRedisplay();
}

void processKeys(unsigned char key, int xx, int yy)
{

    // put code to process regular keys in here
    switch (key)
    {
    case 'w':
        radius -= 1;
        break;
    case 's':
        radius += 1;
        break;
    }

    refreshCam();
    glutPostRedisplay();
}

void fill_buffers(vector<struct group> groups)
{
    for (int k = 0; k < groups.size(); k++)
    {
        fill_buffers(groups[k].child);

        vector<vector<struct Point>> models = groups[k].models;

        for (int i = 0; i < models.size(); i++)
        {

            n_verteces[buffer_counter] += models[i].size();

            glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer_counter++]);

            vector<struct Point> model = models[i];

            float *verteces = (float *)malloc(sizeof(float *) * model.size() * 3);

            for (int j = 0; j < model.size(); j++)
            {
                verteces[3 * j] = model[j].x;
                verteces[3 * j + 1] = model[j].y;
                verteces[3 * j + 2] = model[j].z;
            }

            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.size() * 3, verteces, GL_STATIC_DRAW);

            free(verteces);
        }
    }
}

void prepare_vbo_data()
{
    n_verteces = (int *)malloc(sizeof(int) * scene.nModels);

    glEnableClientState(GL_VERTEX_ARRAY);

    buffer_counter = 0;

    buffers = (GLuint *)malloc(sizeof(GLuint) * scene.nModels);

    glGenBuffers(scene.nModels, buffers);

    fill_buffers(scene.groups);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Input Error!\n");
        return -1;
    }
    string fileDir = "../../assets/";
    string xmlFile = fileDir + argv[1];
    TiXmlDocument doc(xmlFile.c_str());
    if (!doc.LoadFile())
    {
        printf("ERROR: Error opening .xml File!!\n");
        return -2;
    }
    TiXmlElement *root = doc.RootElement();
    for (TiXmlElement *group = root->FirstChild("group")->ToElement(); group; group = group->NextSiblingElement())
    {
        //scene.groups.push_back(process_groups(group));
        load_scene(&scene, group);
    }

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Fase_2");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
    glutMouseFunc(change_mode);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    //  OpenGL settings
    glPolygonMode(GL_FRONT, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (glewInit() != GLEW_OK)
    {
        printf("glew initialization failed!");
        return -1;
    }

    refreshCam();

    prepare_vbo_data();

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}