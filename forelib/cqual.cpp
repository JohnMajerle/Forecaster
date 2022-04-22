//*****************************************************************************
//
// Module:  CQual.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CQual class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "CQual.h"

//*****************************************************************************
//
// CQual - backbone members
//
//*****************************************************************************

CQual::CQual(long lSamples) : m_wLWNumber(0),m_wCMNumber(0)
{
   m_pYCoord = new long[lSamples];
   m_pXCoord = new long[lSamples];
   m_pQuality = new int[lSamples];
   m_pSample = new CSample[lSamples];
}

CQual::~CQual()
{
   delete [] m_pYCoord;
   delete [] m_pXCoord;
   delete [] m_pQuality;
   delete [] m_pSample;
}

//*****************************************************************************

// end of CQual.CPP
