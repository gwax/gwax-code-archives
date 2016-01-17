#ifndef _types_h_
#define _types_h_

typedef struct
{
	const char* symbol;
	double epsilon;
	double radius;
	double mass;

	const char* pigment;
} AtomType;

enum AtomTypes {fake, C, H};
const AtomType elements[] = {
		{"fake", 1.0, 0.2, 0.1, "White"},
		{"C", 1.0E-21, 0.77E-10, 1.994E-26, "Gray20"},
		{"H", 1.0E-21, 0.32E-10, 1.673E-27, "White"}
	};

#endif
