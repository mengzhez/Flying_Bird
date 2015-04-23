#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <MMSystem.h>

#include "glut.h"
#include "glm.h"
#include "bird.h"
#include "torus.h"

#pragma comment(lib,"winmm.lib")

using namespace std;

// Constants
const int timerInterval = 10;

// Global Variables
static GLint windowHeight = 660;
static GLint windowWidth = 880;
float im[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
bool timeSwitch, startSwitch, winSwitch = false;
bool upSwitch, downSwitch, leftSwitch, rightSwitch = false;
float cameraX = 0.8;
float cameraZ = 0.4;

float speed = 0.03;
int torusCount = 0;
int currentTorus = 0;
float time = 0;
int timeCount = 0;
int difficulty = 1;
unsigned char* image = NULL;
int iWidth;
int iHeight;

// Functions
void drawAxis(double length);
void drawNormalString(string str, float x, float y, float z);
void moveBird();
void restart();
void redisplay(int value);

void setupSkybox();
void setupAxis();
void setupScoreAndTime();
void setupTorus();
void setupNavArrow();

void resetView();

Bird bird = Bird(im);
Torus torus[10];

// Draw axis
void drawAxis(double length)
{
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, length);
	glEnd();
	glTranslated(0, 0, length - 0.2);
	glutSolidCone(0.04, 0.2, 12, 9);
	glPopMatrix();
}

// Draw string
void drawNormalString(string str, float x, float y, float z) 
{
	glDisable(GL_LIGHTING);
	glPointSize(4.0);
	for(int i = 0; i < str.size(); i++) 
	{
		int w = glutBitmapWidth(GLUT_BITMAP_9_BY_15, str[i]);
		glRasterPos3f(x, y - i * 0.04, z);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}

	glEnable(GL_LIGHTING);
}

// Move bird
void moveBird()
{
	glPushMatrix();
	glLoadIdentity();
	bird.loadMatrix();
	glTranslatef(speed, 0, 0);
	bird.saveMatrix();
	glPopMatrix();

	if(upSwitch)
	{
		glPushMatrix();
		glLoadIdentity();
		bird.loadMatrix();
		glRotatef(-3, 0, 1, 0);
		bird.saveMatrix();
		glPopMatrix();
	}

	if(downSwitch)
	{
		glPushMatrix();
		glLoadIdentity();
		bird.loadMatrix();
		glRotatef(3, 0, 1, 0);
		bird.saveMatrix();
		glPopMatrix();
	}

	if(leftSwitch)
	{
		glPushMatrix();
		glLoadIdentity();
		bird.loadMatrix();
		glRotatef(3, 0, 0, 1);
		bird.saveMatrix();
		glPopMatrix();
	}

	if(rightSwitch)
	{
		glPushMatrix();
		glLoadIdentity();
		bird.loadMatrix();
		glRotatef(-3, 0, 0, 1);
		bird.saveMatrix();
		glPopMatrix();
	}

	// Check the visibility of torus
	for(int i = 0; i < 10; i++)
	{
		float range = 0.8;
		if(abs(torus[i].getCenter(0)-bird.getMatrix(3, 0))<range && abs(torus[i].getCenter(1)-bird.getMatrix(3, 1))<range && abs(torus[i].getCenter(2)-bird.getMatrix(3, 2))<range)
		{
			if(torus[i].isVisible())
			{
				//PlaySound("data/music/coin.wav", NULL, SND_FILENAME | SND_ASYNC);
				torus[i].setVisibility(false);
				torusCount++;
			}
		}
	}
}

void restart()
{
	time = 0;
	torusCount = 0;
	for(int i=0; i<10; i++)
		torus[i].setVisibility(true);

	currentTorus = 0;
	glPushMatrix();
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			bird.setMatrix(im[i][j], i, j);
		}
	}
	glPopMatrix();
}

/************** Keyboard ***************/
// Special keyboard function
void mySpecialKeyboard(int key, int mouseX, int mouseY)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		upSwitch = true;
		break;
	case GLUT_KEY_DOWN:
		downSwitch = true;
		break;
	case GLUT_KEY_LEFT:
		leftSwitch = true;
		break;
	case GLUT_KEY_RIGHT: 
		rightSwitch = true;
		break;
	default: break;
	}
}

// Special keyboard up function
void mySpecialKeyboardUp(int key, int mouseX, int mouseY)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		upSwitch = false;
		break;
	case GLUT_KEY_DOWN:
		downSwitch = false;
		break;
	case GLUT_KEY_LEFT: 
		leftSwitch = false;
		break;
	case GLUT_KEY_RIGHT: 
		rightSwitch = false;
		break;
	default: break;
	}
}

void myKeyboard(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 's': 
		if(!startSwitch)
		{
			startSwitch = true;
			PlaySound("data/music/title_theme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		else if(winSwitch == true)
		{
			restart();
			winSwitch = false;
			startSwitch = true;
			PlaySound("data/music/title_theme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		glutPostRedisplay();
		break;
	default: break;
	}
}

/************** Mouse ***************/
// Right button menu
void mainMenu(GLint id)
{
	// Handle main menu
	switch(id) {
	case 1: 
		if(startSwitch == false)
		{
			startSwitch = true;
			PlaySound("data/music/title_theme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		else if(winSwitch == true)
		{
			restart();
			winSwitch = false;
			startSwitch = true;
			PlaySound("data/music/title_theme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		glutPostRedisplay();
		break;
	case 2: exit(-1); break;}
}

// Difficulty menu
void difficultyMenu(GLint id)
{
	// Handle difficulty
	switch(id) {
	case 1: speed = 0.03; difficulty = 1; glutPostRedisplay(); break;
	case 2: speed = 0.07; difficulty = 2; glutPostRedisplay(); break;
	case 3: speed = 0.11; difficulty = 3; glutPostRedisplay(); break;
	}
}

void defineMenus(void)
{		
	GLint dMenu = glutCreateMenu(difficultyMenu);
	glutAddMenuEntry("Easy", 1);
	glutAddMenuEntry("Normal", 2);
	glutAddMenuEntry("Hard", 3);

	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Start", 1);
	glutAddSubMenu("Difficulty", dMenu);
	glutAddMenuEntry("Quit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/************** Texture ***************/
void loadImage(char* name)
{
	// Load image
	free(image);
	image = glmReadPPM(name, &iWidth, &iHeight);
}

void textureEnv()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}

/************** myInit ***************/
void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);    // Set background color to white

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.2, 0.2, -0.15, 0.25, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(bird.getMatrix(3, 0)-cameraX, bird.getMatrix(3, 1), bird.getMatrix(3, 2)+cameraZ, bird.getMatrix(3, 0), bird.getMatrix(3, 1), bird.getMatrix(3, 2), 0.0, 0.0, 1.0);
}

/************** myDisplay ************/
void myDisplay(void)
{
	// Handle draw events
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Light
	GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat mat_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_shininess[] = {50.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	GLfloat lightIntensity[] = {50.0f, 50.0f, 50.0f, 1.0f};
	GLfloat lightPosition[]={-1.0, -1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_SHININESS, lightIntensity);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	setupSkybox();
	setupAxis();
	setupScoreAndTime();
	setupTorus();
	if(torusCount != 10)
		setupNavArrow();

	glPushMatrix();
	bird.loadMatrix();
	bird.drawBird();
	glPopMatrix();

	// Start
	if(startSwitch == false)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		drawNormalString("Press 's' to start", bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.29, bird.getMatrix(3, 2)+1.03);
	}
	else
		timeSwitch = true;

	// Win
	if(torusCount == 10)
	{
		currentTorus = 10;

		if(winSwitch == false)
			PlaySound("data/music/level_complete.wav", NULL, SND_FILENAME | SND_ASYNC);

		timeSwitch = false;
		winSwitch = true;
		glColor3f(1.0f, 1.0f, 1.0f);
		loadImage("data/image/congratulations.ppm");
		textureEnv(); 
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_POLYGON);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0, 1); glVertex3f(bird.getMatrix(3, 0), bird.getMatrix(3, 1)-0.8, bird.getMatrix(3, 2)+0.9);
		glTexCoord2f(0, 0); glVertex3f(bird.getMatrix(3, 0), bird.getMatrix(3, 1)-0.8, bird.getMatrix(3, 2)+0.7);
		glTexCoord2f(1, 0); glVertex3f(bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.8, bird.getMatrix(3, 2)+0.7);
		glTexCoord2f(1, 1); glVertex3f(bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.8, bird.getMatrix(3, 2)+0.9);
		glEnd();
		glPopMatrix();

		glColor3f(0.0f, 0.0f, 0.0f);
		drawNormalString("Press 's' to start", bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.29, bird.getMatrix(3, 2)+1.03);
	}

	// Timer
	if(timeSwitch)
		glutTimerFunc(timerInterval, redisplay, 1);

	glutSwapBuffers();
}

void setupSkybox()
{
	// Skybox
	glColor3f(1.0f, 1.0f, 1.0f);
	loadImage("data/image/skyboxDown.ppm");
	textureEnv(); 
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-12, 20, -20);
	glTexCoord2f(0, 0); glVertex3f(-12, -20, -20);
	glTexCoord2f(1, 0); glVertex3f(38, -20, -20);
	glTexCoord2f(1, 1); glVertex3f(38, 20, -20);
	glEnd();
	glPopMatrix();

	loadImage("data/image/skyboxLeft.ppm");
	textureEnv(); 
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(38, 20, 20);
	glTexCoord2f(0, 0); glVertex3f(38, 20, -20);
	glTexCoord2f(1, 0); glVertex3f(-12, 20, -20);
	glTexCoord2f(1, 1); glVertex3f(-12, 20, 20);
	glEnd();
	glPopMatrix();

	loadImage("data/image/skyboxRight.ppm");
	textureEnv(); 
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-12, -20, 20);
	glTexCoord2f(0, 0); glVertex3f(-12, -20, -20);
	glTexCoord2f(1, 0); glVertex3f(38, -20, -20);
	glTexCoord2f(1, 1); glVertex3f(38, -20, 20);
	glEnd();
	glPopMatrix();

	loadImage("data/image/skyboxFront.ppm");
	textureEnv(); 
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(38, -20, 20);
	glTexCoord2f(0, 0); glVertex3f(38, -20, -20);
	glTexCoord2f(1, 0); glVertex3f(38, 20, -20);
	glTexCoord2f(1, 1); glVertex3f(38, 20, 20);
	glEnd();
	glPopMatrix();

	loadImage("data/image/skyboxUp.ppm");
	textureEnv(); 
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(38, -20, 20);
	glTexCoord2f(0, 0); glVertex3f(38, 20, 20);
	glTexCoord2f(1, 0); glVertex3f(-12, 20, 20);
	glTexCoord2f(1, 1); glVertex3f(-12, -20, 20);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void setupAxis()
{
	// Draw axis
	glColor3f(0.0f, 0.0f, 1.0f);  // Blue
	drawAxis(0.5);       // Z-axis
	drawNormalString("Z", 0, 0, 0.6);
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glColor3f(1.0f, 0.0f, 0.0f);  // Red
	drawAxis(0.5);       // X-axis
	drawNormalString("X", 0, 0, 0.6);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	drawAxis(0.5);       // Y-axis
	drawNormalString("Y", 0, 0, 0.6);
	glPopMatrix();
}

void setupScoreAndTime()
{
	// Score and time
	glColor3f(0.0f, 0.0f, 0.0f);

	string str = to_string(torusCount);
	drawNormalString(str, bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.77, bird.getMatrix(3, 2)+1.03);
	drawNormalString("/10", bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.7, bird.getMatrix(3, 2)+1.03);

	switch (difficulty)
	{
	case 1:
		str = "Easy"; break;
	case 2:
		str = "Normal"; break;
	case 3:
		str = "Hard"; break;
	default:
		break;
	}

	drawNormalString(str, bird.getMatrix(3, 0), bird.getMatrix(3, 1)+0.8, bird.getMatrix(3, 2)+0.99);
}

void setupTorus()
{
	bool green = false;
	for(int i=0; i<10; i++)
	{
		torus[i].setCenter(i*2.5+2.5, 0);
		torus[i].setCenter(2.5*sin(float((i+1)*15)), 1);
		torus[i].setCenter(2.5*cos(float((i+1)*15)), 2);

		if(torus[i].isVisible())
		{
			if(i == 0)
			{
				glColor3f(0.0f, 1.0f, 0.0f); // Green
				currentTorus = 0;
				green = true;
			}
			else if(i >= 1)
			{
				if(!torus[i-1].isVisible() && !green)
				{
					glColor3f(0.0f, 1.0f, 0.0f);
					currentTorus = i;
					green = true;
				}
				else
				{
					glColor3f(1.0f, 1.0f, 0.0f); // Yellow
				}
			}
			torus[i].drawTorus();
		}
	}
}

void setupNavArrow()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	float h = 0.9;
	float lx = (torus[currentTorus].getCenter(0) - bird.getMatrix(3, 0)) * 0.1;
	float ly = (torus[currentTorus].getCenter(1) - bird.getMatrix(3, 1)) * 0.1;
	float lz = (torus[currentTorus].getCenter(2) - bird.getMatrix(3, 2)) * 0.1;
	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3d(bird.getMatrix(3, 0), bird.getMatrix(3, 1), bird.getMatrix(3, 2)+h);
	glVertex3d(bird.getMatrix(3, 0)+0.8*lx,  bird.getMatrix(3, 1)+0.8*ly+0.07, bird.getMatrix(3, 2)+0.8*lz+h);
	glVertex3d(bird.getMatrix(3, 0)+0.8*lx,  bird.getMatrix(3, 1)+0.8*ly-0.07, bird.getMatrix(3, 2)+0.8*lz+h);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3d(bird.getMatrix(3, 0)+0.8*lx,  bird.getMatrix(3, 1)+0.8*ly+0.15, bird.getMatrix(3, 2)+0.8*lz+h);
	glVertex3d(bird.getMatrix(3, 0)+lx, bird.getMatrix(3, 1)+ly, bird.getMatrix(3, 2)+lz+h);
	glVertex3d(bird.getMatrix(3, 0)+0.8*lx,  bird.getMatrix(3, 1)+0.8*ly-0.15, bird.getMatrix(3, 2)+0.8*lz+h);
	glEnd();
	glPopMatrix();
}

// Timer function
void redisplay(int value)
{
	time = time + 0.04;
	moveBird();
	resetView();
	glutPostRedisplay();
}

// Reshape handler
void myReshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, windowWidth, windowHeight);              // Set viewport to whole window      
	resetView();
}

void resetView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.2, 0.2, -0.15, 0.25, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(bird.getMatrix(3, 0)-cameraX, bird.getMatrix(3, 1), bird.getMatrix(3, 2)+cameraZ, bird.getMatrix(3, 0), bird.getMatrix(3, 1), bird.getMatrix(3, 2), 0.0, 0.0, 1.0);
}

/************** main *****************/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 50);
	int mainWindow = glutCreateWindow("My 3D Transforms");

	glutSpecialFunc(mySpecialKeyboard);            // register specail keyboard function
	glutSpecialUpFunc(mySpecialKeyboardUp);        // register specail keyboard up function
	glutKeyboardFunc(myKeyboard);                  // register keyboard function

	glutDisplayFunc(myDisplay);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glutReshapeFunc(myReshape);

	defineMenus();
	myInit();

	glutMainLoop();
	return 0;
}
