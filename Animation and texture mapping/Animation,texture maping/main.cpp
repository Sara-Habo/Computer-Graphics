#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "glm.h"
#include <iostream>

GLfloat angle = 0.0; /* in degrees */
GLfloat angle2 = 0.0; /* in degrees */
static int shoulder1 = -0.0, elbow1 = 0, shouldery1 = 0, shoulder2 = 0.0, shouldery2 = 0.0, elbow2 = 0;
static int leg1 = 0, downleg1 = 0, leg2 = 0, downleg2 = 0, leg1p = 0, leg2p = 0;
static int fingerbase1[5] = { 0,0,0,0,0 };
static int fingerup1[5] = { 0,0,0,0,0 };
static int fingerbase2[5] = { 0,0,0,0,0 };
static int fingerup2[5] = { 0,0,0,0,0 };
#define PI 3.14

double eye[] = { 0, 0, 3 };
double center[] = { 0, 0, 2 };
double up[] = { 0, 1, 0 };

// Body Positions
static float xBody = -1.5;
static float yBody = -0.5;
static float zBody = 0.0;
static float B_oriantation = 90.0;
//ball
static float ballx = -0.4, bally = -1.3, ballz = 0;
// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 0.5, 5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5, -5.0, -2.0, 1.0 };

const char* modelname1 = "data3/football-goal.obj";
const char* modelname2 = "data3/ballonFoot.obj";
const char* modelname3 = "data3/quakingAspen.obj";


void drawmodel(char* filename)
{
	GLMmodel* model = glmReadOBJ(filename);
	glmUnitize(model);
	glmFacetNormals(model);
	glmVertexNormals(model, 90.0);
	glmScale(model, .15);
	glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}


//Makes the image into a texture, and returns the id of the texture

GLuint loadTexture(Image* image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);			 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
		0,							  //0 for now
		GL_RGB,					  //Format OpenGL uses for image
		image->width, image->height, //Width and height
		0,							  //The border of the image
		GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
									 //as unsigned numbers
		image->pixels);			  //The actual pixel data
	return textureId;						  //Returns the id of the texture
}

GLuint _textureId; //The id of the texture


//Initializes 3D rendering
void initRendering(const char* floorname, GLuint& textureID)
{
	Image* image = loadBMP(floorname);
	_textureId = loadTexture(image);
	delete image;
	// Turn on the power
	glEnable(GL_LIGHTING);
	// Flip light switch
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	// assign light parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties
	GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	glEnable(GL_NORMALIZE);
	//Enable smooth shading
	glShadeModel(GL_SMOOTH);
	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);
}




void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}
void turnLeft()
{
	double theta = -PI / 100;
	rotatePoint(up, theta, eye);
}

void turnRight()
{
	double theta = PI / 100;
	rotatePoint(up, theta, eye);
}

void moveUp()
{
	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 100, eye);
	rotatePoint(horizontal, PI / 100, up);
}


void moveDown()
{

	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 100, eye);
	rotatePoint(horizontal, -PI / 100, up);
}

void moveForward()
{

	double speed = 0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;
}

void moveBack()
{

	double speed = -0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;
}
void reset()
{
	double e[] = { 0.0, 0.0, 3.0 };
	double c[] = { 0.0, 0.0, 2.0 };
	double u[] = { 0.0, 1.0, 0.0 };
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}



void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(80.0, 1.0, 1.0, 20.0);
	//glMatrixMode(GL_MODELVIEW);
}




void display(void)
{

	glClearColor(209.0, 174.0, 36.0, 1.0);

	// Clear Depth and Color buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, -2, 1);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(5, -2, 1);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(5, -2, -1);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-5, -2, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//translate the whole scene
	glTranslatef(0.0, -0.5, 0.0);
	//model1 goal
	glPushMatrix();

	glTranslatef(1.8, -1.5, -.1);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glScalef(5.0, 15.0, 5.0);

	drawmodel((char*)modelname1);
	glPopMatrix();
	//model2 ball
	glPushMatrix();
	//glRotatef(-90, 0.0, 1.0, 0.0);
	glTranslatef((GLfloat)ballx, (GLfloat)bally, (GLfloat)ballz);
	glScalef(1.0, 1.0, 1.0);

	drawmodel((char*)modelname2);
	glPopMatrix();
	//tree1
	glPushMatrix();
	glTranslatef(0.0, 0.0, -1.0);
	glScalef(10.0, 10.0, 5.0);
	drawmodel((char*)modelname3);
	glPopMatrix();
	//tree2
	glPushMatrix();
	glTranslatef(2.0, 0.0, -1.5);
	glScalef(10.0, 10.0, 5.0);
	drawmodel((char*)modelname3);
	glPopMatrix();

	//tree3
	glPushMatrix();
	glTranslatef(-2.0, 0.0, -1.0);
	glScalef(10.0, 10.0, 5.0);
	drawmodel((char*)modelname3);
	glPopMatrix();
	//body
	glTranslatef((GLfloat)xBody, (GLfloat)yBody, (GLfloat)zBody);
	glRotatef((GLfloat)B_oriantation, 0.0, 1.0, 0.0);
	glScalef(0.15, 0.15, 0.2);


	glRotatef((GLfloat)angle2, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)angle, 0.0, 1.0, 0.0);
	glTranslatef(0, 2, 0);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glutWireSphere(0.4, 15, 16);
	glPopMatrix();
	glTranslatef(0, -2, 0);
	glPushMatrix();
	glScalef(2, 3, 0.5);
	glutWireCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();

	//shoulder of arm1:
	glTranslatef(1, 1.2, 0.0);
	glRotatef((GLfloat)shouldery1, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)shoulder1, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.0, 0.0);
	glPushMatrix(); //s=i,R1*R2*T1*Rs*Ts
	glScalef(1.0, 0.4, .5); //Ss
	glutWireCube(1.0);
	glPopMatrix(); //cm=R1*R2*T1*Rs*Ts ,s=i
	//elbow of arm1:
	glTranslatef(0.5, 0.0, 0.0); //Te1
	glRotatef((GLfloat)elbow1, 0.0, 0.0, 1.0); //Re 
	glTranslatef(0.5, 0.0, 0.0); //Te2
	glPushMatrix();
	glScalef(1.0, 0.4, 0.5);
	glutWireCube(1.0);
	glPopMatrix();


	glPopMatrix();

	//shoulder of arm2:
	glTranslatef(-1, 1.2, 0.0);
	glRotatef((GLfloat)shouldery2, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)shoulder2, 0.0, 0.0, 1.0);
	glTranslatef(-0.5, 0.0, 0.0);
	glPushMatrix(); //s=i,R1*R2*T1*Rs*Ts
	glScalef(1.0, 0.4, .5); //Ss
	glutWireCube(1.0);
	glPopMatrix(); //cm=R1*R2*T1*Rs*Ts ,s=i
	//elbow of arm2:
	glTranslatef(-0.5, 0.0, 0.0); //Te1
	glRotatef((GLfloat)elbow2, 0.0, 0.0, 1.0); //Re 
	glTranslatef(-0.5, 0.0, 0.0); //Te2
	glPushMatrix();
	glScalef(1.0, 0.4, 0.5);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();


	glPopMatrix();
	//glPopMatrix();
	//glPopMatrix();



	// first leg
	glPushMatrix();
	glTranslatef(1.0, -1.3, 0.0);
	glRotatef((GLfloat)leg1, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)leg1p, 0.0, 0.0, 1.0);
	glTranslatef(-0.3, -1.2, 0.0);
	glPushMatrix();
	glScalef(0.68, 2, 0.5);
	glutWireCube(1.0);
	glPopMatrix();


	//downleg1
	glPushMatrix();
	glTranslatef(1.0, -1.3, 0.0);
	glRotatef((GLfloat)downleg1, 1.0, 0.0, 0.0);
	glTranslatef(-1, -0.7, 0.0);
	glPushMatrix();
	glScalef(0.7, 2, 0.5);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(0.0, -1.3, 0.0);
	glScalef(0.8, 0.5, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();

	//leg2
	glPushMatrix();
	glTranslatef(1.0, -1.3, 0.0);
	glRotatef((GLfloat)leg2, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)leg2p, 0.0, 0.0, 1.0);
	glTranslatef(-1.2, -0.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glTranslatef(-1.53, -1.2, 0.0);
	glPushMatrix();
	glScalef(0.68, 2, 0.5);
	glutWireCube(1.0);
	glPopMatrix();

	//downleg2
	glPushMatrix();
	glTranslatef(1.0, -1.3, 0.0);
	glRotatef((GLfloat)downleg2, 1.0, 0.0, 0.0);
	glTranslatef(-1, -0.7, 0.0);
	glPushMatrix();
	glScalef(0.6, 2, 0.5);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(0.0, -1.3, 0.0);
	glScalef(0.8, 0.5, 1.0);
	glutSolidCube(1.0);

	glPopMatrix();
	glPopMatrix();


	glPopMatrix();
	glutSwapBuffers();

}
void floor_menu(int value)
{

	switch (value)
	{
	case '1':
		initRendering("images/grass.bmp", _textureId);
		break;
	case '2':
		initRendering("images/f1.bmp", _textureId);
		break;
	default:
		break;


	}

	glutPostRedisplay();
}

void main_menu(int value) {
	if (value == 0)
	{
		/*	printf(":%d \n", shoulder1);
			printf("ybody:%d \n", yBody);
			printf("zbody:%d \n", zBody);
			printf("zbody:%d \n", shoulder1);*/
		printf("%d,%d,%d,%d,%d,%d,%d,%d\n", shoulder1, shoulder2, elbow1, elbow2, leg1, leg2, downleg1, downleg2);
	}
	else if (value == 1) {
		for (int i = 0; i < 3; i++)
			printf("eye[%d]=%f \n", i, eye[i]);

	}
}

void attachMenu()
{
	glutCreateMenu(floor_menu);
	glutAddMenuEntry("floars", 0);
	glutAddMenuEntry("grass", '1');
	glutAddMenuEntry("Brown", '2');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void setPoses(int frame) {
	//XBody,Ybody,Zbody,XBall,shoulder1,shoulder2,elbow1,elbow2,leg1,leg2,downleg1,downleg2
	int poses[7][12] = {
		{-2.0 , 0.0 ,-0.0 , -0.4 , -90.0 , 90.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
		{-2.0 , 0.0 , -0.0 , -0.4 , -90.0 , -90.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
		{-2.0 , 0.0 , -0.0 , -0.4 , -90.0 , 90.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
		{-2.0, 0.0 , -0.0 , -0.4 , -90.0 , 90.0 , 0.0 , 0.0 ,-20.0 , 0.0 , 20.0 ,0.0},
		{-1.5 , 0.0 , -0.0 , -0.4 , -90.0 , 90.0 , 0.0 , 0.0, -20 , 20, 0.0, 20},
		{-1.5 , 0.0 , -0.0 ,-0.4 , -90.0 , 90.0 , 0.0 , 0.0, -20.0 , 20.0, 0.0, 20},
		{-1.5 + 1.0 , 0.0 , -0.0 ,-0.4 , -90.0 , 90.0 , 0.0 , 0.0, 0.0 , 20.0, 0.0, 20},
		

	};
	xBody = poses[frame][0];
	yBody += poses[frame][1];
	zBody += poses[frame][2];
	ballx = poses[frame][3];
	shoulder1 = poses[frame][4];
	shoulder2 = poses[frame][5];
	elbow1 = poses[frame][6];
	elbow2 = poses[frame][7];
	leg1 = poses[frame][8];
	leg2 = poses[frame][9];
	downleg1 = poses[frame][10];
	downleg2 = poses[frame][11];
}
static int f = 0;
static int displayed_frame = 0;
void timer(int value) {
	f = f % 7;
	setPoses(f);
	f++;
	//displayed_frame++;
	glutPostRedisplay();
	//if (displayed_frame< 7) {
	glutTimerFunc(800, timer, 0);
	//}
}


void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		turnLeft();
		break;
	case GLUT_KEY_RIGHT:
		turnRight();
		break;
	case GLUT_KEY_UP:
		if (center[1] <= 1.5)
			moveUp();
		break;
	case GLUT_KEY_DOWN:
		if (center[1] >= -1.5)
			moveDown();
		break;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case '+':
		moveForward();
		break;
	case '-':
		moveBack();
		break;

	case '0':
		reset();
		break;
		//move body in x
	case '1':
		xBody += 0.5;
		glutPostRedisplay();
		break;

		//move body in -x
	case '2':
		xBody -= 0.5;

		glutPostRedisplay();
		break;

	case '3':
		yBody += 0.3;
		glutPostRedisplay();
		break;


	case '4':
		yBody -= 0.3;

		glutPostRedisplay();
		break;

	case '5':
		zBody = +0.5;
		glutPostRedisplay();
		break;


	case '6':

		zBody = -0.5;
		glutPostRedisplay();
		break;

	case '7':
		B_oriantation += 60;
		glutPostRedisplay();
		break;
	case '8':
		B_oriantation -= 60;
		glutPostRedisplay();
		break;

	case 'b':
		if (shouldery1 == 70) { return; }
		shouldery1 = (shouldery1 + 5);
		glutPostRedisplay();
		break;
	case 'B':
		if (shouldery1 == -160) { return; }
		shouldery1 = (shouldery1 - 5);
		glutPostRedisplay();
		break;
	case 'n':
		if (shouldery2 == 160) { return; }
		shouldery2 = (shouldery2 + 5);
		glutPostRedisplay();
		break;
	case 'N':
		if (shouldery2 == -70) { return; }
		shouldery2 = (shouldery2 - 5);
		glutPostRedisplay();
		break;

	case 's':
		if (shoulder1 == 90) { return; }
		shoulder1 = (shoulder1 + 5);
		glutPostRedisplay();
		break;
	case 'S':
		if (shoulder1 == -90) { return; }
		shoulder1 = (shoulder1 - 5) % 360;
		glutPostRedisplay();
		break;

	case 'e':
		if (elbow1 == 90) { return; }
		elbow1 = (elbow1 + 5);
		glutPostRedisplay();
		break;
	case 'E':
		if (elbow1 == 0) { return; }
		elbow1 = (elbow1 - 5);
		glutPostRedisplay();
		break;

	case 'l':
		if (shoulder2 == 90) { return; }
		shoulder2 = (shoulder2 + 5);
		glutPostRedisplay();
		break;
	case 'L':
		if (shoulder2 == -90) { return; }
		shoulder2 = (shoulder2 - 5) % 360;
		glutPostRedisplay();
		break;

	case 'r':
		if (elbow2 == 90) { return; }
		elbow2 = (elbow2 + 5);
		glutPostRedisplay();
		break;
	case 'R':
		if (elbow2 == 0) { return; }
		elbow2 = (elbow2 - 5);
		glutPostRedisplay();
		break;

	case 'z':
		if (leg1 == 60) { return; }
		leg1 = (leg1 + 5);
		glutPostRedisplay();
		break;
	case 'Z':
		if (leg1 == -90) { return; }
		leg1 = (leg1 - 5);
		glutPostRedisplay();
		break;

	case 'x':
		if (downleg1 == 60) { return; }
		downleg1 = (downleg1 + 5);
		glutPostRedisplay();
		break;
	case 'X':
		if (downleg1 == 0) { return; }
		downleg1 = (downleg1 - 5);
		glutPostRedisplay();
		break;

	case 'c':
		if (leg2 == 60) { return; }
		leg2 = (leg2 + 5);
		glutPostRedisplay();
		break;
	case 'C':
		if (leg2 == -90) { return; }
		leg2 = (leg2 - 5);
		glutPostRedisplay();
		break;

	case 'v':
		if (downleg2 == 90) { return; }
		downleg2 = (downleg2 + 5);
		glutPostRedisplay();
		break;
	case 'V':
		if (downleg2 == 0) { return; }
		downleg2 = (downleg2 - 5);
		glutPostRedisplay();
		break;

	case 'a':
		if (leg1p == 90) { return; }

		else
		{
			leg1p = (leg1p + 5);
			glutPostRedisplay();
		}
		break;
	case 'A':
		if (leg1p == 0) { return; }

		else {
			leg1p = (leg1p - 5);
			glutPostRedisplay();
		}
		break;

	case 'q':
		if (leg2p == 90) { return; }

		else
		{
			leg2p = (leg2p - 5);
			glutPostRedisplay();
		}
		break;
	case 'Q':
		if (leg2p == 0) { return; }

		else {
			leg2p = (leg2p + 5);
			glutPostRedisplay();
		}
		break;


	case 27:
		exit(0);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Animation");
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(100, 100);
	init();
	attachMenu();
	glutTimerFunc(0, timer, 0);
	initRendering("images/f1.bmp", _textureId);
	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialKeys);



	glutMainLoop();
	return 0;
}
