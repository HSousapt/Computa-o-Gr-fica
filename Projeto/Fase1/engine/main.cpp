#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#include "engine_reader.h"

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

int polMode = 0;

float px = 5.0, py = 5.0, pz = 5.0;
float lx = 0.0, ly = 0.0, lz = 0.0;
float alpha = M_PI_2 / 2, beta = M_PI_2 / 2;
float r = sqrt((px * px) + (py * py) + (pz * pz));

vector<vector<struct Point>> models;

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

void renderScene(void)
{
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(px, py, pz,
              lx, ly, lz,
              0.0f, 1.0f, 0.0f);

    // put the geometric transformations here

    // put drawing instructions here
    for (unsigned i = 0; i < models.size(); i++)
        for (unsigned j = 0; j < models[i].size(); j += 3)
        {
            glBegin(GL_TRIANGLES);
            glVertex3f(models[i][j].x, models[i][j].y, models[i][j].z);
            glVertex3f(models[i][j + 1].x, models[i][j + 1].y, models[i][j + 1].z);
            glVertex3f(models[i][j + 2].x, models[i][j + 2].y, models[i][j + 2].z);
            glEnd();
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
    }
}

//refresh the values of  px,py,pz of the camera

void refreshCam()
{
    px = r * cos(beta) * sin(alpha);
    py = r * sin(beta);
    pz = r * cos(beta) * cos(alpha);
}

void processSpecialKeys(int key, int xx, int yy)
{
    // put code to process special keys in here
    switch (key)
    {
    case GLUT_KEY_UP:
        if (beta <= M_PI_2 - 0.1)
            beta += 0.1;
        break;
    case GLUT_KEY_DOWN:
        if (beta >= -M_PI_2 + 0.1)
            beta -= 0.1;
        break;
    case GLUT_KEY_LEFT:
        alpha += 0.1;
        break;
    case GLUT_KEY_RIGHT:
        alpha -= 0.1;
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
        r -= 0.2;
        break;
    case 's':
        r += 0.2;
        break;
    }

    refreshCam();
    glutPostRedisplay();
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
        printf("ERROR: Error opening .xml File!!");
        return -2;
    }
    TiXmlElement *root = doc.RootElement();
    for (TiXmlElement *pElement = root->FirstChild("model")->ToElement(); pElement; pElement = pElement->NextSiblingElement())
    {
        const char *file = pElement->Attribute("file");
        if (file)
        {
            fstream f;
            string file3d = fileDir + file;
            f.open(file3d, ios::in);
            if (f.is_open())
            {
                string line;
                vector<struct Point> points;
                getline(f, line);
                while (!line.empty())
                {
                    double x, y, z;

                    sscanf(line.c_str(), "%lf %lf %lf\n", &x, &y, &z);

                    struct Point p = point(x, y, z);
                    points.push_back(p);
                    getline(f, line);
                }
                models.push_back(points);
                f.close();
            }
            else
            {
                printf("ERROR: Error opening .3d File!!");
                return -2;
            }
        }
        else
        {
            printf("ERROR: Error opening .xml File!!");
            return -3;
        }
    }

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Fase_1");
    //refreshCamPosition();

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutMouseFunc(change_mode);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    //  OpenGL settings
    glPolygonMode(GL_FRONT, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}