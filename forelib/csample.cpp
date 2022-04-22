//*****************************************************************************
//
// Module:  CSample.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CGeometry class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include <math.h>
#include <string.h>
#include "CSample.h"

//*****************************************************************************
//
// CSample - backbone members
//
//*****************************************************************************

CSample::CSample()
{
   return;
}

CSample::CSample(const char *id, const char *type, long xcoord, long ycoord,
				 const double *data) :
	m_lXcoord(xcoord),
	m_lYcoord(ycoord)
{
	strcpy(m_sID,id);
	strcpy(m_sType,type);
	memcpy(ma_dQuality,data,sizeof(ma_dQuality));
	return;
}

CSample::~CSample()
{
   return;
}

ostream& operator<< (ostream& os, CSample& geo)
{
	return os;
}

//*****************************************************************************
//
// CSample - methods
//
//*****************************************************************************

//*****************************************************************************

// end of CSample.CPP
