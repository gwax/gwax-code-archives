#ifndef _atoms_h_
#define _atoms_h_

#include "types.h"

#define NUMBONDS 4

typedef struct
{
	enum AtomTypes type;
	double x,y,z;
	double vx,vy,vz;
	double ax,ay,az;
	double potential,newpotential;
	double ndx,ndy,ndz;

	int bonds[NUMBONDS];
} Atom;

#endif
