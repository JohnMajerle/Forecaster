#include <stdio.h>
#include <string.h>
#include "assign.h"

int assign::match(void *target)
{
	return p_doc->matchID(target);
}

void assign::print(FILE *output)
{
	p_doc->print(output);
	p_pat->print(output);
}
