//*****************************************************************************
//
// Module:  Reference.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Reference class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "Reference.h"

//*****************************************************************************
//
// Reference - backbone members
//
//*****************************************************************************

Reference::Reference() : m_dStation(0.0),m_pSection(0),m_wDelay(0)
{
	return;
}

Reference::Reference(const char *id, double station, UInt delay) :
	m_sID(id),
	m_dStation(station),
	m_pSection(0),
	m_wDelay(delay)
{
	return;
}

Reference::~Reference()
{
	return;
}

ostream& operator<< (ostream& os, Reference& rf)
{
	os << rf.ID() << ' ';
	os << rf.Station() << ' ';
	os << rf.Delay();
	return os;
}

//*****************************************************************************

// end of Reference.CPP
