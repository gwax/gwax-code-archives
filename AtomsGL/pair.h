/*
 *	AtomsGL - GLUT Based atom simulation framework
 *	pair.h - simulation header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _pairgl_pair_h_
#define _pairgl_pair_h_

#define	dim	2
#define maxobjs 1024

typedef struct object
{
	double rmin;
	double s[dim];
	double v[dim];
	double R,G,B;
} object;

void drawedges();
void drawallobjects();
void pairiterate();
void pairinit();
void updateobjectvedge(object* one);
void updateobjectv(object* one, object* two);
void updateobjects(object* one);

#endif