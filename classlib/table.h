// ****************************************************************************
//
// Module:  table.h
// Author:  Paul Dovenor
//
// Purpose: C++ template class header file for table
//
// Notes:
//
// ****************************************************************************

#ifndef __Table_H
#define __Table_H

#define NRANGES 10
#define NDIGITS 80
#define BIGGEST 65335

// ****************************************************************************

class range {

friend class Table;     // Table class has access

private:

	// constructor and destructor
	range() : s_min(-1), s_max(BIGGEST) {}
	virtual ~range() {}

	long int s_min;
	long int s_max;

};

// ****************************************************************************

// class declaration
class Table {

public:

	Table() {}
	Table(char *list);
	virtual ~Table() {}

	// methods to check for entry in table
	int selected(int n);

protected:

	range Tlist[NRANGES+1];
	long  Highest;

	long save_range(int n, char *s);

};

#endif

// ****************************************************************************

// end of Table.h
