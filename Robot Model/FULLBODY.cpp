#include <stdlib.h>
#include <GL/glut.h>

#include <math.h>
#include <cmath>


int moving, startx, starty;
GLfloat angle = 0.0; /* in degrees */
GLfloat angle2 = 0.0; /* in degrees */
static int shoulder1 = 0, elbow1 = 0, shoulder2 = 0, elbow2 = 0;
static int leg1 = 0, downleg1 = 0, leg2 = 0, downleg2 = 0, leg1p = 0, leg2p = 0;
static int fingerbase1[5] = { 0,0,0,0,0 };
static int fingerup1[5] = { 0,0,0,0,0 };
static int fingerbase2[5] = { 0,0,0,0,0 };
static int fingerup2[5] = { 0,0,0,0,0 };
double eye[3] = { 0,0,2 };
double center[3] = { 0,0,0 };

double up[3] = { 0,1,0 };

#define PI 3.14



void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
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
void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}
void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, 1.0, 1.0, 20.0);
	//glMatrixMode(GL_MODELVIEW);
}

void reset()
{
	double e[] = { 0.0, 0.0, 2 };
	double c[] = { 0.0, 0.0, 1.0 };
	double u[] = { 0.0, 1.0, 0.0 };
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}

void DrawFingerFlange1(float y, float z, float angle, int flag) {
	if (flag == 0) {
		glTranslatef(0.5, y, z);
		glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
		glTranslatef(0.1, 0.0, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.1, 0.1);
		glutWireCube(1);
		glPopMatrix();

	}
	else if (flag == 1) {
		glTranslatef(0.1, 0.0, 0.0);
		glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
		glTranslatef(0.1, 0.0, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.1, 0.1);
		glutWireCube(1);
		glPopMatrix();

	}
	if (flag == 2) {
		glTranslatef(-0.5, y, z);
		glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
		glTranslatef(-0.1, 0.0, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.1, 0.1);
		glutWireCube(1);
		glPopMatrix();

	}
	if (flag == 3) {
		glTranslatef(-0.1, y, z);
		glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
		glTranslatef(-0.1, 0.0, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.1, 0.1);
		glutWireCube(1);
		glPopMatrix();

	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);



	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
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
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();


	//draw base of finger1
	DrawFingerFlange1(0.1, -0.2, fingerbase1[0], 0);

	//draw up of finger 1
	DrawFingerFlange1(0.0, 0.0, fingerup1[0], 1);
	glPopMatrix();
	//draw base of finger2
	DrawFingerFlange1(0.1, -0.05, fingerbase1[1], 0);

	//draw up of finger 2
	DrawFingerFlange1(0.0, 0.0, fingerup1[1], 1);
	glPopMatrix();
	//draw base of finger3
	DrawFingerFlange1(0.1, 0.1, fingerbase1[2], 0);

	//draw up of finger 3 
	DrawFingerFlange1(0.0, 0.0, fingerup1[2], 1);
	glPopMatrix();
	//draw base of finger4
	DrawFingerFlange1(0.1, 0.25, fingerbase1[3], 0);

	//draw up of finger 4 
	DrawFingerFlange1(0.0, 0.0, fingerup1[3], 1);
	glPopMatrix();
	
	//draw base of finger5
	DrawFingerFlange1(-0.1, -0.2, fingerbase1[4], 0);

	//draw up of finger 5 
	DrawFingerFlange1(0.0, 0.0, fingerup1[4], 1);



	glPopMatrix();

	//shoulder of arm2:
	glTranslatef(-1, 1.2, 0.0);
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
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();

	//draw base of finger1
	DrawFingerFlange1(0.1, -0.2, fingerbase2[0], 2);
	//draw up of finger 1
	DrawFingerFlange1(0.0, 0.0, fingerup2[0], 3);

	glPopMatrix();
	//draw base of finger2
	DrawFingerFlange1(0.1, -0.05, fingerbase2[1], 2);

	//draw up of finger 2
	DrawFingerFlange1(0.0, 0.0, fingerup2[1], 3);
	glPopMatrix();
	//draw base of finger3
	DrawFingerFlange1(0.1, 0.1, fingerbase2[2], 2);

	//draw up of finger 3 
	DrawFingerFlange1(0.0, 0.0, fingerup2[2], 3);
	glPopMatrix();
	//draw base of finger4
	DrawFingerFlange1(0.1, 0.25, fingerbase2[3], 2);

	//draw up of finger 4 
	DrawFingerFlange1(0.0, 0.0, fingerup2[3], 3);
	glPopMatrix();
	//draw base of finger5
	DrawFingerFlange1(-0.1, -0.2, fingerbase2[4], 2);

	//draw up of finger 5 
	DrawFingerFlange1(0.0, 0.0, fingerup2[4], 3);



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

void lookright(void) {
	rotatePoint(up, PI / 20, eye);
}

void lookleft(void) {
	rotatePoint(up, -PI / 20, eye);
}

void lookup(void) {
	double direction[3];
	double horizontal[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];

	direction[2] = center[2] - eye[2];
	crossProduct(up, direction, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 20, eye);
	rotatePoint(horizontal, PI / 20, up);
	normalize(up);

}
void lookdown(void) {
	double direction[3];
	double horizontal[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];

	direction[2] = center[2] - eye[2];
	crossProduct(up, center, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 20, center);
	rotatePoint(horizontal, -PI / 20, up);
	normalize(up);

}

void forward() {
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	double speed = 0.1;
	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;
}

void backward() {
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	double speed = 0.1;
	eye[0] -= direction[0] * speed;
	eye[1] -= direction[1] * speed;
	eye[2] -= direction[2] * speed;

	center[0] -= direction[0] * speed;
	center[1] -= direction[1] * speed;
	center[2] -= direction[2] * speed;

}
void keySpecial(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		lookleft();
		break;
	case GLUT_KEY_RIGHT:
		lookright();
		break;
	case GLUT_KEY_UP:
		lookup();
		break;
	case GLUT_KEY_DOWN:
		lookdown();
		break;
	}
	glutPostRedisplay();



}
void keyboard(unsigned char key, int x, int y)
{


	switch (key) {
	case '+':
		forward();
		glutPostRedisplay();
		break;
	case '-':
		backward();
		glutPostRedisplay();
		break;
	case '0':
		reset();
		glutPostRedisplay();
		break;
		//moving fingerbase1
	case '1':
		if (fingerbase1[0] == 0 & fingerbase1[1] == 0 & fingerbase1[2] == 0 & fingerbase1[3] == 0 & fingerbase1[4] == 0) {
			return;
		}
		fingerbase1[0] += 5;
		fingerbase1[1] += 5;
		fingerbase1[2] += 5;
		fingerbase1[3] += 5;
		fingerbase1[4] -= 5;
		glutPostRedisplay();
		break;

		//moving fingerbase1
	case '2':
		if (fingerbase1[0] == -90 & fingerbase1[1] == -90 & fingerbase1[2] == -90 & fingerbase1[3] == -90 & fingerbase1[4] == 90) {
			return;
		}
		fingerbase1[0] -= 5;
		fingerbase1[1] -= 5;
		fingerbase1[2] -= 5;
		fingerbase1[3] -= 5;
		fingerbase1[4] += 5;

		glutPostRedisplay();
		break;

	case '3':
		if (fingerup1[0] == 0 & fingerup1[1] == 0 & fingerup1[2] == 0 & fingerup1[3] == 0 & fingerup1[4] == 0) {
			return;
		}
		fingerup1[0] += 5;
		fingerup1[1] += 5;
		fingerup1[2] += 5;
		fingerup1[3] += 5;
		fingerup1[4] -= 5;
		glutPostRedisplay();
		break;


	case '4':
		if (fingerup1[0] == -90 & fingerup1[1] == -90 & fingerup1[2] == -90 & fingerup1[3] == -90 & fingerup1[4] == 90) {
			return;
		}
		fingerup1[0] -= 5;
		fingerup1[1] -= 5;
		fingerup1[2] -= 5;
		fingerup1[3] -= 5;
		fingerup1[4] += 5;

		glutPostRedisplay();
		break;
                //moving fingerbase2
	case '5':
		if (fingerbase2[0] == 0 & fingerbase2[1] == 0 & fingerbase2[2] == 0 & fingerbase2[3] == 0 & fingerbase2[4] == 0) {
			return;
		}
		fingerbase2[0] -= 5;
		fingerbase2[1] -= 5;
		fingerbase2[2] -= 5;
		fingerbase2[3] -= 5;
		fingerbase2[4] += 5;
		glutPostRedisplay();
		break;

		
	case '6':
		if (fingerbase2[0] == 90 & fingerbase2[1] == 90 & fingerbase2[2] == 90 & fingerbase2[3] == 90 & fingerbase2[4] == -90) {
			return;
		}
		fingerbase2[0] += 5;
		fingerbase2[1] += 5;
		fingerbase2[2] += 5;
		fingerbase2[3] += 5;
		fingerbase2[4] -= 5;

		glutPostRedisplay();
		break;

	case '7':
		if (fingerup2[0] == 0 & fingerup2[1] == 0 & fingerup2[2] == 0 & fingerup2[3] == 0 & fingerup2[4] == 0) {
			return;
		}
		fingerup2[0] -= 5;
		fingerup2[1] -= 5;
		fingerup2[2] -= 5;
		fingerup2[3] -= 5;
		fingerup2[4] += 5;
		glutPostRedisplay();
		break;


	case '8':
		if (fingerup2[0] == 90 & fingerup2[1] == 90 & fingerup2[2] == 90 & fingerup2[3] == 90 & fingerup2[4] == -90) {
			return;
		}
		fingerup2[0] += 5;
		fingerup2[1] += 5;
		fingerup2[2] += 5;
		fingerup2[3] += 5;
		fingerup2[4] -= 5;

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
}


static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
}
static void motion(int x, int y)
{
	if (moving) {
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutMouseFunc(mouse);
	//glutMotionFunc(motion);

	glutDisplayFunc(display); glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keySpecial);
	glutMainLoop();
	return 0;
}

