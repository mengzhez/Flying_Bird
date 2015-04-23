#include<iostream>
#include "glut.h"
#include "bird.h"

using namespace std;

void Bird :: drawBird()
{
	// Body
	glPushMatrix();
	glScalef(1.0, 0.9, 1.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.4, 20, 20);
	glPopMatrix();

	// Feather
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.4);
	glRotatef(45, 0, 1, 0);
	glScalef(2.0, 0.3, 0.6);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.1, 0.0, 0.35);
	glRotatef(30, 0, 1, 0);
	glScalef(1.8, 0.3, 0.4);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	// Eyes
	glPushMatrix();
	glTranslatef(0.35, 0.05, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.08, 10, 10);
	glTranslatef(0.055, -0.01, 0.01);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.035, 5, 5);
	glTranslatef(-0.05, -0.09, -0.01);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.08, 10, 10);
	glTranslatef(0.055, 0.01, 0.01);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.035, 5, 5);
	glPopMatrix();

	// Eyebrows
	glPushMatrix();
	glTranslatef(0.4, 0.08, 0.1);
	glRotatef(30, 1, 0, 0);
	glRotatef(-30, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(1.1, 0.2, 0.4);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.1, 5, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.4, -0.08, 0.1);
	glRotatef(-30, 1, 0, 0);
	glRotatef(-30, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	glScalef(1.1, 0.2, 0.4);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.1, 5, 5);
	glPopMatrix();

	// Tail
	glPushMatrix();
	glTranslatef(-0.35, 0.0, -0.2);
	glRotatef(90, 1, 0, 0);
	glScalef(1.6, 0.2, 0.4);
	glutSolidSphere(0.1, 5, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.35, 0.0, -0.2);
	glRotatef(30, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(1.6, 0.2, 0.4);
	glutSolidSphere(0.1, 5, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.35, 0.0, -0.2);
	glRotatef(-30, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(1.6, 0.2, 0.4);
	glutSolidSphere(0.1, 5, 5);
	glPopMatrix();

	// Beak
	glPushMatrix();
	glTranslatef(0.35, 0.0, -0.1);
	glRotatef(90, 0, 1, 0);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidCone(0.08, 0.18, 10, 10);
	glPopMatrix();

	// Import the OBJ model
	/*char* name = "data/angry_birds.obj";
	GLMmodel* birdModel = glmReadOBJ(name);
	if (!birdModel) exit(0);
	glmUnitize(birdModel);
	glmFacetNormals(birdModel);
	glmVertexNormals(birdModel, 90.0);
	glmDraw(birdModel, GLM_MATERIAL | GLM_SMOOTH);*/
}

float Bird :: getMatrix(int i, int j)
{
	if (i >= 0 && i <= 4 && j >= 0 && j <= 4)
		return _m[i][j];
	else
		return 0;
}

void Bird :: setMatrix(float val, int i, int j)
{
	if (i >= 0 && i <= 4 && j >= 0 && j <= 4)
		_m[i][j] = val;
}

void Bird :: loadMatrix()
{
	glMultMatrixf(*_m);
}

void Bird :: saveMatrix()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, *_m);
}

