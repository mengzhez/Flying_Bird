#include<iostream>
#include "glut.h"

using namespace std;

const float torusRadius = 0.3;

// Torus class
class Torus
{
private:
	float _ctr[3];     // Center
	bool _v;           // Visibility

public:
	Torus() 
	{
		_v = true;
	}

	~Torus()
	{}

	// Draw torus
	void drawTorus();

	// Get the center
	float getCenter(int i);

	// Set the center
	void setCenter(float val, int i);

	// Is visible
	bool isVisible();

	// Set to be invisible
	void setVisibility(bool v);
};