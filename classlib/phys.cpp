#include <stdio.h>
#include <string.h>
#include "phys.h"

void get_line(char *c)
{
	for (;;) {
		*c = getc(stdin);
		if (*c == '\n') break;
		c++;
	}
	*c = '\0';
}

int physician::match(void *target)
{
	return !strcmp(specialty,(char *)target);
}

int physician::matchID(void *target)
{
	return !strcmp(empID,(char *)target);
}

void physician::print(FILE *output)
{
	fprintf(output,"---------------------------------------------------\n");
	fprintf(output,"      Physician ID: %s\n", empID);
	fprintf(output,"    Physician Name: %s\n", name);
	fprintf(output,"Assigned Specialty: %s\n", specialty);
}

void physician::getInfo()
{
	printf("  Physician ID: "); get_line(empID);
	printf("Physician Name: "); get_line(name);
	printf("     Specialty: "); get_line(specialty);
}