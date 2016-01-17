/*
 *	Pair Potential Simulator in GLUT
 *	atmsim.h - simulation header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _pairgl_atmsim_h_
#define _pairgl_atmsim_h_

#define	maxdim	3
#define maxobjs 1024

typedef struct object
{
	double m;
	double rmin;
	double q;
	double s[maxdim];
	double v[maxdim];
	double R,G,B;
} object;

void drawallobjects();
void atomiterate();
void atominit();
void updateobjectv(object* one, object* two);
void updateobjects(object* one);

#endif