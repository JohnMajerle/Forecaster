// date.h

#ifndef _DATE_H
#define _DATE_H

#include <string.h>
#include "c4.h"

/* DATE4 */
int date4assign( char *, const long ) ;
const char *date4cdow( const char * ) ;
const char *date4cmonth( const char * ) ;
int date4dow( const char * ) ;
void date4format( const char *, char *, char * ) ;
double date4formatMdx( const char * ) ;
int date4formatMdx2( const char * , double * ) ;
void date4init( char *, const char *, char * ) ;
long date4long( const char * ) ;
void date4timeNow( char * ) ;
void date4today( char * ) ;
#define date4day( datePtr )    ( (int)c4atol( (datePtr) + 6, 2 ) )
#define date4month( datePtr )  ( (int)c4atol( (datePtr) + 4, 2 ) )
#define date4year( yearPtr )   ( (int)c4atol( (yearPtr), 4 ) )

class Date4
{
public:
   Date4() ;
   Date4( long l ) ;
   Date4( const char *p) ;
   Date4( const char *p, char *pict ) ;
   operator long()      { return date4long(ptr()) ; }
   operator double()    { return date4formatMdx( ptr() ); }

   int operator  ==( const Date4& rhs ) const
       { return (date4long(ptr1()) == date4long(rhs.ptr1())) ; }
   int operator  !=( const Date4& rhs ) const
       { return (date4long(ptr1()) != date4long(rhs.ptr1())) ; }
   int operator  >=( const Date4& rhs ) const
       { return (date4long(ptr1()) >= date4long(rhs.ptr1())) ; }
   int operator  > ( const Date4& rhs ) const
       { return (date4long(ptr1()) > date4long(rhs.ptr1())) ; }

   long operator  +( const long l )    { return (date4long(ptr())+l) ; }
   long operator  -( const long l )    { return (date4long(ptr())-l) ; }
   void operator +=( const long l)     { date4assign( ptr(), date4long(ptr())+l ) ; }
   void operator -=( const long l )    { date4assign( ptr(), date4long(ptr())-l ) ; }
   long operator ++( )                 { *this+=1L ; return long(*this) ; }
   long operator ++( int i)            { *this+=1L ; return long(*this)-1L ; }
   long operator --( )                 { *this-=1L ; return long(*this) ; }
   long operator --(int i)             { *this-=1L ; return long(*this)+1L ; }
   void   assign( const long l)        { date4assign(ptr(),l) ; }
   void   assign( const char *p )      { memmove( dt, p, 8 ) ; dt[8] = 0 ; }
   void   assign( const char *p, char *pict )
                                       { date4init( ptr(), p, pict) ; }
   const char   *cdow()                { return date4cdow( ptr() ) ; }
   const char   *cmonth()              { return date4cmonth(ptr()) ; }
   int    day()                        { return date4day(ptr()) ; }    // Day of month  (1-31)
   int    dow()                        { return date4dow(ptr()) ; }    // Day of week   (1-7) 
   void   format( char *result, char *pict )
                                       { date4format( ptr(), result, pict ) ; }
   char  *format( char *pict ) const ;
   int    isLeap() const               { long y = date4year(dt) ;
                                         return (y%4 == 0 && y%100 != 0 || y%400 == 0) ?  1 : 0 ; }
   unsigned len()                      { return 8 ; }
   unsigned len1() const               { return 8 ; }
   int    month()                      { return date4month(ptr()) ; } // Month of year (1-12)
   void   today()                      { date4today(ptr()) ; }
   int    year()                       { return date4year(ptr()) ; } ;
   char *ptr()                         { return  dt ; }
   char const *ptr1() const            { return  dt ; }

private:
   char dt[9] ;
} ;

#endif
