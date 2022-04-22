// date.cpp

#include <memory.h>
#include <string.h>
#include <time.h>
#include "date.h"

#define  JULIAN_ADJUSTMENT    1721425L
#define  S4NULL_DATE          1.0E100   // may not compile on some Op.Sys. 
                                        // Set to MAXDOUBLE ?              
static int monthTot[] =
    { 0, 0,  31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 } ;
         // Jan Feb Mar  Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec
         //  31  28  31   30   31   30   31   31   30   31   30   31

static char v4buffer[256];

typedef struct
{
   char cdow[12] ;
}  DOW ;

typedef struct
{
   char cmonth[10] ;
} MONTH ;

static DOW dayOfWeek[] =
{
   { "\0          " },
   { "Sunday\0    " },
   { "Monday\0    " },
   { "Tuesday\0   " },
   { "Wednesday\0 " },
   { "Thursday\0  " },
   { "Friday\0    " },
   { "Saturday\0  " },
} ;

static MONTH monthOfYear[] =
{
   { "\0        " },
   { "January\0 " },
   { "February\0" },
   { "March\0   " },
   { "April\0   " },
   { "May\0     " },
   { "June\0    " },
   { "July\0    " },
   { "August\0  " },
   { "September" },
   { "October\0 " },
   { "November\0" },
   { "December\0" },
} ;

Date4::Date4()
{
   memset( dt, ' ', 8 ) ;
   dt[8] = 0 ;
}

Date4::Date4( long l )
{
   date4assign(dt, l) ;
   dt[8] = 0 ;
}

Date4::Date4( const char *p )
{
   memmove( dt, p, 8 ) ;
   dt[8] = 0 ;
}

Date4::Date4( const char *p,char *pict )
{
   date4init( dt, p, pict ) ;
   dt[8] = 0 ;
}

char *Date4::format(char *pict) const
{
	date4format(dt,v4buffer,pict);
    return v4buffer;
}

static int c4Julian( const int year, const int month, const int day )
{
   //  Returns 
   //     >0   The day of the year starting from 1 
   //          Ex.    Jan 1, returns  1 
   //     -1   Illegal Date 
   int isLeap, monthDays ;

   isLeap =  ( year%4 == 0 && year%100 != 0 || year%400 == 0 ) ?  1 : 0 ;

   monthDays = monthTot[ month+1 ] -  monthTot[ month] ;
   if ( month == 2 )  monthDays += isLeap ;

   if ( year  < 0  ||
        month < 1  ||  month > 12  ||
        day   < 1  ||  day   > monthDays )
        return( -1 ) ;        // Illegal Date 

   if ( month <= 2 )  isLeap = 0 ;

   return(  monthTot[month] + day + isLeap ) ;
}

static int c4monDy( const int year, const int days,  int *monthPtr,  int *dayPtr )
{
   //  Given the year and the day of the year, returns the month and day of month. 
   int isLeap, i ;

   isLeap = ( year % 4 == 0 && year % 100 != 0 || year % 400 == 0 ) ?  1 : 0 ;
   if ( days <= 59 )
      isLeap = 0 ;

   for( i = 2; i <= 13; i++)
   {
      if ( days <= monthTot[i] + isLeap )
      {
         *monthPtr = --i ;
         if ( i <= 2)
            isLeap = 0 ;

         *dayPtr = days - monthTot[ i] - isLeap ;
         return 0 ;
      }
   }
   *dayPtr = 0 ;
   *monthPtr = 0 ;

   return -1 ;
}

static long c4ytoj( const int y )
{
   int yr ;
   //  Calculates the number of days to the year 
   //  This calculation takes into account the fact that 
   //     1)  Years divisible by 400 are always leap years. 
   //     2)  Years divisible by 100 but not 400 are not leap years. 
   //     3)  Otherwise, years divisible by four are leap years. 
   //  Since we do not want to consider the current year, we will 
   //  subtract the year by 1 before doing the calculation. 
   yr = y - 1 ;
   return( yr*365L +  yr/4L - yr/100L + yr/400L ) ;
}

int date4assign( char *datePtr, const long ldate )
{
   // Converts from a Julian day to the dbf file date format. 
   long totDays ;
   int  iTemp, year, nDays, maxDays, month, day ;

   if ( ldate <= 0 )
   {
      memset( datePtr, ' ',  8 ) ;
      return 0L ;
   }

   totDays = ldate - JULIAN_ADJUSTMENT ;
   iTemp = (int)( (double)totDays / 365.2425 ) ;
   year = iTemp + 1 ;
   nDays = (int)( totDays - c4ytoj( year ) ) ;
   if ( nDays <= 0 )
   {
      year-- ;
      nDays = (int)( totDays - c4ytoj( year ) ) ;
   }

   if ( ( year % 4 == 0 ) && ( year % 100 ) || ( year % 400 == 0 ) )
      maxDays = 366 ;
   else
      maxDays = 365 ;

   if ( nDays > maxDays )
   {
      year++ ;
      nDays -= maxDays ;
   }

   c4monDy( year, nDays, &month, &day ) ;

   c4ltoa45( (long)year, datePtr, -4 ) ;
   c4ltoa45( (long)month, datePtr + 4, -2 ) ;
   c4ltoa45( (long)day, datePtr + 6, -2 ) ;

   return 0 ;
}

const char *date4cdow( const char *datePtr )
{
   return dayOfWeek[date4dow(datePtr)].cdow ;
}

const char *date4cmonth( const char *datePtr )
{
   return monthOfYear[date4month( datePtr )].cmonth ;
}


int date4dow( const char *datePtr )
{
   long date ;
   date = date4long(datePtr) ;
   if ( date < 0 )
      return 0 ;
   return (int)( ( date + 1 ) % 7 ) + 1 ;
}

void date4format( const char *datePtr, char *result, char *picture )
{
   int rest, mNum ;
   unsigned int resultLen, length ;
   char *ptrEnd, *monthPtr, tChar ;

   resultLen = strlen( picture ) ;
   memset( result, ' ', resultLen ) ;

   _strupr( picture ) ;
   c4encode( result, datePtr, picture, "CCYYMMDD" ) ;

   ptrEnd = strchr( picture, 'M' ) ;
   if ( ptrEnd )
   {
      monthPtr = result+ (int)( ptrEnd - picture ) ;
      length = 0 ;
      while ( *(ptrEnd++) == 'M' )
         length++ ;

      if ( length > 2)
      {
         // Convert from a numeric form to character format for month 
         if (!memcmp( datePtr+4, "  ", 2 ))   // if blank month 
         {
            memset( monthPtr, ' ', length ) ;
            return ;
         }

         mNum = c4atoi( datePtr+4, 2) ;

         if ( mNum < 1)
            mNum = 1 ;
         if ( mNum > 12)
            mNum = 12 ;

         rest = length - 9 ;
         if (length > 9)
            length = 9 ;

         memcpy( monthPtr, monthOfYear[mNum].cmonth, length ) ;
         if (rest > 0)
            memset( monthPtr+length, (int) ' ', (size_t)rest ) ;

         tChar = monthOfYear[mNum].cmonth[length] ;
         if( tChar == '\0' || tChar == ' ' )
         {
            mNum = strlen(monthOfYear[mNum].cmonth) ;
            if ( (unsigned)mNum != length )
               monthPtr[mNum] = ' ' ;
         }
      }
   }
}

double date4formatMdx( const char *datePtr )
{
   long ldate ;
   ldate = date4long(datePtr) ;
   if ( ldate == 0 )
      return (double) S4NULL_DATE ;  // Blank or Null date 
   return (double)ldate ;
}

int date4formatMdx2( const char *datePtr, double *doubPtr )
{
   long ldate ;

   ldate = date4long(datePtr) ;
   if ( ldate == 0 )
      *doubPtr = (double)S4NULL_DATE ;  // Blank or Null date 
   else
      *doubPtr = (double)ldate ;
   return 0 ;
}

void date4init( char *datePtr, const char *dateData, char *picture )
{
   char *monthStart, monthData[10], buf[2] ;
   int yearCount, monthCount, dayCount, centuryCount, i, length ;

   dayCount = 5 ;
   monthCount = 3 ;
   yearCount = 1 ;
   centuryCount= -1 ;

   memset( datePtr, ' ', 8 ) ;

   _strupr( picture ) ;
   for ( i=0; picture[i] != '\0'; i++ )
   {
      switch( picture[i] )
      {
         case 'D':
            if ( ++dayCount >= 8 )
               break ;
            datePtr[dayCount] = dateData[i] ;
            break ;
         case 'M':
            if ( ++monthCount >=6 )
               break ;
            datePtr[monthCount] = dateData[i] ;
            break ;
         case 'Y':
            if ( ++yearCount >= 4 )
               break ;
            datePtr[yearCount] = dateData[i] ;
            break ;
         case 'C':
            if ( ++centuryCount >= 2 )
               break ;
            datePtr[centuryCount] = dateData[i] ;
            break ;
         default:
            break ;
      }
   }

   if ( strcmp( datePtr, "        " ) == 0 )
      return ;

   if ( centuryCount == -1 )
      memcpy( datePtr, "19", (size_t)2 ) ;
   if ( yearCount ==  1 )
      memcpy( datePtr + 2, "01", (size_t)2 ) ;
   if ( monthCount == 3 )
      memcpy( datePtr + 4, "01", (size_t)2 ) ;
   if ( dayCount == 5 )
      memcpy( datePtr + 6, "01", (size_t)2 ) ;

   if ( monthCount >= 6 )
   {
      // Convert the Month from Character Form to Date Format 
      monthStart = strchr( picture, 'M' ) ;

      length = monthCount - 3 ;        // Number of 'M' characters in picture 

      memcpy( datePtr+4, "  ", (size_t)2 ) ;

      if ( length > 3 )
         length = 3 ;
      memcpy( monthData, dateData + (int)( monthStart - picture ), (size_t)length) ;
      while ( length > 0 )
         if ( monthData[length-1] == ' ' )
            length-- ;
         else
            break ;

      monthData[length] = '\0' ;

      _strlwr( monthData ) ;
      buf[0] = monthData[0] ;
      buf[1] = 0 ;
	  _strupr(buf) ;
      monthData[0] = buf[0] ;

      if ( length > 0 )
         for( i = 1 ; i <= 12; i++ )
         {
            if ( memcmp( monthOfYear[i].cmonth, monthData, (size_t)length ) == 0 )
            {
               c4ltoa45( (long) i, datePtr+4, 2 ) ;  // Found Month Match 
               break ;
            }
         }
   }

   for ( i = 0 ; i < 8 ; i++ )
      if ( datePtr[i] == ' ' )
         datePtr[i] = '0' ;
}

long date4long( const char *datePtr )
{
   //  Returns: 
   //    >0  -  Julian day 
   //           That is the number of days since the date  Jan 1, 4713 BC 
   //           Ex.  Jan 1, 1981 is  2444606 
   //     0  -  NULL Date (dbfDate is all blank) 
   //    -1  -  Illegal Date 
   int  year, month, day, dayYear ;

   year = c4atoi( datePtr, 4 ) ;
   if ( year == 0)
      if ( memcmp( datePtr, "        ", 8 ) == 0)
         return  0 ;

   month = c4atoi( datePtr + 4, 2 ) ;
   day = c4atoi( datePtr + 6, 2 ) ;
   dayYear = c4Julian( year, month, day ) ;
   if ( dayYear < 1 )    // Illegal Date 
      return -1L ;

   return ( c4ytoj( year ) + dayYear + JULIAN_ADJUSTMENT ) ;
}

void date4timeNow( char *timeData )
{
   long timeVal ;
   struct tm *tmPtr ;

   time( (time_t *)&timeVal) ;
   tmPtr = localtime( (time_t *)&timeVal) ;

   c4ltoa45( (long)tmPtr->tm_hour, timeData, -2) ;
   timeData[2] = ':' ;
   c4ltoa45( (long)tmPtr->tm_min, timeData + 3, -2) ;
   timeData[5] = ':' ;
   c4ltoa45( (long)tmPtr->tm_sec, timeData + 6, -2) ;
}

void date4today( char *datePtr )
{
   long timeVal ;
   struct tm *tmPtr ;

   time( (time_t *)&timeVal ) ;
   tmPtr = localtime( (time_t *)&timeVal ) ;

   c4ltoa45( 1900L + tmPtr->tm_year, datePtr, -4 ) ;
   c4ltoa45( (long)tmPtr->tm_mon + 1, datePtr + 4, -2 ) ;
   c4ltoa45( (long)tmPtr->tm_mday, datePtr + 6, -2 ) ;
}
