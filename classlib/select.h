// ****************************************************************************
//
// Module:  select.h
// Author:  Paul Dovenor
//
// Purpose: C++ template class header file for select
//
// Notes:
//
// ****************************************************************************

#ifndef __Select_H
#define __Select_H

#define MAXLIST    10
#define MAXIDCHARS 5

// ****************************************************************************

// class declaration
class Select {

public:

	Select() {}
	Select(char *list);
	virtual ~Select() {}

	// methods to check for entry in table
	int selected(const char *id);
    int count () {
        int i = 0;  for (i = 0; idList[i][0]!='\0'; ++i); return i;
        }

protected:

	char idList[MAXLIST+1][MAXIDCHARS+1];

};

#endif

// ****************************************************************************

// end of Select.h
