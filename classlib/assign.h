
#ifndef __ASSIGN_H
#define __ASSIGN_H

#include <stdio.h>
#include "bt.h"
#include "phys.h"
#include "pat.h"

class assign : public BaseType {

public:
	assign(physician *new_doc, patient *new_pat) : p_doc(new_doc),p_pat(new_pat) {} 
	int match(void *target);
	void print(FILE *output);
	void getInfo() {}

	physician *doc() { return p_doc; }
	patient *pat() { return p_pat; }

private:
	physician *p_doc;
	patient   *p_pat;
};

#endif

