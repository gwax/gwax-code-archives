/*
 *	Soap Surface Simulator in GLUT
 *	globals.h - global variable header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _soapgl_globals_h_
#define _soapgl_globals_h_

extern vector3d vectorUp;
extern camera cameraDefault;
extern camera *cameraCurrent;

extern char *windowTitle;

extern window windowMain;
extern window windowTemp;

extern int stateFullScreen;

extern int frame;
extern int timebase;
extern int time;
extern int timepassed;

extern float ratio;

extern double dt;
extern double t;

extern double xMin, xMax;
extern double yMin, yMax;

extern double lenMul;

extern double xRot, yRot, zRot;

extern int numverts;
extern vertex verts[maxverts];

extern double rscale;
extern double minrscale;
extern double maxr;
extern double minr;

#endif