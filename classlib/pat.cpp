#include <stdio.h>
#include <string.h>
#include "pat.h"

int patient::match(void *target)
{
	return !strcmp(name,(char *)target);
}

void patient::print(FILE *output)
{
	fprintf(output,"---------------------------------------------------\n");
	fprintf(output,"      Patient Name: %s\n", name);
	fprintf(output,"       Patient SSN: %s\n", ssn);
	fprintf(output,"  Patient Symptoms: %s\n", symptoms);	
	fprintf(output,"Assigned Specialty: %s\n", specialty);
}

void patient::getInfo()
{
	printf("    Patient Name: "); get_line(name);
	printf("     Patient SSN: "); get_line(ssn);
	printf("Patient Symptoms: "); get_line(symptoms);
	printf("       Specialty: "); get_line(specialty);
}