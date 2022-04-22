#ifndef __BaseType_H
#define __BaseType_H

#include <stdio.h>

// ****************************************************************************

class BaseType {
public:
	virtual int match(void *target) = 0;
	virtual void print(FILE *output) = 0;
};

#endif

// ****************************************************************************

// end of bt.h
