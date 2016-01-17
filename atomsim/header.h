#ifndef _header_h_
#define _header_h_
typedef struct
{
	int iterations;
	double mincutoff;
	double dt;
	double bigX,bigY,bigZ;
	double desiredT;
	double T;
	int numatoms;
} Headerinfo;

#endif
