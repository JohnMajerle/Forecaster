//*****************************************************************************
//
// Module:  CGeometry.CPP
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
#include "CGeometry.h"
#include "Error.h"



//*****************************************************************************
//
// CGeometry - backbone members
//
//*****************************************************************************

CQual *CGeometry::m_pQuality = 0;

CGeometry::CGeometry() : ma_dGeometry(NUMGEO)
{
   return;
}

CGeometry::CGeometry(const char *id, const char *type, const Double& data) :
	m_sID(id),
	m_sType(type),
	ma_dGeometry(data),
	m_dEstimateCleanTPF(0.0),
	m_dEstimateRawTPF(0.0),
	m_dEstimateHeading(data[0])
{
	return;
}

CGeometry::~CGeometry()
{
   return;
}

ostream& operator<< (ostream& os, CGeometry& geo)
{
	os << geo.ID() << ' ';
	os << geo.Type() << ' ';
	os << geo.Station() << ' ';
	os << geo.XCoord() << ' ';
	os << geo.YCoord() << ' ';
	os << geo.Azimuth() << ' ';
	os << geo.Length() << ' ';
	os << geo.Width() << ' ';
	os << geo.LinearFactor() << ' ';
	os << geo.Recovery();
	return os;
}

//*****************************************************************************
//
// CGeometry - methods
//
//*****************************************************************************

void CGeometry::Quality(double start, double end, double *quality)
{
	if (!m_pQuality)
		throw ForeError(CGeometry_NO_QUALITY,m_sID);

	double x_coord[5],y_coord[5];
	Block(start,end,x_coord,y_coord);
	m_pQuality->BlockEstimate(quality,x_coord,y_coord,
					(const char *)m_sType,ma_dGeometry[7]);
}

double CGeometry::CleanTonsPerFt(double heading)
{
	double clean_tons,raw_tons;
	double quality[NUMQUAL+3];

	if ((fabs(m_dEstimateCleanTPF) <= eps) ||
		(fabs(m_dEstimateHeading-heading) > 500.)) {
		Quality(heading-1.0,heading+1.0,quality);
		if (quality[0] <= eps) 
			throw ForeError(CGeometry_NO_QUALITY,m_sID);
		if (m_pQuality->PercentWashed() > 99.) {
			raw_tons = *(quality+NUMQUAL);
			clean_tons = *(quality+NUMQUAL+1);
		} else {
			raw_tons = *(quality+NUMQUAL) / ((100.-m_pQuality->PercentWashed())/100.);
			clean_tons = *(quality+NUMQUAL) + *(quality+NUMQUAL+1);
		}
		m_dEstimateCleanTPF = clean_tons * 0.5;
		m_dEstimateRawTPF = raw_tons * 0.5;
		m_dEstimateHeading = heading;
	}
	return m_dEstimateCleanTPF;
}

double CGeometry::RawTonsPerFt(double heading)
{
	double clean_tons,raw_tons;
	double quality[26];

	if ((fabs(m_dEstimateRawTPF) <= eps) ||
		(fabs(m_dEstimateHeading-heading) > 500.)) {
		Quality(heading-1.0,heading+1.0,quality);
		if (quality[0] <= eps) 
			throw ForeError(CGeometry_NO_QUALITY,m_sID);
		if (m_pQuality->PercentWashed() > 99.) {
			raw_tons = *(quality+NUMQUAL);
			clean_tons = *(quality+NUMQUAL+1);
		} else {
			raw_tons = *(quality + NUMQUAL) / ((100.-m_pQuality->PercentWashed())/100.);
			clean_tons = *(quality + NUMQUAL) + *(quality + NUMQUAL + 1);
		}
		m_dEstimateCleanTPF = clean_tons * 0.5;
		m_dEstimateRawTPF = raw_tons * 0.5;
		m_dEstimateHeading = heading;
	}
	return m_dEstimateRawTPF;
}

void CGeometry::Block(double start_stn, double end_stn, double *x_coord, double *y_coord)
{
      double x = ma_dGeometry[1];               // x-coord of initial point
      double y = ma_dGeometry[2];               // y_coord of initial point
      double az = ma_dGeometry[3] * PI / 180.;  // azimuth in radians
      double az90 = az + PI/2.;					// direction perpendicular to azimuth
      double width = ma_dGeometry[5];           // width of block
      double start = ma_dGeometry[0];           // station at initial point
      double length_s = start_stn - start;		// distance to start of block
      double length_e = end_stn - start;		// distance to end of block

      x_coord[0] = x + (length_s*sin(az));
      y_coord[0] = y + (length_s*cos(az));
      x_coord[1] = x + (length_e*sin(az));
      y_coord[1] = y + (length_e*cos(az));
      x_coord[2] = x_coord[1] + (width*sin(az90));
      y_coord[2] = y_coord[1] + (width*cos(az90));
      x_coord[3] = x_coord[0] + (width*sin(az90));
      y_coord[3] = y_coord[0] + (width*cos(az90));
      x_coord[4] = x_coord[0];
      y_coord[4] = y_coord[0];
}

//*****************************************************************************

// end of CGeometry.CPP
