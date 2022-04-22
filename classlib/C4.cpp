// c4.cpp  Conversion Routines

#include <memory.h>
#include <stdlib.h>
#include <string.h>

// c4atod    Converts a string to a double 

double c4atod( const char *str, const int lenStr )
{
   char buffer[50] ;
   int  len ;

   len = ( lenStr >= 50 ) ? 49 : lenStr ;
   memcpy( buffer, str, (size_t)len ) ;
   buffer[len] = '\0' ;
   return atof( buffer ) ;
}

void c4atod2( char *str, int lenStr, double *result )
{
   char buffer[50] ;
   int  len ;

   len = ( lenStr >= 50 ) ? 49 : lenStr ;
   memcpy( buffer, str, (size_t)len ) ;
   buffer[len] = '\0' ;
   *result = atof( buffer ) ;
   return;
}

int c4atoi( const char *str, const int l )
{
   char buf[128] ;
   int lenStr ;

   if ( l >= (int)sizeof( buf ) )
      lenStr = (int)sizeof( buf ) - 1 ;
   else
      lenStr = l ;
   memcpy( buf, str, (size_t)lenStr ) ;
   buf[lenStr] = '\0' ;
   return atoi( buf ) ;
}

long c4atol( const char *str, const int l )
{
   char buf[128] ;
   int lenStr ;

   if ( l >= (int)sizeof( buf ) )
      lenStr = (int)sizeof( buf ) - 1 ;
   else
      lenStr = l ;

   memcpy( buf, str, (size_t)lenStr) ;
   buf[lenStr] = '\0' ;
   return atol( buf ) ;
}

char *c4descend( char *to, const char *from, int len )
{
   for(; len-- > 0; )
      to[len] = -from[len] ;
   return to ;
}


// c4encode
//
//   - From CCYYMMDD to CCYY.MM.DD
//
//   Ex.        c4encode( to, from, "CCYY.MM.DD", "CCYYMMDD" ) ;
//

void c4encode( char *to, const char *f, char *tTo, const char *tF )
{
   int pos ;
   char chr ;
   char *chrPos ;
   char const * tFrom, * from ;

   tFrom = tF ;
   from = f ;

   strcpy( to, tTo ) ;

   while ( (chr = *tFrom++) != 0)
   {
      if ( ( chrPos= strchr( tTo, chr ) ) == 0 )
      {
         from++;
         continue ;
      }

      pos = (int)( chrPos - tTo ) ;
      to[pos++] = *from++ ;

      while (chr == *tFrom)
      {
         if (chr == tTo[pos] )
            to[pos++] = *from ;
         tFrom++ ;
         from++ ;
      }
   }
}

//  c4ltoa45
//
//    Converts a long to a string.  Fill with '0's rather than blanks if
//    'num' is less than zero.
//

void c4ltoa45( long lVal, char *ptr, int num)
{
   int   n, numPos ;
   long  iLong ;

   iLong = (lVal>0) ? lVal : -lVal ;
   numPos = n = (num > 0) ? num : -num ;

   while (n-- > 0)
   {
      ptr[n] = (char) ('0'+ iLong%10) ;
      iLong = iLong/10 ;
   }

   if ( iLong > 0 )
   {
     memset( ptr, (int) '*', (size_t) numPos ) ;
     return ;
   }

   num--;
   for (n=0; n<num; n++)
      if (ptr[n]=='0')
         ptr[n]= ' ';
      else
         break ;

   if (lVal < 0)
   {
      if ( ptr[0] != ' ' )
      {
         memset( ptr, (int) '*', (size_t) numPos ) ;
         return ;
      }
      for (n=num; n>=0; n--)
         if (ptr[n]==' ')
         {
            ptr[n]= '-' ;
            break ;
         }
   }
}

void c4trimN( char *str, int nCh )
{
   int len ;

   if ( nCh <= 0 )
      return ;

   /* Count the Length */
   len = 0 ;
   while ( len< nCh )
   {
      len++ ;
      if ( *str++ == '\0' )
         break ;
   }

   if ( len < nCh )
      nCh = len ;

   *(--str) = '\0' ;

   while( --nCh > 0 )
   {
      str-- ;
      if ( *str == '\0' ||  *str == ' ' )
         *str = '\0' ;
      else
         break ;
   }
}