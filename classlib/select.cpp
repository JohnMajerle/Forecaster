#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "str.h"
#include "select.h"

// ****************************************************************************

// Select - constructor

Select::Select(char *list)
{
      int i=0;
      char *listp=list,*s;

      while (i<MAXLIST) {
         if ((s=strtok(listp," ,\t")) == NULL)
            break;
         if (strlen(s) <= MAXIDCHARS) {
            sprintf(&idList[i][0],"%-*s",MAXIDCHARS,s);
            strcpy(&idList[i][0],s);
            prefix_justify(&idList[i++][0]);
         }
         listp = NULL;
      }
      idList[i][0]='\0';
}

// Look for the given id in the token list. Return TRUE if found otherwise FALSE.

int Select::selected(const char *id)
{
      const char *s1,*s2;

      for (int i=0; idList[i][0] != '\0'; ++i) {
         s1 = id; s2 = &idList[i][0];
         if (strmatch(s1,s2) == 0)
            return 1;
      }
      return 0;
}

// ****************************************************************************

// end of Select.cpp
