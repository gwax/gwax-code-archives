/*
 *	Soap Surface Simulator in GLUT
 *	soap.h - simulation header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _soapgl_pair_h_
#define _soapgl_pair_h_

#define	dim	3
#define maxverts 1024

typedef struct vertex
{
	int fixed;
	int exists;
	double s[dim];
	double v[dim];
} vertex;

void soapinit();
void dostuff();
void recalcmaxr();

/*
void drawedges();
void drawallobjects();
void pairiterate();
void updateobjectvedge(object* one);
void updateobjectv(object* one, object* two);
void updateobjects(object* one);
*/
#endif