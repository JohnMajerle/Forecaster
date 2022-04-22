//*****************************************************************************
//
// Module:  fM.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the fileMgr class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __fileMgr_H
#define __fileMgr_H

#include <fstream>

using namespace std;

// constants and internal data types for class

const int fileMgrMajorVersion = 1;
const int fileMgrMinorVersion = 0;

// error types

enum {
    fileMgr_NO_ERROR,
	fileMgr_OPEN_ERROR
};

// class declaration

class fileMgr {

public:

	// backbone methods
	fileMgr(const char *);
	virtual ~fileMgr();
	fstream& file();

	// get error value
	inline virtual long Error() { return m_Error; }

private:

	fstream m_file;
	long    m_Error;        // error value

	// private copy constructor and operator= (define these and make them
	// public to enable copy and assignment of the class)
	fileMgr(const fileMgr&);
	fileMgr& operator=(const fileMgr&);
};

#endif

//*****************************************************************************

// end of fM.h
