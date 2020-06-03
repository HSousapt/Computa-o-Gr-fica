#include "engine_reader.h"

using namespace std;

int polMode = 2;

float px, py, pz;
float radius = 300;
float lx = 0.0, ly = 0.0, lz = 0.0;
float alpha = 45.0, beta = 45.0;
int frame = 0, timebase = 0;
int startX, startY, tracking = 0;
bool on_off = true;

struct scene scene;

GLuint *buffers, *normals, *texts;

int *n_verteces;

int buffer_counter, draw_counter;

int text_counter, draw_text;

//refresh the values of  px,py,pz of the camera
void refreshCam()
{
    px = radius * cos(beta) * sin(alpha);
    py = radius * sin(beta);
    pz = radius * cos(beta) * cos(alpha);
}

void processMouseButtons(int button, int state, int xx, int yy)
{
    if (state == GLUT_DOWN)
    {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP)
    {
        if (tracking == 1)
        {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2)
        {

            radius -= yy - startY;
            if (radius < 3)
                radius = 3.0;
        }
        tracking = 0;
    }
}

void processMouseMotion(int xx, int yy)
{
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1)
    {

        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 83.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = radius;
    }
    else if (tracking == 2)
    {

        alphaAux = alpha;
        betaAux = beta;
        rAux = radius - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    px = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    pz = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    py = rAux * sin(betaAux * 3.14 / 180.0);
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

void draw_vbo(vector<struct model> models)
{
    for (struct model m : models)
    {
        if (m.hasmaterial)
        {
            draw_materials(m.materials);
        }
        glBindBuffer(GL_ARRAY_BUFFER, buffers[draw_counter]);
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, normals[draw_counter]);
        glNormalPointer(GL_FLOAT, 0, 0);

        if (m.hastexture)
        {
            glBindTexture(GL_TEXTURE_2D, m.textID);
            glBindBuffer(GL_ARRAY_BUFFER, texts[draw_text++]);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
        }
        glDrawArrays(GL_TRIANGLES, 0, n_verteces[draw_counter++]);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void draw_scene(vector<struct group> groups, int time)
{
    for (int i = 0; i < groups.size(); i++)
    {
        struct group group = groups[i];
        glPushMatrix();
        {
            draw_gt(group, time);
            draw_vbo(group.models);
            draw_scene(group.child, time);
        }
        glPopMatrix();
    }
}

void show_fps(int time)
{
    float fps;
    char s[64];

    frame++;
    if (time - timebase > 1000)
    {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f7.3", fps);
        glutSetWindowTitle(s);
    }
}

void activate_lights(bool on_off)
{
    if (on_off)
    {
        if (scene.lights.size() > 0)
        {
            render_lighting(scene.lights);
        }
    }
    else
        glDisable(GL_LIGHTING);
}

void renderScene(void)
{
    int time;
    draw_counter = 0;
    draw_text = 0;

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(px, py, pz,
              lx, ly, lz,
              0.0f, 1.0f, 0.0f);

    activate_lights(on_off);
    // put the geometric transformations here
    time = glutGet(GLUT_ELAPSED_TIME);

    draw_scene(scene.groups, time);

    // End of frame
    glutPostRedisplay();
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

void processKeys(unsigned char key, int xx, int yy)
{
    // put code to process regular keys in here
    polMode = (polMode + 1) % 3;
    switch (key)
    {
    case 'm':
    {
        if (polMode == 0)
            glPolygonMode(GL_FRONT, GL_LINE);
        if (polMode == 1)
            glPolygonMode(GL_FRONT, GL_POINT);
        if (polMode == 2)
            glPolygonMode(GL_FRONT, GL_FILL);
        break;
    }
    case 'l':
        if (on_off)
            on_off = false;
        else
            on_off = true;
        break;
    }
    glutPostRedisplay();
}

void fill_buffers(vector<struct group> groups)
{
    for (int k = 0; k < groups.size(); k++)
    {
        vector<struct model> models = groups[k].models;

        for (int i = 0; i < models.size(); i++)
        {
            vector<struct Point> points = models[i].points;
            vector<struct Point> norms = models[i].normals;
            vector<struct Point> tex = models[i].textcords;

            n_verteces[buffer_counter] += points.size();

            float *v = (float *)malloc(sizeof(float *) * points.size() * 3);
            float *n = (float *)malloc(sizeof(float *) * points.size() * 3);
            float *t = (float *)malloc(sizeof(float *) * points.size() * 2);

            for (int j = 0; j < points.size(); j++)
            {
                v[3 * j] = points[j].x;
                v[3 * j + 1] = points[j].y;
                v[3 * j + 2] = points[j].z;

                n[3 * j] = norms[j].x;
                n[3 * j + 1] = norms[j].y;
                n[3 * j + 2] = norms[j].z;

                t[2 * j] = tex[j].x;
                t[2 * j + 1] = tex[j].y;
            }

            glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer_counter]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size() * 3, v, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, normals[buffer_counter++]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * norms.size() * 3, n, GL_STATIC_DRAW);

            if (models[i].hastexture)
            {
                glBindBuffer(GL_ARRAY_BUFFER, texts[text_counter++]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tex.size() * 2, t, GL_STATIC_DRAW);
            }

            free(v);
            free(n);
            free(t);
        }
        fill_buffers(groups[k].child);
    }
}

void prepare_vbo_data()
{
    n_verteces = (int *)malloc(sizeof(int) * scene.nModels);

    buffer_counter = 0;

    text_counter = 0;

    buffers = (GLuint *)malloc(sizeof(GLuint) * scene.nModels);

    normals = (GLuint *)malloc(sizeof(GLuint) * scene.nModels);

    texts = (GLuint *)malloc(sizeof(GLuint) * scene.nTextures);

    glGenBuffers(scene.nModels, buffers);
    glGenBuffers(scene.nModels, normals);
    glGenBuffers(scene.nTextures, texts);

    fill_buffers(scene.groups);
}

void init_Gl(void)
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnableClientState(GL_NORMAL_ARRAY);

    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_RESCALE_NORMAL);
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

    load_scene(&scene, root);
    printf("Use o rato para andar e olhar à volta\n");
    printf("l -> ligar/desligar luzes\n");
    printf("m -> mudar o modo de desenho\n");
    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Fase_3");

    // Required callback registry
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);

    // put here the registration of the keyboard callbacks
    glutMouseFunc(change_mode);
    glutKeyboardFunc(processKeys);
    glutMotionFunc(processMouseMotion);
    glutMouseFunc(processMouseButtons);

    //  OpenGL settings
    if (glewInit() != GLEW_OK)
    {
        printf("glew initialization failed!");
        return -1;
    }

    init_Gl();

    prepare_vbo_data();

    load_textures(&(scene.groups));

    refreshCam();

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}