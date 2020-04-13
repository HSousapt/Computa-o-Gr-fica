#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

int frame = 0, timebase = 0;

int polMode = 0;
GLfloat spin = 0.0;

unsigned int imageHeight, imageWidth;
GLuint verteces, vertecesCount;
ILubyte *imageData;

float alpha = 0;
float camX = 0, camY = 0, camZ = 0;
float lx = 0, lz = 1;
int heye = 5;

void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45, ratio, 1, 1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float height(int i, int j)
{
	return (float)imageData[i * imageWidth + j];
}

float hf(float x, float z)
{
	float h = ((float)imageHeight - 1) / 2;
	float w = ((float)imageWidth - 1) / 2;

	//what??
	float terrainZ = x + w;
	float terrainX = z + h;

	float x1 = floor(terrainX);
	float z1 = floor(terrainZ);
	float x2 = x1 + 1;
	float z2 = z1 + 1;

	float fz = terrainZ - z1;
	float fx = terrainX - x1;

	float h_x1z = height(x1, z1) * (1 - fz) + height(x1, z2) * fz;
	float h_x2z = height(x2, z1) * (1 - fz) + height(x2, z2) * fz;

	float h_xz = h_x1z * (1 - fx) + h_x2z * fx;

	return h_xz;
}

void Inner_Teapots(int tps)
{
	glPushMatrix();
	{
		glTranslatef(0.0, 2, 0.0);

		double ang = 2 * M_PI / tps;
		float to_degrees = 180 / M_PI;

		for (int i = 0; i < tps; i++)
		{
			float x = 10 * cos(i * ang + spin);
			float z = 10 * sin(i * ang + spin);

			glPushMatrix();
			{
				glColor3d(0.0, 0.0, 1);
				glTranslatef(x, hf(x, z), z);
				glRotatef(-atan2(z, x) * to_degrees, 0, 1, 0);
				glutSolidTeapot(2);
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void Outter_Teapots(int tps)
{
	glPushMatrix();
	{
		glTranslatef(0.0, 2, 0.0);

		double ang = 2 * M_PI / tps;
		float to_degrees = 180 / M_PI;

		for (int i = 0; i < tps; i++)
		{
			float x = 35 * cos(i * ang - spin);
			float z = 35 * sin(i * ang - spin);

			glPushMatrix();
			{
				glColor3d(1.0, 0.0, 0.0);
				glTranslatef(x, hf(x, z), z);
				glRotatef(atan2(x, z) * to_degrees, 0, 1, 0);
				glutSolidTeapot(2);
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void Tree()
{
	glPushMatrix();
	//Tronco da arvore
	glRotatef(-90, 1, 0.0, 0.0);
	glColor3d(0.5f, 0.35f, 0.05f);
	glutSolidCone(0.5, 5, 10, 10);

	//Ramos e folhas
	glPushMatrix();
	glColor3f(0.1f, 0.7f, 0.1f);
	glTranslatef(0.0, 0.0, 3);
	glutSolidCone(2, 5, 10, 10);
	glPopMatrix();
	glPopMatrix();
}

void Draw_Trees(int n)
{
	srand((unsigned int)n);

	for (int i = 0; i < n; i++)
	{

		float x = 0;
		float z = 0;

		while (sqrt((x * x) + (z * z)) < 55)
		{
			x = (float)(rand() % 200) - 100;
			z = (float)(rand() % 200) - 100;
		}

		glPushMatrix();
		{
			glTranslatef(x, hf(x, z), z);
			Tree();
		}
		glPopMatrix();
	}
}

void drawTerrain()
{
	glBindBuffer(GL_ARRAY_BUFFER, verteces);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for (int i = 0; i < imageHeight - 1; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, (imageWidth)*2 * i, (imageWidth)*2);
	}
}

void show_fps(void)
{
	int time;
	char s[64];
	float fps;

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
}

void draw_scene()
{
	glColor3d(1.0f, 1.0f, 1.0f);

	drawTerrain();

	glPushMatrix();
	{
		glColor3d(1.0f, 0.0, 1.0f);
		glTranslatef(0.0, 1.0, 0.0);
		glutSolidTorus(1, 3, 10, 10);
	}
	glPopMatrix();

	Inner_Teapots(8);

	Outter_Teapots(16);

	Draw_Trees(250);

	show_fps();
}

void displaytext(char const *string, GLint x, GLint y)
{
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 800, 0.0, 800);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0f, 0.25f, 0.0f);
	glRasterPos2i(x, y);
	void *font = GLUT_BITMAP_TIMES_ROMAN_24;
	for (int i = 0; i < strlen(string); i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void renderScene(void)
{

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(camX, hf(camX, camZ) + heye, camZ,
			  lx, hf(camX, camZ) + heye, lz,
			  0.0f, 1.0f, 0.0f);

	draw_scene();

	displaytext("Use w, a, s, d to move", 10, 50);
	displaytext("Use arrows to turn around", 10, 30);
	displaytext("Press m to change drawing mode", 10, 10);

	// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	//Rotate camera to the right
	case GLUT_KEY_RIGHT:
		alpha -= 0.1;
		break;
	//Rotate camera to the left
	case GLUT_KEY_LEFT:
		alpha += 0.1;
		break;
	}
	lx = camX + sin(alpha);
	lz = camZ + cos(alpha);
}

void processKeys(unsigned char key, int xx, int yy)
{
	float dx, dz, rx, rz;
	float h = ((float)imageHeight - 1) / 2;
	float w = ((float)imageWidth - 1) / 2;
	switch (key)
	{
	//Change drawing mode
	case 'm':
		polMode = (polMode + 1) % 3;
		if (polMode == 0)
			glPolygonMode(GL_FRONT, GL_LINE);
		if (polMode == 1)
			glPolygonMode(GL_FRONT, GL_POINT);
		if (polMode == 2)
			glPolygonMode(GL_FRONT, GL_FILL);
		glutPostRedisplay();
		break;

	//Move Forward
	case 'w':

		dx = lx - camX;
		dz = lz - camZ;

		//restrict camera movement to the map size
		if ((camX + dx < h) && (camX + dx < w) && (camZ + dz < h) && (camZ + dz < w) &&
			(-h < camX + dx) && (-w < camX + dx) && (-h < camZ + dz) && (-w < camZ + dz))
		{
			camX += dx;
			camZ += dz;
			lx += dx;
			lz += dz;
		}
		break;

	//Move Backward
	case 's':

		dx = lx - camX;
		dz = lz - camZ;

		//restrict camera movement to the map
		if ((camX - dx < h) && (camX - dx < w) && (camZ - dz < h) && (camZ - dz < w) &&
			(-h < camX - dx) && (-w < camX - dx) && (-h < camZ - dz) && (-w < camZ - dz))
		{
			camX -= dx;
			camZ -= dz;
			lx -= dx;
			lz -= dz;
		}
		break;

	//Move Left
	case 'a':

		dx = lx - camX;
		dz = lz - camZ;

		//cross produt (dx,dy,dz)x(0,1,0) = (dz,0,-dx)
		rx = dz;
		rz = -dx;

		//restrict camera movement to the map
		if ((camX + rx < h) && (camX + rx < w) && (camZ + rz < h) && (camZ + rz < w) &&
			(-h < camX + rx) && (-w < camX + rx) && (-h < camZ - dz) && (-w < camZ - dz))
		{
			camX += rx;
			camZ += rz;
			lx += rx;
			lz += rz;
		}
		break;

	//Move Right
	case 'd':
		dx = lx - camX;
		dz = lz - camZ;
		//cross produt (dx,dy,dz)x(0,1,0) = (dz,0,-dx)
		rx = dz;
		rz = -dx;

		//restrict camera movement to the map
		if ((camX - dx < h) && (camX - dx < w) && (camZ - dz < h) && (camZ - dz < w) &&
			(-h < camX - dx) && (-w < camX - dx) && (-h < camZ - dz) && (-w < camZ - dz))
		{
			camX -= rx;
			camZ -= rz;
			lx -= rx;
			lz -= rz;
		}
		break;
	}
}

void fill_vertex_array(int th, int tw)
{
	std::vector<float> ps;
	glBindBuffer(GL_ARRAY_BUFFER, verteces);
	float h = ((float)th - 1) / 2;
	float w = ((float)tw - 1) / 2;
	for (int i = 1; i <= th; i++)
	{
		for (int j = 1; j <= tw; j++)
		{
			//Point a
			ps.push_back(-w + (j - 1));
			ps.push_back(height((i - 1), (j - 1)));
			ps.push_back(-h + (i - 1));
			//Point b
			ps.push_back(-w + (j - 1));
			ps.push_back(height(i, (j - 1)));
			ps.push_back(-h + i);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ps.size(), ps.data(), GL_STATIC_DRAW);
}

void init()
{

	verteces = 0;
	// 	Load the height map "terreno.jpg"
	unsigned int t;
	//ILubyte *imageData;
	ilGenImages(1, &t);
	ilBindImage(t);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring) "terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	imageHeight = ilGetInteger(IL_IMAGE_WIDTH);
	imageWidth = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();

	glGenBuffers(1, &verteces);
	// 	Build the vertex arrays
	fill_vertex_array(imageHeight, imageWidth);

	// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void spinDisplay(void)
{
	spin = spin + 0.05;
	if (spin > 360.0)
		spin = spin - 360.0;
	glutPostRedisplay();
}

int main(int argc, char **argv)
{

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutIdleFunc(spinDisplay);
	ilInit();

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	if (glewInit() != GLEW_OK)
	{
		printf("glew initialization failed!");
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT, GL_LINE);

	init();

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}
