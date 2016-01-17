/*
 *	Pair Potential Simulator in GLUT
 *	globals.c - global variable source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include "main.h"
#include "atmsim.h"

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

GLUquadricObj* quadratic;
float ratio;

double dt;
double t;
double epsilon;
double k;
double rthreshold;
double redge;

double xMin, xMax;
double yMin, yMax;

double lenMul;

object obj[maxobjs];
int objs;

double xRot, yRot, zRot;

int objdetail;

int dim;