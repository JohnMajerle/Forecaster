
#ifndef __PATIENT_H
#define __PATIENT_H

#include <stdio.h>
#include "bt.h"

extern void get_line(char *c);

class patient : public BaseType {

public:
	int  match(void *target);
	void print(FILE *output);
	void getInfo();
	char *spec() { return specialty; }

private:
	char name[80];
	char ssn[15];
	char symptoms[100];
	char specialty[2];

};

#endif

