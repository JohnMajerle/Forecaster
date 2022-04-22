//*****************************************************************************
//
// Module:  CResults.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CResults class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include <stdio.h>
#include <math.h>
#include "CResults.h"
#include "Error.h"

static char v4buffer[3000];


//*****************************************************************************
//
// global functions
//
//*****************************************************************************

char *_print_date(Date4& date)
{
      static char buffer[9];
      date.format(buffer,"YY/MM/DD");
      return buffer;
}

char *_print_sta(double sta)
{
      static char buffer[11];
      sprintf(buffer,"%10.0f",sta);
      memmove(buffer,buffer+1,7);
      *(buffer+7) = '+';
      return buffer;
}

void _weighted_quality(double area, double *qual, double new_area, double *new_qual)
{
      int i;

      /* calculate volumes and tonnages */
      
      double volume = area * *qual;
      double new_volume = new_area * *new_qual;
      double raw_tons = *(qual+NUMQUAL);
      double new_raw_tons = *(new_qual+NUMQUAL);
      double clean_tons = *(qual+NUMQUAL+1);
      double new_clean_tons = *(new_qual+NUMQUAL+1);
      double raw_ash_tons = (raw_tons * *(qual+2)) / 100.;
      double new_raw_ash_tons = (new_raw_tons * *(new_qual+2)) / 100.;
      double clean_ash_tons = (clean_tons * *(qual+11)) / 100.;
      double new_clean_ash_tons = (new_clean_tons * *(new_qual+11)) / 100.;

      /* weighting factors */
      
      double area_wt,vol_wt,raw_wt,cln_wt;
      double raw_ash_wt,clean_ash_wt;
      double wt_qual;
      double pre_washed_tons,new_pre_washed_tons,to_be_washed_tons;

      /* accumulate weights */
      
      area_wt = area + new_area;
      vol_wt = volume + new_volume;
      raw_wt = raw_tons + new_raw_tons;
      cln_wt = clean_tons + new_clean_tons;
      raw_ash_wt = raw_ash_tons + new_raw_ash_tons;
      clean_ash_wt = clean_ash_tons + new_clean_ash_tons;

      if (fabs(vol_wt) < eps) return;   // no quality to weight

      /* calculate final weighted qualities */

      /* mining height & seam height */
      
      if (new_area > eps) {
         wt_qual = (*(qual+0) * area) + (*(new_qual+0) * new_area);
         *(qual+0) = wt_qual / area_wt;
         wt_qual = (*(qual+NUMQUAL+2) * area) + (*(new_qual+NUMQUAL+2) * new_area);
         *(qual+NUMQUAL+2) = wt_qual / area_wt;
      }

      /* sg */
      
      if (new_volume > eps) {
         wt_qual = (*(qual+1) * volume) + (*(new_qual+1) * new_volume);
         *(qual+1) = wt_qual / vol_wt;
      }

      /* raw -  ash,sul,btu,vols */
      
      if (new_raw_tons > eps) {
         for (i=2; i<=5; ++i) {
            wt_qual = (*(qual+i) * raw_tons) + (*(new_qual+i) * new_raw_tons);
            *(qual+i) = wt_qual / raw_wt;
         }
      }

      /* raw ash fusion temps - id,h=w,h=1/2w,fluid */
      
      if (new_raw_ash_tons > eps) {
         for (i=6; i<=9; ++i) {
            wt_qual = (*(qual+i) * raw_ash_tons) + (*(new_qual+i) * new_raw_ash_tons);
            *(qual+i) = wt_qual / raw_ash_wt;
         }
      }

      /* yield */
      
      pre_washed_tons = (*(qual+10) > eps)? (clean_tons / *(qual+10)) : 0.0;
      new_pre_washed_tons = (*(new_qual+10) > eps)? (new_clean_tons / *(new_qual+10)) : 0.0;
      to_be_washed_tons = pre_washed_tons + new_pre_washed_tons;
      if (to_be_washed_tons > eps)
         *(qual+10) = cln_wt / to_be_washed_tons;

      /* clean -  ash,sul,btu,vols,fsi,inerts,reflectance */
      
      if (new_clean_tons > eps) {
         for (i=11; i<=17; ++i) {
            wt_qual = (*(qual+i) * clean_tons) + (*(new_qual+i) * new_clean_tons);
            *(qual+i) = wt_qual / cln_wt;
         }
      }

      /* clean -  giesler fluidity */
      
      if (new_clean_tons > eps  &&  *(new_qual+18) >= -eps) {
         if (fabs(*(new_qual+18)) <= eps)   // cannot use a zero fluidity 
            *(new_qual+18) = 1.0;
         if (*(qual+18) < -eps)             // no previous fluidity so replace with new 
            *(qual+18) = *(new_qual+18);
         else {                             // weight average fluidity using logs
            wt_qual = (log(*(qual+18)) * clean_tons) + (log(*(new_qual+18)) * new_clean_tons);
            wt_qual /= cln_wt;
            *(qual+18) = exp(wt_qual);
         }
      }

      /* clean ash fusion temps - id,h=w,h=1/2w,fluid */
      
      if (new_clean_ash_tons > eps) {
         for (i=19; i<=22; ++i) {
            wt_qual = (*(qual+i) * clean_ash_tons) + (*(new_qual+i) * new_clean_ash_tons);
            *(qual+i) = wt_qual / clean_ash_wt;
         }
      }

      /* raw & clean tons */
      
      *(qual+NUMQUAL) = raw_wt;    // raw tons
      *(qual+NUMQUAL+1) = cln_wt;  // clean tons
}

void _product_quality(double *prd_qual, double *qual, double pct_washed)
{
      int    i,j;
      double raw_tons = *(qual+NUMQUAL);
      double clean_tons = *(qual+NUMQUAL+1);
      double wt_tons = raw_tons + clean_tons;
      double wt_qual;
      double as_mined_tons;

      if (wt_tons > eps) {
         for (i=2,j=11; i<=4; ++i,++j) {
            wt_qual = (*(qual+i)*raw_tons) + (*(qual+j)*clean_tons);
            *(prd_qual+i-1) = wt_qual / wt_tons;
         }
         as_mined_tons = raw_tons / ((100.-pct_washed)/100.);
         if (as_mined_tons > eps)
            *(prd_qual+0) = (wt_tons / as_mined_tons) * 100.;
         *(prd_qual+4) = as_mined_tons;
      } else {
         for (i=0; i<5; ++i)
            *(prd_qual+i) = 0.0;
      }
}


static int _print_qual(char *buffer, int width, int precision, double value)
{
      int ret;
      static char *blanks="                    ";

      if (value > eps)
         ret = sprintf(buffer,"%*.*f ",width,precision,value);
      else
         ret = sprintf(buffer,"%*.*s ",width,width,blanks);
      return ret;
}

static int _field_qual(char *buffer, int width, int precision, double value)
{
      int ret;
      static char *blanks="                    ";

      if (value > eps)
         ret = sprintf(buffer,"!%*.*f!,",width,precision,value);
      else
         ret = sprintf(buffer,"!%*.*s!,",width,width,blanks);
      return ret;
}

char *_report_qual(double *qual, char r_coal, char r_type,
						  double pct_washed)
{
      int    i,j,pos;
      double product_tons;
      double prd_qual[5];
      double as_mined_tons;
      double so2=0.0;
      static char buffer[200];

      /* data common to all types of report */

      if (pct_washed < 100.-eps) {
         _product_quality(prd_qual,qual,pct_washed);
         as_mined_tons = prd_qual[4];
      } else 
         as_mined_tons = qual[NUMQUAL];
      j  = _print_qual(buffer,9,0,as_mined_tons);		   // as-mined tons

      if (r_coal == 'P') {    // product tons      
         product_tons = qual[NUMQUAL] + qual[NUMQUAL+1];
         j += _print_qual(buffer+j,10,0,product_tons);     // product tons
      } else 
         j += _print_qual(buffer+j,10,0,qual[NUMQUAL+1]);  // clean tons

      j += _print_qual(buffer+j,6,2,qual[0]);              // mining height
      j += _print_qual(buffer+j,6,2,qual[NUMQUAL+2]);      // seam height
      j += _print_qual(buffer+j,5,2,qual[1]);              // s.g.

      /* regular production data */
      
      if (r_coal == 'C'  ||  r_coal == 'R') {
         pos = (r_coal == 'C')? 11: 2;   // raw or clean
         j += _print_qual(buffer+j,6,2,qual[10]);          // yield
         j += _print_qual(buffer+j,6,2,qual[pos++]);       // ash
         j += _print_qual(buffer+j,4,2,qual[pos++]);       // sul
         j += _print_qual(buffer+j,5,0,qual[pos]);         // BTU
		 if (qual[pos] > eps) {
            so2 = (20000*qual[pos-1]) / qual[pos];
            j += _print_qual(buffer+j,5,2,so2);            // so2
         }
      } else if (r_coal == 'P') {        // product
         j += _print_qual(buffer+j,6,2,prd_qual[0]);       // yield
         j += _print_qual(buffer+j,6,2,prd_qual[1]);       // ash
         j += _print_qual(buffer+j,4,2,prd_qual[2]);       // sul
         j += _print_qual(buffer+j,5,0,prd_qual[3]);       // BTU
         if (prd_qual[3] > eps) {
            so2 = (20000*prd_qual[2]) / prd_qual[3];
            j += _print_qual(buffer+j,5,2,so2);            // so2
         }
      }

      /* met coal */

      if (r_type == 'M'  &&  r_coal == 'R') {
         j += _print_qual(buffer+j-6,5,2,qual[5]);         // vol instead of so2
      } else if (r_type == 'M'  &&  r_coal == 'C') {
         j += _print_qual(buffer+j,5,2,qual[15]);          // fsi
         j += _print_qual(buffer+j,5,2,qual[14]);          // vol
         j += _print_qual(buffer+j,5,2,qual[16]);          // inerts
         j += _print_qual(buffer+j,5,2,qual[17]);          // refectance
         j += _print_qual(buffer+j,6,0,qual[18]);          // fluidity
      }

      /* steam coal: ash fusion temperatures */
      
      if (r_type == 'S'  &&  r_coal != 'P') {
         pos = (r_coal == 'C')? 19: 6;   // raw or clean 
         for (i=0; i<4; ++i)
            j += _print_qual(buffer+j,6,0,qual[pos+i]);    // ash fusion temps
      }
      
      return buffer;
}

char *_xml_qual(double *qual, double pct_washed)
{
	  int j;
      int    partial_wash = (pct_washed < 100.-eps);
      double so2=0.0;
      static char buffer[1000];

	  j = sprintf(buffer,"<Raw_Tons>%.0f</Raw_Tons>\n",qual[NUMQUAL]);
	  j += sprintf(buffer+j,"<Clean_Tons>%.0f</Clean_Tons>\n",qual[NUMQUAL+1]);
	  j += sprintf(buffer+j,"<Mine_Hgt>%.2f</Mine_Hgt>\n",qual[0]);
      j += sprintf(buffer+j,"<Seam_Hgt>%.2f</Seam_Hgt>\n",qual[NUMQUAL+2]);
      j += sprintf(buffer+j,"<SpGr>%.2f</SpGr>\n",qual[1]);
      
	  j += sprintf(buffer+j,"<Raw_Ash>%.2f</Raw_Ash>\n",qual[2]);
      j += sprintf(buffer+j,"<Raw_Sul>%.2f</Raw_Sul>\n",qual[3]);
      j += sprintf(buffer+j,"<Raw_BTU>%.0f</Raw_BTU>\n",qual[4]);
      so2 = (qual[4] > eps)? (20000*qual[3]) / qual[4]: 0.0;
	  j += sprintf(buffer+j,"<Raw_SO2>%.2f</Raw_SO2>\n",so2);
      j += sprintf(buffer+j,"<Raw_Vol>%.2f</Raw_Vol>\n",qual[5]);
	  
	  j += sprintf(buffer+j,"<Raw_AFT-ID>%.0f</Raw_AFT-ID>\n",qual[6]);
	  j += sprintf(buffer+j,"<Raw_AFT-HW>%.0f</Raw_AFT-HW>\n",qual[7]);
	  j += sprintf(buffer+j,"<Raw_AFT-H1/2W>%.0f</Raw_AFT-H1/2W>\n",qual[8]);
	  j += sprintf(buffer+j,"<Raw_AFT-FLD>%.0f</Raw_AFT-FLD>\n",qual[9]);

	  j += sprintf(buffer+j,"<Yield>%.2f</Yield>\n",qual[10]);
	  j += sprintf(buffer+j,"<Clean_Ash>%.2f</Clean_Ash>\n",qual[11]);
      j += sprintf(buffer+j,"<Clean_Sul>%.2f</Clean_Sul>\n",qual[12]);
      j += sprintf(buffer+j,"<Clean_BTU>%.0f</Clean_BTU>\n",qual[13]);
      so2 = (qual[13] > eps)? (20000*qual[12]) / qual[13]: 0.0;
	  j += sprintf(buffer+j,"<Clean_SO2>%.2f</Clean_SO2>\n",so2);
      j += sprintf(buffer+j,"<Clean_Vol>%.2f</Clean_Vol>\n",qual[14]);

	  j += sprintf(buffer+j,"<Clean_Fsi>%.2f</Clean_Fsi>\n",qual[15]);
	  j += sprintf(buffer+j,"<Clean_Inerts>%.2f</Clean_Inerts>\n",qual[16]);
	  j += sprintf(buffer+j,"<Clean_Reflectance>%.2f</Clean_Reflectance>\n",qual[17]);
	  j += sprintf(buffer+j,"<Clean_Fluidity>%.0f</Clean_Fluidity>\n",qual[18]);
	  
	  j += sprintf(buffer+j,"<Clean_AFT-ID>%.0f</Clean_AFT-ID>\n",qual[19]);
	  j += sprintf(buffer+j,"<Clean_AFT-HW>%.0f</Clean_AFT-HW>\n",qual[20]);
	  j += sprintf(buffer+j,"<Clean_AFT-H1/2W>%.0f</Clean_AFT-H1/2W>\n",qual[21]);
	  j += sprintf(buffer+j,"<Clean_AFT-FLD>%.0f</Clean_AFT-FLD>\n",qual[22]);

      if (partial_wash) {
		 double prd_qual[5];
		 _product_quality(prd_qual,qual,pct_washed);
         double as_mined_tons = prd_qual[4];
         double product_tons = qual[NUMQUAL] + qual[NUMQUAL+1];
		 j += sprintf(buffer+j,"<As-Mined_Tons>%.0f</As-Mined_Tons>\n",as_mined_tons);
		 j += sprintf(buffer+j,"<Product_Tons>%.0f</Product_Tons>\n",product_tons);
 	     j += sprintf(buffer+j,"<Product_Yield>%.2f</Product_Yield>\n",prd_qual[0]);
	     j += sprintf(buffer+j,"<Product_Ash>%.2f</Product_Ash>\n",prd_qual[1]);
         j += sprintf(buffer+j,"<Product_Sul>%.2f</Product_Sul>\n",prd_qual[2]);
         j += sprintf(buffer+j,"<Product_BTU>%.0f</Product_BTU>\n",prd_qual[3]);
         so2 = (prd_qual[3] > eps)? (20000*prd_qual[2]) / prd_qual[3]: 0.0;
	     j += sprintf(buffer+j,"<Product SO2>%.2f</Product_SO2>\n",so2);
      }
      
      return buffer;
}

char *_item_qual(double *qual, double pct_washed)
{
	  int j;
      int    partial_wash = (pct_washed < 100.-eps);
      double so2=0.0;
      static char buffer[500];

      j  = _field_qual(buffer,  9,0,qual[NUMQUAL]);			// raw tons
      j += _field_qual(buffer+j,10,0,qual[NUMQUAL+1]);		// clean tons
      j += _field_qual(buffer+j,6,2,qual[0]);				// mining height
      j += _field_qual(buffer+j,6,2,qual[NUMQUAL+2]);		// seam height
      j += _field_qual(buffer+j,5,2,qual[1]);				// s.g.

      j += _field_qual(buffer+j,6,2,qual[2]);				// raw ash
      j += _field_qual(buffer+j,4,2,qual[3]);				// raw sul
      j += _field_qual(buffer+j,5,0,qual[4]);				// raw BTU
	  if (qual[4] > eps)
        so2 = (20000*qual[3]) / qual[4];
	  else
		so2 = 0.0;
      j += _field_qual(buffer+j,5,2,so2);					// raw so2
	  j += _field_qual(buffer+j,5,2,qual[5]);				// raw vol
	  for (int i=0; i<4; ++i)
		j += _field_qual(buffer+j,6,0,qual[6+i]);			// raw ash fusion temps

      j += _field_qual(buffer+j,6,2,qual[10]);				// yield
      j += _field_qual(buffer+j,6,2,qual[11]);				// clean ash
      j += _field_qual(buffer+j,4,2,qual[12]);				// clean sul
      j += _field_qual(buffer+j,5,0,qual[13]);				// clean BTU
	  if (qual[13] > eps)
        so2 = (20000*qual[12]) / qual[13];
	  else
		so2 = 0.0;
      j += _field_qual(buffer+j,5,2,so2);					// clean so2

      j += _field_qual(buffer+j,5,2,qual[14]);				// clean vol
	  j += _field_qual(buffer+j,5,2,qual[15]);				// clean fsi
      j += _field_qual(buffer+j,5,2,qual[16]);				// clean inerts
      j += _field_qual(buffer+j,5,2,qual[17]);				// clean reflectance
      j += _field_qual(buffer+j,6,0,qual[18]);				// clean fluidity
      for (int i=0; i<4; ++i)
         j += _field_qual(buffer+j,6,0,qual[19+i]);			// clean ash fusion temps

      if (partial_wash) {
		 double prd_qual[5];
		 _product_quality(prd_qual,qual,pct_washed);
         double as_mined_tons = prd_qual[4];
         double product_tons = qual[NUMQUAL] + qual[NUMQUAL+1];
         j += _field_qual(buffer+j,9,0,as_mined_tons);		// as-mined tons
         j += _field_qual(buffer+j,10,0,product_tons);		// product tons
         j += _field_qual(buffer+j,6,2,prd_qual[0]);		// product yield
         j += _field_qual(buffer+j,6,2,prd_qual[1]);		// product ash
         j += _field_qual(buffer+j,4,2,prd_qual[2]);		// product sul
         j += _field_qual(buffer+j,5,0,prd_qual[3]);		// product BTU
         if (prd_qual[3] > eps) 
            so2 = (20000*prd_qual[2]) / prd_qual[3];
		 else
			so2 = 0.0;
         j += _field_qual(buffer+j,5,2,so2);				// product so2
      }
      
      return buffer;
}


static char *_store_qual(double *qual)
{
      int j;
      static char buffer[200];
      j = sprintf(buffer,"%5.2f ",*qual);                  // mining height
      j += sprintf(buffer+j,"%4.2f ",*(qual+1));           // s.g.
      j += sprintf(buffer+j,"%5.2f ",*(qual+2));           // raw ash
      j += sprintf(buffer+j,"%4.2f ",*(qual+3));           // raw sul
      j += sprintf(buffer+j,"%5.0f ",*(qual+4));           // raw BTU
      j += sprintf(buffer+j,"%5.2f ",*(qual+5));           // raw vol
      j += sprintf(buffer+j,"%4.0f ",*(qual+6));           // raw aft - id
      j += sprintf(buffer+j,"%4.0f ",*(qual+7));           // raw aft - hw
      j += sprintf(buffer+j,"%4.0f ",*(qual+8));           // raw aft - 2hw
      j += sprintf(buffer+j,"%4.0f ",*(qual+9));           // raw aft - fluid
      j += sprintf(buffer+j,"%5.2f ",*(qual+10));          // yield
      j += sprintf(buffer+j,"%5.2f ",*(qual+11));          // clean ash
      j += sprintf(buffer+j,"%4.2f ",*(qual+12));          // clean sul
      j += sprintf(buffer+j,"%5.0f ",*(qual+13));          // clean BTU
      j += sprintf(buffer+j,"%5.2f ",*(qual+14));          // clean vol
      j += sprintf(buffer+j,"%5.2f ",*(qual+15));          // clean fsi
      j += sprintf(buffer+j,"%5.2f ",*(qual+16));          // clean inerts
      j += sprintf(buffer+j,"%4.2f ",*(qual+17));          // clean relectance
      j += sprintf(buffer+j,"%6.0f ",*(qual+18));          // clean fluidity
      j += sprintf(buffer+j,"%4.0f ",*(qual+19));          // clean aft - id
      j += sprintf(buffer+j,"%4.0f ",*(qual+20));          // clean aft - hw
      j += sprintf(buffer+j,"%4.0f ",*(qual+21));          // clean aft - 2hw
      j += sprintf(buffer+j,"%4.0f ",*(qual+22));          // clean aft - fluid
      j += sprintf(buffer+j,"%6.0f ",*(qual+23));		   // raw tons
      j += sprintf(buffer+j,"%6.0f ",*(qual+24));		   // clean tons
      j += sprintf(buffer+j,"%5.2f ",*(qual+25));		   // seam height
      return buffer;
}

//*****************************************************************************
//
// CResults - backbone members
//
//*****************************************************************************

CResults::CResults() :
	m_pPanel(0),
	m_dStart(0.0),
	m_dEnd(0.0),
	m_wDays(0),
	m_lDays(0),
	m_wLate(0),
	m_wLead(0),
	m_wMaxUsed(0),
	m_dShifts(0.0),
	m_dArea(0.0),
	m_dClean(0.0),
	m_dRaw(0.0),
	m_wError(CResults_NO_ERROR)
{
	memset(m_dQuality,0,sizeof(m_dQuality));
	m_dQuality[18] = -1;
	return;
}

CResults::CResults(const char *id, const char *type, double start, double end,
				   const char *unit, const char *super, Date4& begin, ULong days,
				   UInt late, UInt lead, UChar max_used, double shifts, double area,
				   double clean, double raw) : 
	m_pPanel(0),
	m_sID(id),
	m_sType(type),
	m_dStart(start),
	m_dEnd(end),
	m_sUnit(unit),
	m_sSuper(super),
	m_Begin(begin),
	m_End(begin),
	m_lDays(days),
	m_wLate(late),
	m_wLead(lead),
	m_wMaxUsed(max_used),
	m_dShifts(shifts),
	m_dArea(area),
	m_dClean(clean),
	m_dRaw(raw),
	m_wError(CResults_NO_ERROR)
{
	m_wDays = _32BitCount(days);

	if (late > maxUint) m_wLate = maxUint;
	if (lead > maxUint) m_wLead = maxUint;

	memset(m_dQuality,0,sizeof(m_dQuality));
	m_dQuality[18] = -1;

	return;
}

CResults::~CResults()
{
	return;
}

//*****************************************************************************
//
// CResults - accessors
//
//*****************************************************************************

void CResults::UnitData(double pct_washed,
						double& ftpms,
						double& ftpday,
						double& raw_tnspms,
						double& clean_tnspms,
						double& product_tnspms,
						double& raw_tnspft,
						double& clean_tnspft,
						double& product_tnspft,
						double& raw_tnspday,
						double& clean_tnspday,
						double& product_tnspday,
						double& mspday)
{
	double ms = m_dShifts;
	double raw_tons = m_dQuality[NUMQUAL];
	double clean_tons = m_dQuality[NUMQUAL+1];
	double distance = fabs(m_dEnd - m_dStart);
	double linear = distance * m_pPanel->LinearFactor();

	if (Super()) {
		ms *= .5; raw_tons *= .5; clean_tons *=.5;
	}

	ftpms = linear / ms;
	ftpday = linear / m_wDays;
	raw_tnspms = raw_tons / ms;
	clean_tnspms = clean_tons / ms;
	raw_tnspft = raw_tons / linear;
	clean_tnspft = clean_tons / linear;
	raw_tnspday = raw_tons / m_wDays;
	clean_tnspday = clean_tons / m_wDays;
	mspday = ms / m_wDays;

	if (pct_washed < 100.-eps) {
		double as_mined_tons = raw_tons / ((100.-pct_washed)/100.);
		double product_tons = raw_tons + clean_tons;
		raw_tnspms = as_mined_tons / ms;
		product_tnspms = product_tons / ms;
		raw_tnspft = as_mined_tons / linear;
		product_tnspft = product_tons / linear;
		raw_tnspday = as_mined_tons / m_wDays;
		product_tnspday = product_tons / m_wDays;
	}
}

//*****************************************************************************
//
// CResults - implementors
//
//*****************************************************************************


void CResults::Print(fileMgr& Store)
{
	int nDays = 1, used = 0;
	static char *blank = "XXXXX";
	static char ln_buf[301];

	char date[9];
	m_Begin.format(date,"CCYYMMDD");

    int j = sprintf(ln_buf,"%8.8s ",date);
    j += sprintf(ln_buf+j,"%-6.6s ",(const char*)m_sID);
    j += sprintf(ln_buf+j,"%2.2s ",(const char*)m_sType);
	j += sprintf(ln_buf+j,"%-5.5s ",(const char*)m_sUnit);
	if (m_sSuper.length())
		j += sprintf(ln_buf+j,"%5.5s ",(const char*)m_sSuper);
	else
		j += sprintf(ln_buf+j,"%5.5s ",blank);
	j += sprintf(ln_buf+j,"%2.2d ",nDays);
	j += sprintf(ln_buf+j,"%6.2f ",m_dShifts);
	j += sprintf(ln_buf+j,"%8.2f ",m_dStart);
	j += sprintf(ln_buf+j,"%8.2f ",m_dEnd);
	j += sprintf(ln_buf+j,"%10.2f ",m_dArea);
	j += sprintf(ln_buf+j,"%12lu ",m_lDays);
	j += sprintf(ln_buf+j,"%3u ",m_wLate);
	j += sprintf(ln_buf+j,"%3u ",m_wLead);
	j += sprintf(ln_buf+j,"%1u ",m_wMaxUsed);
	j += sprintf(ln_buf+j,"%s",_store_qual(m_dQuality));
	Store.file() << ln_buf << endl;
}

// [NEEDSWORK] Phocaz: Does not compile
/*
void CResults::Scan(fileMgr& Store)
{
	int nDays;
	static char s_buffer[5][10];
	static char ln_buf[301];

	fstream fin = Store.file();
	fin.getline(ln_buf,300,'\n');
	if (fin.fail()) {
		m_wError = CResults_FILE_ERROR;
		return;
	}
	if (fin.eof()) {
		m_wError = CResults_EOF;
		return;
	}
	int nchar = fin.gcount();

	int rc = sscanf(ln_buf,"%s %s %s %s %s %d %lf %lf %lf %lf %lu %u %u %u",
					s_buffer[0],
					s_buffer[1],
					s_buffer[2],
					s_buffer[3],
					s_buffer[4],
					&nDays,
					&m_dShifts,
					&m_dStart,
					&m_dEnd,
					&m_dArea,
					&m_lDays,
					&m_wLate,
					&m_wLead,
					&m_wMaxUsed);
	if (rc == EOF  ||  rc != 14) {
		m_wError = CResults_RECORD_ERROR;
		return;
	}

	m_pPanel = 0;
	m_sID = s_buffer[1];
	m_sType = s_buffer[2];
	m_sUnit = s_buffer[3];
	m_sSuper = s_buffer[4];
	m_Begin.assign(s_buffer[0]);
	m_End.assign(s_buffer[0]);
	m_wError = CResults_NO_ERROR;
}
*/

void CResults::SetGeometry(Array<CGeometry>& a_Geometry)
{
    // [NEEDSWORK] Phocaz: compiler error inside xutility

	//Pointer<CGeometry> first(a_Geometry,0);
	//Pointer<CGeometry> beyond(a_Geometry,a_Geometry.size());
	//Pointer<CGeometry> p = find(first,beyond,String(m_sID));
	//if (p == beyond)
	//	throw ForeError(CSection_NO_GEOMETRY,m_sID);
	//m_pPanel = &(*p);
}

void CResults::SetQuality()
{
	if (!m_pPanel)
		throw ForeError(CSection_NO_GEOMETRY,m_sID);

	m_pPanel->Quality(m_dStart,m_dEnd,m_dQuality);
	if (fabs(m_dQuality[0]) <= eps) 
		throw ForeError(CGeometry_NO_QUALITY,m_sID);
}

char* CResults::SectionReport(Select& unit_list, char coal, char type, double pct_washed)
{
    int j = sprintf(v4buffer,"%s ",_print_date(m_Begin));
    j += sprintf(v4buffer+j,"%-6s",(const char *)m_sID);
	char extra_unit = Super()? '+': ' ';
	if (unit_list.selected(m_sUnit))
		j += sprintf(v4buffer+j,"(%-5s)%c ",(const char *)m_sUnit,extra_unit);
	else	// assume a super section and extra unit in list
		j += sprintf(v4buffer+j,"(%-5s)%c ",(const char *)m_sSuper,extra_unit);
    j += sprintf(v4buffer+j,"%2.2d ",_32BitCount(m_lDays));
    j += sprintf(v4buffer+j,"%6.2f  ",m_dShifts);
    j += sprintf(v4buffer+j,"%s  ",_print_sta(m_dStart));
    j += sprintf(v4buffer+j,"%s  ",_print_sta(m_dEnd));

	double area = m_dArea;
	double quality[26];
	memcpy(quality,m_dQuality,sizeof(m_dQuality));
	if (Super()  &&  (!unit_list.selected(m_sUnit) || !unit_list.selected(m_sSuper))) {
		// super section: assume only one unit in list
		area *= .5;
		quality[NUMQUAL] *= .5;
		quality[NUMQUAL+1] *= .5;
	}
    j += sprintf(v4buffer+j,"%8.2f ",area/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_report_qual(quality,coal,type,pct_washed));
	return v4buffer;
}

char* CResults::UnitReport(int super, char coal, char type, double pct_washed)
{
	// id
	//
    int j = sprintf(v4buffer,"%s ",_print_date(m_Begin));
	const char *unit = super? (const char *)m_sSuper: (const char *)m_sUnit;
    j += sprintf(v4buffer+j,"%-5s (%-6s) ",unit,(const char *)m_sID);
    j += sprintf(v4buffer+j,"%2.2d       ",_32BitCount(m_lDays));

	// stats
	//
	char flag_ftpms = _getBitMask(m_wMaxUsed,0)? '*': ' ';
	char flag_tnspms = _getBitMask(m_wMaxUsed,1)? '*': ' ';
	double ftpms,ftpday,mspday;
	double raw_tnspms,clean_tnspms,product_tnspms=0;
	double raw_tnspft,clean_tnspft,product_tnspft=0;
	double raw_tnspday,clean_tnspday,product_tnspday=0;
	UnitData(pct_washed,ftpms,ftpday,raw_tnspms,clean_tnspms,product_tnspms,
		     raw_tnspft,clean_tnspft,product_tnspft,
			 raw_tnspday,clean_tnspday,product_tnspday,mspday);
	double tnspms,tnspft;
	if (coal == 'C') { tnspms = clean_tnspms; tnspft = clean_tnspft; }
	if (coal == 'R') { tnspms = raw_tnspms; tnspft = raw_tnspft; }
	if (coal == 'P') { tnspms = product_tnspms; tnspft = product_tnspft; }
    j += sprintf(v4buffer+j,"%6.2f%c",ftpms,flag_ftpms);
	j += sprintf(v4buffer+j,"%6.0f%c",tnspms,flag_tnspms);
	j += sprintf(v4buffer+j,"%6.2f ",tnspft);
	j += sprintf(v4buffer+j,"%5.2f ",mspday);

	// quality
	//
	double area = m_dArea;
	double quality[26];
	memcpy (quality,m_dQuality,sizeof(quality));
	if (Super()) {
		area *=.5;
		quality[NUMQUAL] *=.5;
		quality[NUMQUAL+1] *=.5;
	}
    j += sprintf(v4buffer+j,"%7.2f ",area/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_report_qual(quality,coal,type,pct_washed));
	return v4buffer;
}

char* CResults::SectionItem(Select& unit_list, double pct_washed)
{
    int j = sprintf(v4buffer,"!%s!,",_print_date(m_Begin));
    j += sprintf(v4buffer+j,"!%s!,",(const char *)m_sID);
	if (unit_list.selected(m_sUnit))
		j += sprintf(v4buffer+j,"!%s!,",(const char *)m_sUnit);
	else
		j += sprintf(v4buffer+j,"! !,");
	if (Super() && unit_list.selected(m_sSuper))
		j += sprintf(v4buffer+j,"!%s!,",(const char*)m_sSuper);
	else
		j += sprintf(v4buffer+j,"! !,");
    j += sprintf(v4buffer+j,"!%2.2d!,",_32BitCount(m_lDays));
    j += sprintf(v4buffer+j,"!%6.2f!,",m_dShifts);
    j += sprintf(v4buffer+j,"!%s!,",_print_sta(m_dStart));
    j += sprintf(v4buffer+j,"!%s!,",_print_sta(m_dEnd));

	double area = m_dArea;
	double quality[26];
	memcpy(quality,m_dQuality,sizeof(m_dQuality));
	if (Super()  &&  (!unit_list.selected(m_sUnit) || !unit_list.selected(m_sSuper))) {
		// super section: assume only one unit in list
		area *= .5;
		quality[NUMQUAL] *= .5;
		quality[NUMQUAL+1] *= .5;
	}
    j += sprintf(v4buffer+j,"!%8.2f!,",area/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_item_qual(quality,pct_washed));
	return v4buffer;
}

char* CResults::SectionXML(double pct_washed)
{
	int j = sprintf(v4buffer,"<Result>\n");
    j += sprintf(v4buffer+j,"<Start_Date>%s</Start_Date>\n",_print_date(m_Begin));
    j += sprintf(v4buffer+j,"<Section>%s</Section>\n",(const char *)m_sID);
	j += sprintf(v4buffer+j,"<Mine_Unit>%s</Mine_Unit>\n",(const char *)m_sUnit);
	if (Super())
		j += sprintf(v4buffer+j,"<Super>%s</Super>\n",(const char*)m_sSuper);
	else
		j += sprintf(v4buffer+j,"<Super></Super>\n");
	j += sprintf(v4buffer+j,"<Work_Days>%d</Work_Days>\n",m_wDays);
    j += sprintf(v4buffer+j,"<Days>%d</Days>\n",_32BitCount(m_lDays));
    j += sprintf(v4buffer+j,"<Shifts>%.2f</Shifts>\n",m_dShifts);
	char *start = _print_sta(m_dStart);
	trim(start);
    j += sprintf(v4buffer+j,"<Start_Station>%s</Start_Station>\n",start);
	char *end = _print_sta(m_dEnd);
	trim(end);
    j += sprintf(v4buffer+j,"<End_Station>%s</End_Station>\n",end);
    j += sprintf(v4buffer+j,"<Acres>%.2f</Acres>\n",m_dArea/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_xml_qual(m_dQuality,pct_washed));
	j += sprintf(v4buffer+j,"</Result>\n");
	return v4buffer;
}

char* CResults::SectionSummaryItem()
{
    int j;
	double ftpday = fabs(m_dEnd - m_dStart) / m_wDays;
	double rtnspday = m_dQuality[NUMQUAL] / m_wDays;
	double cltnspday = m_dQuality[NUMQUAL+1] / m_wDays;

    j = sprintf(v4buffer,"!%s!,",(const char *)m_sID);
	j += sprintf(v4buffer+j,"!%s!,",(const char *)m_sUnit);
	if (Super())
		j += sprintf(v4buffer+j,"!%s!,",(const char*)m_sSuper);
	else
		j += sprintf(v4buffer+j,"! !,");
	j += sprintf(v4buffer+j,"!%s!,",_print_date(m_Begin));
	j += sprintf(v4buffer+j,"!%s!,",_print_date(m_End));
    j += sprintf(v4buffer+j,"!%3i!,",m_wDays);
	j += sprintf(v4buffer+j,"!%3u!,",m_wLate);
	j += sprintf(v4buffer+j,"!%5.2f!,",m_dQuality[NUMQUAL+2]);
    j += sprintf(v4buffer+j,"!%s!,",_print_sta(m_dStart));
    j += sprintf(v4buffer+j,"!%s!,",_print_sta(m_dEnd));
    j += sprintf(v4buffer+j,"!%7.2f!,",m_dArea/sqft2acres);
	j += sprintf(v4buffer+j,"!%6.2f!,",ftpday);
	j += sprintf(v4buffer+j,"!%7.0f!,",m_dQuality[NUMQUAL]);
	j += sprintf(v4buffer+j,"!%7.0f!,",m_dQuality[NUMQUAL+1]);
	j += sprintf(v4buffer+j,"!%5.0f!,",rtnspday);
	j += sprintf(v4buffer+j,"!%5.0f!,",cltnspday);
	return v4buffer;
}

char* CResults::UnitItem(int super, double pct_washed)
{
    int j = sprintf(v4buffer,"!%s!,",_print_date(m_Begin));
	const char *unit = super? (const char *)m_sSuper: (const char *)m_sUnit;
    j += sprintf(v4buffer+j,"!%s!,",unit);
    j += sprintf(v4buffer+j,"!%s!,",(const char *)m_sID);
    j += sprintf(v4buffer+j,"!%2.2d!,! !,",_32BitCount(m_lDays));

	double ftpms,ftpday,mspday;
	double raw_tnspms,clean_tnspms,product_tnspms=0;
	double raw_tnspft,clean_tnspft,product_tnspft=0;
	double raw_tnspday,clean_tnspday,product_tnspday=0;
	UnitData(pct_washed,ftpms,ftpday,raw_tnspms,clean_tnspms,product_tnspms,
		     raw_tnspft,clean_tnspft,product_tnspft,
			 raw_tnspday,clean_tnspday,product_tnspday,mspday);
	j += sprintf(v4buffer+j,"!%6.2f!,",ftpms);
	j += sprintf(v4buffer+j,"!%6.0f!,",raw_tnspms);
	j += sprintf(v4buffer+j,"!%6.0f!,",clean_tnspms);
	j += sprintf(v4buffer+j,"!%6.0f!,",product_tnspms);
	j += sprintf(v4buffer+j,"!%6.2f!,",raw_tnspft);
	j += sprintf(v4buffer+j,"!%6.2f!,",clean_tnspft);
	j += sprintf(v4buffer+j,"!%6.2f!,",product_tnspft);
	j += sprintf(v4buffer+j,"!%5.2f!,",mspday);

	double area = m_dArea;
	double quality[26];
	memcpy(quality,m_dQuality,sizeof(m_dQuality));
	if (Super()) {
		area *= .5;
		quality[NUMQUAL] *= .5;
		quality[NUMQUAL+1] *= .5;
	}
    j += sprintf(v4buffer+j,"!%8.2f!,",area/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_item_qual(quality,pct_washed));
	return v4buffer;
}

char* CResults::UnitXML(double pct_washed)
{
	// main unit data
	int j = sprintf(v4buffer,"<Result>\n");
	j += sprintf(v4buffer+j,"<Start_Date>%s</Start_Date>\n",_print_date(m_Begin));
	j += sprintf(v4buffer+j,"<Mine_Unit>%s</Mine_Unit>\n",(const char *)m_sUnit);
	j += sprintf(v4buffer+j,"<Section>%s</Section>\n",(const char *)m_sID);
	j += sprintf(v4buffer+j,"<Work_Days>%d</Work_Days>\n",m_wDays);
    j += sprintf(v4buffer+j,"<Days>%d</Days>\n",_32BitCount(m_lDays));

	double ftpms,ftpday,mspday;
	double raw_tnspms,clean_tnspms,product_tnspms=0;
	double raw_tnspft,clean_tnspft,product_tnspft=0;
	double raw_tnspday,clean_tnspday,product_tnspday=0;
	UnitData(pct_washed,ftpms,ftpday,raw_tnspms,clean_tnspms,product_tnspms,
		     raw_tnspft,clean_tnspft,product_tnspft,
			 raw_tnspday,clean_tnspday,product_tnspday,mspday);
	j += sprintf(v4buffer+j,"<FT_PMS>%.2f</FT_PMS>",ftpms);
	j += sprintf(v4buffer+j,"<Raw_Tons_PMS>%.0f</Raw_Tons_PMS>",raw_tnspms);
	j += sprintf(v4buffer+j,"<Clean_Tons_PMS>%.0f</Clean_Tons_PMS>",clean_tnspms);
	j += sprintf(v4buffer+j,"<Prd_Tons_PMS>%.0f</Prd_Tons_PMS>",product_tnspms);
	j += sprintf(v4buffer+j,"<Raw_Tons_PFT>%.2f</Raw_Tons_PFT>",raw_tnspft);
	j += sprintf(v4buffer+j,"<Clean_Tons_PFT>%.2f</Clean_Tons_PFT>",clean_tnspft);
	j += sprintf(v4buffer+j,"<Prd_Tons_PFT>%.2f</Prd_Tons_PFT>",product_tnspft);
	j += sprintf(v4buffer+j,"<MS_PDay>%.2f</MS_PDay>",mspday);

	double area = m_dArea;
	double quality[26];
	memcpy(quality,m_dQuality,sizeof(m_dQuality));
	if (Super()) {
		area *= .5;
		quality[NUMQUAL] *= .5;
		quality[NUMQUAL+1] *= .5;
	}
    j += sprintf(v4buffer+j,"<Acres>%.2f</Acres>\n",area/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_xml_qual(quality,pct_washed));
	j += sprintf(v4buffer+j,"</Result>\n");

	// super unit data
	if (!Super()) return v4buffer;
	j += sprintf(v4buffer+j,"<Result>\n");
	j += sprintf(v4buffer+j,"<Start_Date>%s</Start_Date>\n",_print_date(m_Begin));
	j += sprintf(v4buffer+j,"<Mine_Unit>%s</Mine_Unit>\n",(const char *)m_sSuper);
	j += sprintf(v4buffer+j,"<Section>%s</Section>\n",(const char *)m_sID);
	j += sprintf(v4buffer+j,"<Work_Days>%d</Work_Days>\n",m_wDays);
    j += sprintf(v4buffer+j,"<Days>%d</Days>\n",_32BitCount(m_lDays));
	j += sprintf(v4buffer+j,"<FT_PMS>%.2f</FT_PMS>",ftpms);
	j += sprintf(v4buffer+j,"<Raw_Tons_PMS>%.0f</Raw_Tons_PMS>",raw_tnspms);
	j += sprintf(v4buffer+j,"<Clean_Tons_PMS>%.0f</Clean_Tons_PMS>",clean_tnspms);
	j += sprintf(v4buffer+j,"<Prd_Tons_PMS>%.0f</Prd_Tons_PMS>",product_tnspms);
	j += sprintf(v4buffer+j,"<Raw_Tons_PFT>%.2f</Raw_Tons_PFT>",raw_tnspft);
	j += sprintf(v4buffer+j,"<Clean_Tons_PFT>%.2f</Clean_Tons_PFT>",clean_tnspft);
	j += sprintf(v4buffer+j,"<Prd_Tons_PFT>%.2f</Prd_Tons_PFT>",product_tnspft);
	j += sprintf(v4buffer+j,"<MS_PDay>%.2f</MS_PDay>",mspday);
    j += sprintf(v4buffer+j,"<Acres>%.2f</Acres>\n",area/sqft2acres);
    j += sprintf(v4buffer+j,"%s",_xml_qual(quality,pct_washed));
	j += sprintf(v4buffer+j,"</Result>\n");

	return v4buffer;
}

char* CResults::UnitSummaryItem(int super, double pct_washed)
{
	const char *unit = super? (const char *)m_sSuper: (const char *)m_sUnit;
    int j = sprintf(v4buffer,"!%s!,",unit);
    j += sprintf(v4buffer+j,"!%s!,",(const char *)m_sID);
	j += sprintf(v4buffer+j,"!%s!,",_print_date(m_Begin));
	j += sprintf(v4buffer+j,"!%s!,",_print_date(m_End));
    j += sprintf(v4buffer+j,"!%3i!,",m_wDays);

	double ftpms,ftpday,mspday;
	double raw_tnspms,clean_tnspms,product_tnspms=0;
	double raw_tnspft,clean_tnspft,product_tnspft=0;
	double raw_tnspday,clean_tnspday,product_tnspday=0;
	UnitData(pct_washed,ftpms,ftpday,raw_tnspms,clean_tnspms,product_tnspms,
		     raw_tnspft,clean_tnspft,product_tnspft,
			 raw_tnspday,clean_tnspday,product_tnspday,mspday);
	j += sprintf(v4buffer+j,"!%5.0f!,",raw_tnspday);
	j += sprintf(v4buffer+j,"!%5.0f!,",clean_tnspday);
	j += sprintf(v4buffer+j,"!%5.2f!,",mspday);
	j += sprintf(v4buffer+j,"!%6.2f!,",ftpms);
	j += sprintf(v4buffer+j,"!%5.0f!,",raw_tnspms);
	j += sprintf(v4buffer+j,"!%5.0f!,",clean_tnspms);

	j += sprintf(v4buffer+j,"!%5.2f!,",m_dQuality[NUMQUAL+2]);
	j += sprintf(v4buffer+j,"!%5.2f!,",m_dQuality[0]);

	return v4buffer;
}

//*****************************************************************************
//
// CResults - operators
//
//*****************************************************************************

int CResults::operator== (CResults& rhs)
{
	return ( (m_sID == rhs.m_sID) &&
			 (m_sUnit == rhs.m_sUnit) &&
			 (m_sSuper == rhs.m_sSuper) &&
			 (fabs(m_dEnd - rhs.m_dStart)<=eps));
}

CResults& CResults::operator+= (CResults& rhs)
{
	m_End = rhs.m_End;
	m_dEnd = rhs.m_dEnd;
	m_wMaxUsed = rhs.m_wMaxUsed;
	m_wDays += rhs.m_wDays;
	m_lDays |= rhs.m_lDays;
	m_dShifts += rhs.m_dShifts;
	m_dArea += rhs.m_dArea;
	m_dClean += rhs.m_dClean;
	m_dRaw += rhs.m_dRaw;
	_weighted_quality(m_dArea-rhs.m_dArea,m_dQuality,rhs.m_dArea,rhs.m_dQuality);
	return *this;
}

CResults& CResults::operator= (CResults& rhs)
{
	m_pPanel = rhs.m_pPanel;
	m_sID = rhs.m_sID;
	m_sType = rhs.m_sType;
	m_dStart = rhs.m_dStart;
	m_dEnd = rhs.m_dEnd;
	m_sUnit = rhs.m_sUnit;
	m_sSuper = rhs.m_sSuper;
	m_Begin = rhs.m_Begin;
	m_End = rhs.m_End;
	m_wDays = rhs.m_wDays;
	m_lDays = rhs.m_lDays;
	m_wLate = rhs.m_wLate;
	m_wLead = rhs.m_wLead;
	m_wMaxUsed = rhs.m_wMaxUsed;
	m_dShifts = rhs.m_dShifts;
	m_dArea = rhs.m_dArea;
	m_dClean = rhs.m_dClean;
	m_dRaw = rhs.m_dRaw;
	memcpy(m_dQuality,rhs.m_dQuality,sizeof(m_dQuality));
	return *this;
}

ofstream& operator<< (ofstream& os, CResults& rs)
{
	char *blank="XXXXX";
	char date[9];
	rs.m_Begin.format(date,"CCYYMMDD");
	os << date << endl;
	os << rs.m_sID << endl;
	os << rs.m_sType << endl;
	os << rs.m_sUnit << endl;
	if (rs.m_sSuper.length())
		os << rs.m_sSuper << endl;
	else
		os << blank << endl;
	os.write((char *)&rs.m_dShifts,sizeof(double));
	os.write((char *)&rs.m_dStart,sizeof(double));
	os.write((char *)&rs.m_dEnd,sizeof(double));
	os.write((char *)&rs.m_dArea,sizeof(double));
	os.write((char *)&rs.m_lDays,sizeof(ULong));
	os.write((char *)&rs.m_wLate,sizeof(UInt));
	os.write((char *)&rs.m_wLead,sizeof(UInt));
	os.write((char *)&rs.m_wMaxUsed,sizeof(UChar));
	for (int i=0; i<26; ++i)
		os.write((char *)&rs.m_dQuality[i],sizeof(double));
	return os;
}

ifstream& operator>> (ifstream& is, CResults& rs)
{
	String strTmp;
	is >> strTmp;
	rs.m_Begin.assign(strTmp);
	rs.m_End.assign(strTmp);
	is >> rs.m_sID;
	is >> rs.m_sType;
	is >> rs.m_sUnit;
	is >> rs.m_sSuper;
	is.read((char *)&rs.m_dShifts,sizeof(double));
	is.read((char *)&rs.m_dStart,sizeof(double));
	is.read((char *)&rs.m_dEnd,sizeof(double));
	is.read((char *)&rs.m_dArea,sizeof(double));
	is.read((char *)&rs.m_lDays,sizeof(ULong));
	is.read((char *)&rs.m_wLate,sizeof(UInt));
	is.read((char *)&rs.m_wLead,sizeof(UInt));
	is.read((char *)&rs.m_wMaxUsed,sizeof(UChar));
	for (int i=0; i<26; ++i)
		is.read((char *)&rs.m_dQuality[i],sizeof(double));
	return is;
}

//*****************************************************************************

// end of CResults.CPP


