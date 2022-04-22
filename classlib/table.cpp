#include <stdlib.h>
#include <string.h>

#include "table.h"

// ****************************************************************************

// Table - constructor

Table::Table(char *list)
{
  int i;
  char *listp,*s;
  long tmp;

  if(*list == '\0') {
     Tlist[0].s_min=0;
     Tlist[0].s_max=Highest=BIGGEST;
     Tlist[1].s_min=-1;
  } else {
     listp=list;
     for(i=0;i<NRANGES;++i) {
        if((s=strtok(listp,", \t")) == NULL)
           break;
        if((tmp=save_range(i,s)) > Highest)
           Highest=tmp;
        listp=NULL;
     }
     Tlist[i].s_min=-1;
  }
}

//   return non-zero value if the number argument is a member of the
//   selection list

int Table::selected(int n)
{
  int i;

  for(i=0;Tlist[i].s_min!=-1;++i)
     if(n>=Tlist[i].s_min && n<=Tlist[i].s_max)
        return(1);
  return(0);
}

//   convert a string number spec to a numeric range in the selection table
//   and return the highest number in the range

long Table::save_range(int n, char *s)
{
      char *cp,num[NDIGITS+1];

      // get the first (and possibly only) number 
	  //
      cp=num;
      while(*s!='\0' && *s!='-')
         *cp++=*s++;
      *cp='\0';
      Tlist[n].s_min=atol(num);
      if(*s == '\0')
         return(Tlist[n].s_max=Tlist[n].s_min);

      // get the second number 
	  //
      if(*++s == '\0')
         return(Tlist[n].s_max=BIGGEST);
      else {
         cp=num;
         while(*s!='\0' && *s!='-')
            *cp++=*s++;
         *cp='\0';
         Tlist[n].s_max=atol(num);
      }
      return(Tlist[n].s_max);
}

// ****************************************************************************

// end of Table.cpp
