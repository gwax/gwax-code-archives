/*
 *	Soap Surface Simulator in GLUT
 *	main.h - main header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _soapgl_main_h_
#define _soapgl_main_h_

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

#endif