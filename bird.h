#include<iostream>
#include "glut.h"

using namespace std;

// Bird class
class Bird
{
private:

	float _ctr[3];     // Center
	float _m[4][4];    // Transform matrix

public:
	// Bird constructor
	Bird(float im[][4]) 
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				_m[i][j] = im[i][j];
			}
		}
	}

	~Bird()
	{}

	// Draw Bird
	void drawBird();

	// Get matrix element
	float getMatrix(int i, int j);

	// Set matrix element
	void setMatrix(float val, int i, int j);

	// Load transform matrix
	void loadMatrix();

	// Save transform matrix
	void saveMatrix();
};