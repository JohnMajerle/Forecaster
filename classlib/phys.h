
#ifndef __PHYSICIAN_H
#define __PHYSICIAN_H

#include <stdio.h>
#include "bt.h"

extern void get_line(char *c);

class physician : public BaseType {

public:
	int  match(void *target);
	int  matchID(void *target);
	void print(FILE *output);
	void getInfo();

private:
	char empID[10];
	char name[80];
	char specialty[2];

};

#endif

