#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "atoms.h"
#include "header.h"
#include "types.h"

#define getclosest(d,bo2,b)  ((d > bo2)?(d-b):((d < -bo2)?(d+b):d))

#define OUTPUTEVERY 1000

#define MAXNEIGHBORS 500

const double kB = 1.380658E-23;

Atom* allatoms;
int numatoms;
int** neighbors;

double bigX,bigY,bigZ;
double bXo2,bYo2,bZo2;

double mincutoff;
double maxdr;

double dt,hdt;

double desiredT;
double T;
double velMul;

int iterations;

void correctT()
{
	int i;

	T = 0;

	for(i=0; i<numatoms; i++)
	{
		T += 0.5 * elements[allatoms[i].type].mass * (allatoms[i].vx*allatoms[i].vx + allatoms[i].vy*allatoms[i].vy + allatoms[i].vz*allatoms[i].vz);
	}

	T /= (double)numatoms;
	T /= 1.5 * kB;
}

void makeneighbors()
{
	int i,j,k;
	double dx,dy,dz;
	Atom* thisatom;
	Atom* thatatom;

	maxdr = 0.0;

	printf("calculating neighbors at i=%d\n\n", iterations);

	for(i=0; i<numatoms; i++)
	{
		thisatom = allatoms + i;
		k = 0;

		thisatom->ndx = thisatom->x;
		thisatom->ndy = thisatom->y;
		thisatom->ndz = thisatom->z;

		for(j=i+1; j<numatoms && k<MAXNEIGHBORS; j++)
		{
			thatatom = allatoms + j;

			dx = thisatom->x - thatatom->x;
			dy = thisatom->y - thatatom->y;
			dz = thisatom->z - thatatom->z;

			dx = getclosest(dx,bXo2,bigX);
			dy = getclosest(dy,bYo2,bigY);
			dz = getclosest(dz,bZo2,bigZ);

			if(dx<mincutoff&&dx>-mincutoff&&dy<mincutoff&&dy>-mincutoff&&dz<mincutoff&&dz>-mincutoff)
			{
				neighbors[i][k++] = j;
			}
		}

		neighbors[i][k] = -1;
	}
}

void interactions()
{
	int i,j,k,l,m,n;
	Atom* thisatom;
	Atom* atom2;
	Atom* atom3;
	Atom* thatatom;
	int atom2off;
	int atom3off;
	int atom4off;
	double epsilon,sigma,thismass,thatmass,k0,x0,z;
	double rmsq;
	double dx,dy,dz;
	double rsq,rmor2,rmor6,rmor12,r,rmx0;
	double pot,mag;
	double fx,fy,fz;
	int bonded,doffset;

	for(i=0; i<numatoms; i++)
	{
		thisatom = allatoms + i;
		thismass = elements[thisatom->type].mass;

		/*order 1 bonded interactions*/
		for(j=0; j<NUMBONDS; j++)
		{
			if(thisatom->bonds[j] <= 0)
				continue;

			thatatom = thisatom + thisatom->bonds[j];
			thatmass = elements[thatatom->type].mass;

			switch(thisatom->type)
			{
			case H:
				k0 = 483;
				x0 = 1.12E-10;
				break;
			default:
				switch(thatatom->type)
				{
				case H:
					k0 = 483;
					x0 = 1.12E-10;
					break;
				default:
					k0 = 450;
					x0 = 1.53E-10;
				}
				break;
			}

			dx = thisatom->x - thatatom->x;
			dy = thisatom->y - thatatom->y;
			dz = thisatom->z - thatatom->z;

			dx = getclosest(dx,bXo2,bigX);
			dy = getclosest(dy,bYo2,bigY);
			dz = getclosest(dz,bZo2,bigZ);

			rsq = dx*dx + dy*dy + dz*dz;
			r = sqrt(rsq);
			rmx0 = r-x0;

			pot = 0.5 * k0 * rmx0 * rmx0;
			mag = -k0 * rmx0 / r;
			fx = dx*mag;
			fy = dy*mag;
			fz = dz*mag;

			thisatom->newpotential += pot;
			thisatom->ax += fx/thismass;
			thisatom->ay += fy/thismass;
			thisatom->az += fz/thismass;

			thatatom->newpotential += pot;
			thatatom->ax -= fx/thatmass;
			thatatom->ay -= fy/thatmass;
			thatatom->az -= fz/thatmass;
		}

		/*order 2 bonded interactions*/
		for(j=0; j<NUMBONDS; j++)
		{
			atom2off = thisatom->bonds[j];
			if(atom2off == 0)
				continue;

			atom2 = thisatom+atom2off;

			for(k=0; k<NUMBONDS; k++)
			{
				atom3off = atom2off+atom2->bonds[k];
				if(atom2->bonds[k] == 0 || atom3off <= 0)
					continue;

				thatatom = thisatom + atom3off;
				thatmass = elements[thatatom->type].mass;

				switch(thisatom->type)
				{
				case H:
					switch(thatatom->type)
					{
					case H:
						x0 = 1.83E-10;
						break;
					default:
						x0 = 2.18E-10;
						break;
					}
					break;
				default:
					switch(thatatom->type)
					{
					case H:
						x0 = 2.18E-10;
						break;
					default:
						x0 = 2.50E-10;
						break;
					}
					break;
				}
				k0 = 10;

				dx = thisatom->x - thatatom->x;
				dy = thisatom->y - thatatom->y;
				dz = thisatom->z - thatatom->z;

				dx = getclosest(dx,bXo2,bigX);
				dy = getclosest(dy,bYo2,bigY);
				dz = getclosest(dz,bZo2,bigZ);

				rsq = dx*dx + dy*dy + dz*dz;
				r = sqrt(rsq);
				rmx0 = r-x0;

				pot = 0.5 * k0 * rmx0 * rmx0;
				mag = -k0 * rmx0 / r;
				fx = dx*mag;
				fy = dy*mag;
				fz = dz*mag;

				thisatom->newpotential += pot;
				thisatom->ax += fx/thismass;
				thisatom->ay += fy/thismass;
				thisatom->az += fz/thismass;

				thatatom->newpotential += pot;
				thatatom->ax -= fx/thatmass;
				thatatom->ay -= fy/thatmass;
				thatatom->az -= fz/thatmass;
			}
		}

		/*order 3 bonded interactions*/
		for(j=0; j<NUMBONDS; j++)
		{
			atom2off = thisatom->bonds[j];
			if(atom2off == 0)
				continue;

			atom2 = thisatom + atom2off;

			for(k=0; k<NUMBONDS; k++)
			{
				atom3off = atom2off + atom2->bonds[k];
				if(atom2->bonds[k] == 0 || atom3off <= 0)
					continue;

				atom3 = thisatom + atom3off;

				for(l=0; l<NUMBONDS; l++)
				{
					atom4off = atom3off + atom3->bonds[l];
					if(atom3->bonds[k] == 0 || atom4off <= 0)
						continue;

					thatatom = thisatom + atom4off;
					thatmass = elements[thatatom->type].mass;

					switch(thisatom->type)
					{
					case H:
						switch(thatatom->type)
						{
						case H:
							x0 = 2.278E-10;
							z = 2.392E-19;
							break;
						default:
							x0 = 2.446E-10;
							z = 2.234E-19;
							break;
						}
						break;
					default:
						switch(thatatom->type)
						{
						case H:
							x0 = 2.446E-10;
							z = 2.234E-19;
							break;
						default:
							x0 = 2.551E-10;
							z = 2.660E-19;
							break;
						}
						break;
					}

					dx = thisatom->x - thatatom->x;
					dy = thisatom->y - thatatom->y;
					dz = thisatom->z - thatatom->z;

					dx = getclosest(dx,bXo2,bigX);
					dy = getclosest(dy,bYo2,bigY);
					dz = getclosest(dz,bZo2,bigZ);

					rsq = dx*dx + dy*dy + dz*dz;

					pot = z*x0*x0/rsq;
					mag = 2.0*pot/rsq;
					fx = dx*mag;
					fy = dy*mag;
					fz = dz*mag;

					thisatom->newpotential += pot;
					thisatom->ax += fx/thismass;
					thisatom->ay += fy/thismass;
					thisatom->az += fz/thismass;

					thatatom->newpotential += pot;
					thatatom->ax -= fx/thatmass;
					thatatom->ay -= fy/thatmass;
					thatatom->az -= fz/thatmass;
				}
			}
		}

		/*order 0, non-bonded (Van Der Waal's) interactions*/
		for(j=0; j<MAXNEIGHBORS; j++)
		{
			if(neighbors[i][j] == -1)
				break;

			thatatom = allatoms + neighbors[i][j];
			doffset = neighbors[i][j] - i;

			bonded = 0;
			for(n=0; n<NUMBONDS; n++)
			{
				if(doffset == thisatom->bonds[n])
					bonded = 1;

				for(m=0; m<NUMBONDS; m++)
				{
					if(doffset == (thisatom+thisatom->bonds[n])->bonds[m]+thisatom->bonds[n])
						bonded = 1;
				}
			}

			if(bonded)
				continue;

			thatmass = elements[thatatom->type].mass;

			fx = fy = fz = 0.0;

			if(thisatom->type == thatatom->type)
			{
				epsilon = elements[thisatom->type].epsilon;
				sigma = 2.0 * elements[thisatom->type].radius;
			}
			else
			{
				epsilon = sqrt(elements[thisatom->type].epsilon*elements[thatatom->type].epsilon);
				sigma = elements[thisatom->type].radius + elements[thatatom->type].radius;
			}

			rmsq = sigma*sigma;

			dx = thisatom->x - thatatom->x;
			dy = thisatom->y - thatatom->y;
			dz = thisatom->z - thatatom->z;

			dx = getclosest(dx,bXo2,bigX);
			dy = getclosest(dy,bYo2,bigY);
			dz = getclosest(dz,bZo2,bigZ);

			rsq = dx*dx + dy*dy + dz*dz;

			rmor2 = rmsq/rsq;
			rmor6 = rmor2*rmor2*rmor2;
			rmor12 = rmor6*rmor6;

			pot = -4.0*epsilon*(rmor6-rmor12);
			mag = -24.0*(epsilon/rsq)*(rmor6-2.0*rmor12);
			fx = dx*mag;
			fy = dy*mag;
			fz = dz*mag;

			thisatom->newpotential += pot;
			thisatom->ax += fx/thismass;
			thisatom->ay += fy/thismass;
			thisatom->az += fz/thismass;

			thatatom->newpotential += pot;
			thatatom->ax -= fx/thatmass;
			thatatom->ay -= fy/thatmass;
			thatatom->az -= fz/thatmass;
		}
	}
}

void integrate()
{
	int i;
	double axhdt, ayhdt, azhdt;
	double dx,dy,dz;
	Atom* thisatom;

	for(i=0; i<numatoms; i++)
	{
		thisatom = allatoms + i;

		thisatom->potential = thisatom->newpotential;

		/*temperature correction*/
		thisatom->vx *= velMul;
		thisatom->vy *= velMul;
		thisatom->vz *= velMul;

		/* velocity verlet integration */
		axhdt = thisatom->ax * hdt;
		ayhdt = thisatom->ay * hdt;
		azhdt = thisatom->az * hdt;
		thisatom->vx += axhdt;
		thisatom->vy += ayhdt;
		thisatom->vz += azhdt;
		thisatom->x += thisatom->vx * dt + axhdt*dt;
		thisatom->y += thisatom->vy * dt + ayhdt*dt;
		thisatom->z += thisatom->vz * dt + azhdt*dt;
		thisatom->vx += axhdt;
		thisatom->vy += ayhdt;
		thisatom->vz += azhdt;

		if(thisatom->x > bigX) thisatom->x -= bigX;
		else if(thisatom->x < 0.0) thisatom->x += bigX;
		if(thisatom->y > bigY) thisatom->y -= bigY;
		else if(thisatom->y < 0.0) thisatom->y += bigY;
		if(thisatom->z > bigZ) thisatom->z -= bigZ;
		else if(thisatom->z < 0.0) thisatom->z += bigZ;

		dx = thisatom->x - (thisatom->ndx);
		dy = thisatom->y - (thisatom->ndy);
		dz = thisatom->z - (thisatom->ndz);

		dx = getclosest(dx,bXo2,bigX);
		dy = getclosest(dy,bYo2,bigY);
		dz = getclosest(dz,bZo2,bigZ);

		maxdr = max(max(maxdr,dx),max(dy,dz));;

		thisatom->newpotential = 0.0;
		thisatom->ax = 0.0;
		thisatom->ay = 0.0;
		thisatom->az = 0.0;
	}
}

int main(int argc, char** argv)
{
	int i,j;
	int outevery = OUTPUTEVERY;
	int keepgoing;
	Headerinfo tempheader;
	FILE* file;
	int outnum;
	char buffer[15];
	int time0;

	time0 = (int)time(NULL);

	outnum = 0;

	if(argc < 2)
	{
		printf("no infile specified\n\n");
		return 0;
	}
	
	file = fopen(argv[1], "rb");

	if(file == NULL)
	{
		printf("failed to open infile %s for reading\n\n", argv[1]);
		return errno;
	}
	
	printf("using infile %s\n\n", argv[1]);

	i = (int)fread(&tempheader, sizeof(Headerinfo), 1, file);

	if(i < 1)
	{
		printf("error reading header from infile\n\n");
		return 0;
	}

	iterations = tempheader.iterations;

	if(iterations > 0)
		keepgoing = 1;
	else
		keepgoing = 0;

	mincutoff = tempheader.mincutoff;
	dt = tempheader.dt;
	bigX = tempheader.bigX;
	bXo2 = bigX/2.0;
	bigY = tempheader.bigY;
	bYo2 = bigY/2.0;
	bigZ = tempheader.bigZ;
	bZo2 = bigZ/2.0;
	desiredT = tempheader.desiredT;
	T = desiredT;

	numatoms = tempheader.numatoms;

	allatoms = (Atom*)malloc(numatoms*sizeof(Atom));
	if(allatoms == NULL)
	{
		printf("unable to allocate memory for allatoms\n\n");
		return 0;
	}

	i = (int)fread(allatoms, sizeof(Atom), numatoms, file);

	if(i < numatoms)
	{
		printf("error reading atoms from infile\n\n");
		return 0;
	}

	fclose(file);

	neighbors = (int**)malloc(numatoms*sizeof(int*));
	if(neighbors == NULL)
	{
		printf("unable to allocate memory for neighbors\n\n");
		free(allatoms);
		return 0;
	}

	for(i=0; i<numatoms; i++)
	{
		neighbors[i] = (int*)malloc(MAXNEIGHBORS*sizeof(int));
		if(neighbors[i] == NULL)
		{
			printf("unable to allocate memory for neighbors[%d]\n\n", i);
			for(j=0; j<i; j++)
			{
				free(neighbors[i]);
			}
			free(neighbors);
			free(allatoms);
			return 0;
		}
	}
	for(i=0; i<numatoms; i++)
	{
		for(j=0; j<MAXNEIGHBORS; j++)
		{
			neighbors[i][j] = -1;
		}
	}
	makeneighbors();

	hdt = 0.5 * dt;

	while(keepgoing)
	{
		if(maxdr>0.5*mincutoff)
		{
			makeneighbors();
		}

		interactions();

		correctT();
		velMul = sqrt(desiredT/T);

		integrate();

		iterations--;

		if(T>10000)
		{
			iterations = min(10, iterations);
			outevery = 1;
		}

		if(iterations == outevery*(iterations/outevery))
		{
			file = NULL;
			do
			{
				if(file != NULL) fclose(file);
				outnum++;
				sprintf(buffer,"o%07d.atm", outnum);
				file = fopen(buffer, "r");
			} while(file != NULL);
			printf("%s\n", buffer);
			printf("T=%f\n", T);
			printf("i=%d\n", iterations);
			printf("t=%d\n", (int)time(NULL) - time0);
			printf("\n");
			file = fopen(buffer, "wb");

			tempheader.iterations = iterations;
			tempheader.mincutoff = mincutoff;
			tempheader.dt = dt;
			tempheader.bigX = bigX;
			tempheader.bigY = bigY;
			tempheader.bigZ = bigZ;
			tempheader.numatoms = numatoms;
			tempheader.desiredT = desiredT;
			tempheader.T = T;

			fwrite(&tempheader, sizeof(Headerinfo), 1, file);

			fwrite(allatoms, sizeof(Atom), numatoms, file);

			fclose(file);
		}

		if(iterations > 0)
			keepgoing = 1;
		else
			keepgoing = 0;
	}

	free(allatoms);

	return 1;
}
