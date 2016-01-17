/*
 *	Soap Surface Simulator in GLUT
 *	globals.c - global variable source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include "main.h"
#include "soap.h"

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

float ratio;

double dt;
double t;

double xMin, xMax;
double yMin, yMax;

double lenMul;

double xRot, yRot, zRot;

int numverts;
vertex verts[maxverts];

double rscale;
double minrscale;
double maxr;
double minr;