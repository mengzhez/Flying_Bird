#include<iostream>
#include "glut.h"
#include "torus.h"

using namespace std;

void Torus :: drawTorus()
{
	glPushMatrix();
	glTranslatef(_ctr[0], _ctr[1], _ctr[2]);
	glRotatef(90, 0, 1, 0);
	glutSolidTorus(torusRadius, torusRadius + 0.8, 20, 20);
	glPopMatrix();
}

float Torus :: getCenter(int i)
{
	if (i >= 0 && i <= 3)
		return _ctr[i];
	else 
		return 0;
}

void Torus :: setCenter(float val, int i)
{
	_ctr[i] = val;
}

bool Torus :: isVisible()
{
	return _v;
}

void Torus :: setVisibility(bool v)
{
	_v = v;
}