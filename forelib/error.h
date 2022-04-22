//*****************************************************************************
//
// Module:  Error.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the ForeError class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __ForeError_H
#define __ForeError_H

#include <stdio.h>
#include "..\classlib\str.h"

enum {
	CSection_NO_GEOMETRY,
	CSection_NO_RATE,
	CGeometry_NO_TPF,
	CGeometry_NO_QUALITY
};

class ForeError
{
public:

	ForeError() {}
	ForeError(long err, String src) : error(err), source(src)
	{
		char errorMsg[300] = {0};
		if (error == CSection_NO_GEOMETRY) 
			sprintf(errorMsg, "Error with section %5.5s: No geometry data.", (const char*)source);
		else if (error == CSection_NO_RATE) 
			sprintf(errorMsg, "Error with section %5.5s: Cannot resolve advance rate.",(const char*)source);
		else if (error == CGeometry_NO_TPF) 
			sprintf(errorMsg, "Error with section %5.5s: No tons/ft.",(const char*)source);
		else if (error == CGeometry_NO_QUALITY) 
			sprintf(errorMsg, "Error with section %5.5s: Unable to calculate quality.",(const char*)source);
        
		message = errorMsg;
	}
	String& ErrorMsg() { return message; }

private:
	long   error;
	String source;
	String message;

};

#endif

//*****************************************************************************

// end of Error.h
