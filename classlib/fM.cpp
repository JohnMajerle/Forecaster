//*****************************************************************************
//
// Module:  fM.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the fileMgr class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "fM.h"



//*****************************************************************************
//
// fileMgr - backbone members
//
//*****************************************************************************

fileMgr::fileMgr(const char *newFile)
{
	m_file.open(newFile,ios::in|ios::out);
	m_Error = m_file.fail()? fileMgr_OPEN_ERROR: fileMgr_NO_ERROR;
	return;
}

fileMgr::~fileMgr()
{
	m_file.close();
	return;
}

fstream& fileMgr::file()
{
	return m_file;
}

//*****************************************************************************

// end of fM.CPP
