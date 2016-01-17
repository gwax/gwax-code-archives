/*
 *	Pair Potential Simulator in GLUT
 *	atmsim.c - simulation source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include <math.h>
#include "main.h"
#include "atmsim.h"
#include "globals.h"

void drawallobjects()
{
	int i;
	double xPos, yPos, zPos, rad;
	double xCur, yCur, zCur;
	
	xCur = 0.0;
	yCur = 0.0;
	zCur = 0.0;
	for(i=0; i<objs; i++)
	{
		switch(dim)
		{
		case 3:
			xPos = lenMul*obj[i].s[0]/yMax;
			yPos = lenMul*obj[i].s[1]/yMax;
			zPos = lenMul*obj[i].s[2]/yMax;
			break;
		case 2:
			xPos = lenMul*obj[i].s[0]/yMax;
			yPos = lenMul*obj[i].s[1]/yMax;
			zPos = 0.0;
			break;
		case 1:
			xPos = lenMul*obj[i].s[0]/yMax;
			yPos = 0.0;
			zPos = 0.0;
			break;
		}
		rad = lenMul*obj[i].rmin/yMax;

		glColor3f(obj[i].R, obj[i].G, obj[i].B);
		glTranslatef(xPos-xCur, yPos-yCur, zPos-zCur);
		gluSphere(quadratic, rad, objdetail, objdetail);

		xCur = xPos;
		yCur = yPos;
		zCur = zPos;
	}
	glTranslatef(-xCur, -yCur, -zCur);
}

void atominit()
{
	int i, j;
	dim = 2;

	yMin = -70;
	yMax = -yMin;
	xMin = 4*yMin/3;
	xMax = 4*yMax/3;

	objdetail = 16;
	t = 0;

	dt = 0.0000000001; // if your system 'splodes this is too big
	rthreshold = 20;	//it seems to be pretty good for now

	epsilon = 50;	// constant, damned if I know what it should be
	k = 20;			// constant, this one I know, but I'm not putting it in now

	lenMul = 1.65;

	objs = 999;

    for(i=0; i<37; i++)
	{
		for(j=0; j<27; j++)
		{
			obj[27*i+j].rmin = 2;
			obj[27*i+j].m = 1;
			obj[27*i+j].q = 0;
			obj[27*i+j].s[0] = -72 + 4*i;
			obj[27*i+j].s[1] = -52 + 4*j;
			obj[27*i+j].s[2] = 0;
			obj[27*i+j].v[0] = 0;
			obj[27*i+j].v[1] = 0;
			obj[27*i+j].v[2] = 0;

			obj[27*i+j].R = 1;
			obj[27*i+j].G = 1;
			obj[27*i+j].B = 1;
		}
	}
}

void atomiterate()
{
	int i, j;

	for(i=0; i<objs; i++)
	{
		updateobjects(&obj[i]);
	}

	for(i=0; i<objs; i++)
	{
		for(j=i+1; j<objs; j++)
		{
			updateobjectv(&obj[i], &obj[j]);
		}
	}

	t += dt;
}

void updateobjectv(object* one, object* two)
{
	int i;
	double r, rm;
	double rs[maxdim];
	double f[maxdim];

	rm = one->rmin + two->rmin;

	for(i=0; i<dim; i++)
	{
		rs[i] = one->s[i] - two->s[i];
	}

	r = 0;
	for(i=0; i<dim; i++)
	{
		r += rs[i]*rs[i];
	}
	if(sqrt(r)>rthreshold)
		return;


	for(i=0; i<dim; i++)
	{
		f[i] = 0;
		f[i] += epsilon*(-12*rs[i]*pow(rm,12)/pow(r,7) + 12*rs[i]*pow(rm,6)/pow(r,4));
		f[i] -= k*one->q*two->q*rs[i]/pow(r,1.5);
	}

	for(i=0; i<dim; i++)
	{
		one->v[i] -= dt*f[i]/one->m;
		two->v[i] += dt*f[i]/two->m;
	}
}

void updateobjects(object* one)
{
	int i;
	for(i=0; i<dim; i++)
	{
		one->s[i] += dt*one->v[i];
	}
}