/*
 *	Generic GLUT framework
 *	main.h - main header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _glutframework_main_h_	//prevent multiple instances
#define _glutframework_main_h_

//external prototypes
void endprog();

//external structures
typedef struct _vector3d
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
} vector3d;

typedef struct _camera
{
	vector3d location;
	vector3d target;
} camera;

typedef struct _window
{
	int x;
	int y;
	int width;
	int height;
} window;

//global variables
vector3d vectorUp;
camera cameraDefault;
camera *cameraCurrent;

char *windowTitle;

window windowMain;
window windowTemp;

int stateFullScreen;

int frame;
int timebase;
int time;
int timepassed;

int incps;
int mspinc;

#endif