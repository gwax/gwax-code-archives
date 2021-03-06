/*
 *	Pair Potential Simulator in GLUT
 *	globals.h - global variable header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _pairgl_globals_h_
#define _pairgl_globals_h_

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

extern GLUquadricObj* quadratic;
extern float ratio;

extern double dt;
extern double t;
extern double epsilon;
extern double k;
extern double rthreshold;
extern double redge;

extern double xMin, xMax;
extern double yMin, yMax;

extern double lenMul;

extern object obj[maxobjs];
extern int objs;

extern double xRot, yRot, zRot;

extern int objdetail;

extern int dim;

#endif