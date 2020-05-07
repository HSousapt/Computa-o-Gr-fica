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

#define ANG2RAD M_PI/180.0 

#define COWBOYS 8
#define RAIO_COWBOYS 5
#define INDIOS 16
#define RAIO_INDIOS 25
#define ARVORES 500
#define STEP_COWBOY 1.0f
#define STEP_INDIO 0.5f


float step = 0.0;

float height = 2.0f;
float x = 0.0f;
float z = 0.0f;

int frame = 0, timebase = 0;

int heye = 5;
float lx = 1, lz = 1;
float camX = 00, camY = 00, camZ = 00;

int alpha = 0, beta = 45, r = 50;
float theta = 0;

unsigned int texture;
int imageWidth;
unsigned char *imageData;


float brown[4] = { 1.0f,1.0f, 0.5f, 1.0f };
float lbrown[4] = { 0.6f, 0.1f, 0.5f, 1.0f };
float green2[4] = {0.3f, 1.0f, 0.2, 1.0f};
float green3[4] = {0.5f, 0.5f, 0.2f, 1.0f};

// vectors to store vertex data temporarily
std::vector<float> position, normal, texCoord;
GLuint buffers[3];

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


float h(int i, int j) {

	return(imageData[i*imageWidth + j] * 0.5f);
}


float h2(float x, float z) {


	int x1, x2, z1, z2;
	float xGrid, zGrid;
	float fracX, fracZ, ha, hb;

	xGrid = x + imageWidth / 2;
	zGrid = z + imageWidth / 2;

	x1 = (int)floor(xGrid);
	x2 = x1 + 1;
	fracX = xGrid - x1;

	z1 = (int)floor(zGrid);
	z2 = z1 + 1;
	fracZ = zGrid - z1;

	ha = h(x2, z1) * fracX + (1 - fracX) * h(x1, z1);
	hb = h(x2, z2) * fracX + (1 - fracX) * h(x1, z2);

	return(hb * fracZ + (ha * (1 - fracZ)));
}


void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


void computeNormal(int i, int j) {

	// fill the normal vector with the normal for vertex at grid location (i,j)
	float v1[3], v2[3], v1v2[3], normV1V2[3];
	v1[0] = i - i;
	v1[1] = h(i, j + 1) - h(i, j - 1);
	v1[2] = (j + 1) - (j - 1);

	v2[0] = (i + 1) - (i - 1);
	v2[1] = h(i + 1, j) - h(i - 1, j);
	v2[2] = j - j;

	//Realizar o produto entre os vetores v1 e v2
	cross(v2, v1, v1v2);


	normalize(v1v2);
	
	for (int i = 0; i < 3; i++) {
		normal.push_back(v1v2[i]);
	}

}

void prepareTerrain() {

	for (int i = 1; i < imageWidth - 2; i++) {
		for (int j = 1; j < imageWidth - 1; j++) {

			computeNormal(i + 1, j);
			
			// fill texCoord vector with the texture coordinates for grid location (i+1,j)

			texCoord.push_back(i + 1);
			texCoord.push_back(j);

			position.push_back(i - imageWidth*0.5f + 1);
			position.push_back(h(i + 1, j));
			position.push_back(j - imageWidth*0.5f);

			computeNormal(i, j);

			texCoord.push_back(i);
			texCoord.push_back(j);

			// fill texCoord vector with the texture coordinates for grid location (i,j)

			position.push_back(i - imageWidth*0.5f);
			position.push_back(h(i, j));
			position.push_back(j - imageWidth*0.5f);
		}
	}
	glGenBuffers(3, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), &(position[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), &(normal[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(float), &(texCoord[0]), GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}


void renderTerrain() {

	float white[4] = { 0.7f, 1.0f, 0.7f, 1.0f };
	float wt[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wt);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	for (int i = 1; i < imageWidth - 2; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, (imageWidth - 2) * 2 * i, (imageWidth - 2) * 2);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

}


void drawTree() {

	glPushMatrix();
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, brown);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lbrown);
	glutSolidCone(0.25f, 4, 5, 1);

	float randg = rand() * 0.5f / RAND_MAX;
	float green[4] = { 0.0f, (0.5f + randg), 0.0f, 1.0f };
	float lgreen[4] = { 0.0f, (0.5f + randg), 0.0f, 0.5f };

    int x=rand()%3;
    switch (x) {
        case 0:
 			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
            break;
        case 1:
            glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,green2);
            break;
        case 2:
            glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,green3);
            break;
            
        default:
            break;
    }

	glMaterialfv(GL_FRONT, GL_SPECULAR, lgreen);
	glMaterialf(GL_FRONT, GL_SHININESS, 16);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glutSolidCone(2.0f, 5.0f, 10.0f, 10.0f);
	glPopMatrix();
}


void placeTrees() {

	float r = 35.0;
	float alpha;
	float rr;
	float x,z;

	srand(31457);
	int arvores = 0;

	while (arvores < ARVORES) {

		rr = rand() * 150.0/ RAND_MAX;
		alpha = rand() * 6.28 / RAND_MAX;

		x = cos(alpha) * (rr + r);
		z = sin(alpha) * (rr + r);

		if (fabs(x) < 100 && fabs(z) < 100) {

			glPushMatrix();
			glTranslatef(x, h2(x, z),z);
			drawTree();
			glPopMatrix();
			arvores++;
		}
	}
}


void drawDonut() {

	float purple[4] = { 1.0f,0.0f,1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
	glPushMatrix();
	glTranslatef(0.0,0.5,0.0);
	glutSolidTorus(0.5,1.25,8,16);
	glPopMatrix();
}


void drawIndios() {

	float angulo;
	float red[4] = { 1.0f,0.0f,0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
	for (int i = 0; i < INDIOS; i++) {
		
		angulo = i * 360.0/INDIOS + step * STEP_INDIO;
		glPushMatrix();
		glRotatef(angulo,0.0,1.0,0.0);
		glTranslatef(0.0,0.0,RAIO_INDIOS);
		glutSolidTeapot(1);
		glPopMatrix();
	}
}


void drawCowboys() {

	float angulo;
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);

	for (int i = 0; i < COWBOYS; i++) {

		angulo = i * 360.0/COWBOYS + step * STEP_COWBOY;
		glPushMatrix();
		glRotatef(-angulo,0.0,1.0,0.0);
		glTranslatef(RAIO_COWBOYS,0.0,0.0);
		glutSolidTeapot(1);
		glPopMatrix();
	}
}


void drawScene() {

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	renderTerrain();
	placeTrees();
	drawDonut();
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	// move teapots up so that they are placed on top of the ground plane
	glTranslatef(0.0,1.0,0.0);
	drawCowboys();
	drawIndios();
	glPopMatrix();
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
	//glColor3f(0.0f, 0.0f, 1.0f);
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


void renderScene(void) {

	float diff[3]={1,1,1};
	float amb[3]={0.3,0.3,0.3};
	float wt[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glClearColor(0.5f,0.6f,1.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(camX, h2(camX, camZ) + heye, camZ,
			  lx, h2(camX, camZ) + heye, lz,
			  0.0f, 1.0f, 0.0f);

	float lpos[4] = { 1,1,1,0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

	drawScene();

	displaytext("Use w, a, s, d to move", 10, 50);
	displaytext("Use arrows to turn around", 10, 30);

	show_fps();

	step++;

// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		theta -= 0.1;
		break;

	case GLUT_KEY_LEFT:
		theta += 0.1;
		break;
	}

	lx = camX + sin(theta);
	lz = camZ + cos(theta);

}


void processKeys(unsigned char key, int xx, int yy) {
	float dx, dz, rx, rz;
	switch (key)
	{
	case 'w':
		dx = lx - camX;
		dz = lz - camZ;
		
		if(camX+dx<127&&camX+dx>-127 && camZ+dz<128&&camZ+dz>-127){
		
		camX += dx;
		camZ += dz;
		
		lx += dx;
		lz += dz;}
		break;

	case 's':
		dx = lx - camX;
		dz = lz - camZ;
		if(camX-dx<127&&camX-dx>-127 && camZ-dz<128&&camZ-dz>-127){
		camX -= dx;
		camZ -= dz;
		lx -= dx;
		lz -= dz;}
		break;

	case 'a':
		dx = lx - camX;
		dz = lz - camZ;

		//cross produt (dx,dy,dz)x(0,1,0) = (dz,0,-dx)
		rx = dz;
		rz = -dx;
		if(camX+rx<127&&camX+rx>-127 && camZ+rz<128&&camZ+rz>-127){
		camX += rx;
		camZ += rz;
		lx += rx;
		lz += rz;}
		break;

	case 'd':
		dx = lx - camX;
		dz = lz - camZ;

		//cross produt (dx,dy,dz)x(0,1,0) = (dz,0,-dx)
		rx = dz;
		rz = -dx;
		if(camX-rx<127&&camX-rx>-127 && camZ-rz<127&&camZ-rz>-127){
		camX -= rx;
		camZ -= rz;
		lx -= rx;
		lz -= rz;}
		break;
	}

}



void loadTexture() {

	unsigned int t, tw, th;
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)"relva.jpg");
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, &texture);

	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
}


void init() {

	alpha = 0.0;
	beta = 0.0;

	unsigned int ima[1];

	ilInit();
	ilGenImages(1, ima);
	ilBindImage(ima[0]);
	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	imageWidth = ilGetInteger(IL_IMAGE_HEIGHT);
	imageData = ilGetData();

	prepareTerrain();

	loadTexture();
	
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}


int main(int argc, char **argv) {

// init GLUT and window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		

// callback registration 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);



#ifndef __APPLE__	
	glewInit();
#endif	

	init();

// GLUT's main cicle
	glutMainLoop();
	
	return 0;
}

