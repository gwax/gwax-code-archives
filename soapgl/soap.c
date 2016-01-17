/*
 *	Soap Surface Simulator in GLUT
 *	soap.c - simulation source file
 *	(c)2002 George Waksman
 *
 */

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "main.h"
#include "soap.h"
#include "globals.h"


void soapinit()
{
	int i,j;

	yMin = -20;
	yMax = -yMin;
	xMin = 4*yMin/3;
	xMax = 4*yMax/3;
	lenMul = 1.65;

	dt = 0.0001; // if your system 'splodes this is too big
	t = 0;

	rscale = 2;
	minrscale = 1.0/3.0;

    numverts = 441;

	for(i=0; i<21; i++)
	{
		for(j=0; j<21; j++)
		{
			verts[21*i+j].exists = 1;

			if(i<10)
			{
				verts[21*i+j].s[0] = -20 + 2*i;
				verts[21*i+j].s[1] = -20 + 2*j;
				verts[21*i+j].s[2] = 0;
			}
			else
			{
				verts[21*i+j].s[0] = 0;
				verts[21*i+j].s[1] = -20 + 2*j;
				verts[21*i+j].s[2] = -20 + 2*i;
			}

			verts[21*i+j].v[0] = 0;
			verts[21*i+j].v[1] = 0;
			verts[21*i+j].v[2] = 0;

			if((i==0 || i==20) || (j==0 || j==20))
			{
				verts[21*i+j].fixed = 1;
			}
			else
			{
				verts[21*i+j].fixed = 0;
			}
		}
	}

	recalcmaxr();
	//minr = 0;
}

void drawverts()
{
	int i;
	double xPos, yPos, zPos;
	
	glBegin(GL_POINTS);
	for(i=0; i<numverts; i++)
	{
		switch(dim)
		{
		case 3:
			xPos = lenMul*verts[i].s[0]/yMax;
			yPos = lenMul*verts[i].s[1]/yMax;
			zPos = lenMul*verts[i].s[2]/yMax;
			break;
		case 2:
			xPos = lenMul*verts[i].s[0]/yMax;
			yPos = lenMul*verts[i].s[1]/yMax;
			zPos = 0.0;
			break;
		case 1:
			xPos = lenMul*verts[i].s[0]/yMax;
			yPos = 0.0;
			zPos = 0.0;
			break;
		}

		if(verts[i].fixed==1)
		{
			glColor3f(1.0,0.0,0.0);
		}
		else
		{
			glColor3f(0.0,1.0,0.0);
		}
		glVertex3f(xPos,yPos,zPos);
	}
	glEnd();
}

void dostuff()
{
	int i,j,k;
	double xPos,yPos,zPos;
	double r, f;

	for(i=0;i<numverts;i++)
	{
		for(k=0;k<dim;k++)
		{
			verts[i].v[k] = 0;
		}
	}

	for(i=0;i<numverts;i++)
	{
		if(verts[i].exists != 1)
			goto endoffori;
		for(j=i+1;j<numverts;j++)
		{
			if(verts[j].exists != 1)
				goto endofforj;
			r = 0.0;
			for(k=0;k<dim;k++)
			{
				r += pow((verts[i].s[k]-verts[j].s[k]),2.0);
			}
			r = pow(r, 0.5);

			/*if(r<minr)
			{
				if(verts[j].fixed != 1)
				{
					verts[j].exists = 0;
					verts[i].v[0] = 0;
					verts[i].v[1] = 0;
				}
				else if(verts[i].fixed != 1)
				{
					verts[i].exists = 0;
					verts[j].v[0] = 0;
					verts[j].v[1] = 0;
				}
			}*/

			if(r<maxr /*&& r>minr*/)
			{
				f=0;
				for(k=0;k<dim;k++)
				{
					f = (verts[i].s[k]-verts[j].s[k])*(r-minr);
					verts[i].v[k] -= f*dt;
					verts[j].v[k] += f*dt;
				}

				glBegin(GL_LINES);

					if(verts[i].fixed==1)
					{
						glColor3f(1.0,0.0,0.0);
					}
					else
					{
						glColor3f(0.0,1.0,1.0);
					}
					switch(dim)
					{
					case 3:
						xPos = lenMul*verts[i].s[0]/yMax;
						yPos = lenMul*verts[i].s[1]/yMax;
						zPos = lenMul*verts[i].s[2]/yMax;
						break;
					case 2:
						xPos = lenMul*verts[i].s[0]/yMax;
						yPos = lenMul*verts[i].s[1]/yMax;
						zPos = 0.0;
						break;
					case 1:
						xPos = lenMul*verts[i].s[0]/yMax;
						yPos = 0.0;
						zPos = 0.0;
						break;
					}
					glVertex3f(xPos,yPos,zPos);

					if(verts[j].fixed==1)
					{
						glColor3f(1.0,0.0,0.0);
					}
					else
					{
						glColor3f(0.0,1.0,1.0);
					}
					switch(dim)
					{
					case 3:
						xPos = lenMul*verts[j].s[0]/yMax;
						yPos = lenMul*verts[j].s[1]/yMax;
						zPos = lenMul*verts[j].s[2]/yMax;
						break;
					case 2:
						xPos = lenMul*verts[j].s[0]/yMax;
						yPos = lenMul*verts[j].s[1]/yMax;
						zPos = 0.0;
						break;
					case 1:
						xPos = lenMul*verts[j].s[0]/yMax;
						yPos = 0.0;
						zPos = 0.0;
						break;
					}
					glVertex3f(xPos,yPos,zPos);
					
				glEnd();
			}
			endofforj:;
		}

		if(verts[i].fixed != 1)
		{
			for(k=0;k<dim;k++)
			{
				verts[i].s[k] += verts[i].v[k];
			}
		}
		endoffori:;
	}
}

void recalcmaxr()
{
	int i,j;
	double maxs[dim];
	double mins[dim];

	for(i=0;i<dim;i++)
	{
		maxs[i] = 0.0;
		mins[i] = 0.0;
		for(j=0;j<numverts;j++)
		{
			maxs[i] = __max(maxs[i], verts[j].s[i]);
			mins[i] = __min(mins[i], verts[j].s[i]);
		}
	}

	//the following won't work if the structure gets thin
	//and approaches a lower dimensional structure so I
	//need to come back and fix it later
	maxr = 1.0;
	for(i=0;i<dim;i++)
	{
		maxr *= maxs[i]-mins[i];
	}

	maxr = rscale*pow( maxr/pow( pow((double)numverts,(1.0/3.0))-1.0, 3.0), (1.0/3.0));

	minr = minrscale*maxr; //maybe? hopefully?
}