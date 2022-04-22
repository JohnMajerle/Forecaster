// FileName: str.cpp
// created 7/29/94
// last modified 12/95
// Code for the  String class as defined Garnett's manuscript.

#include "str.h"
#include <string.h>		// Standard library for c-string utility functions
#include <stdlib.h>		// For the "exit" function
#include <ctype.h>		// Standard library for cConverting between upper and lower case

const int DEFAULT_SIZE = 16;

String::String(void)
{
	pc = new char[DEFAULT_SIZE];
	if (!pc) 				// Memory manager out of space.
		exit(1);   			// Abort program.
	count = 0;
	pc[count] = '\0';	// Use this to mark the end of string
	size = DEFAULT_SIZE;
}

String::String(const String& s)
{

	size = s.size;
	pc = new char[size];
	if (!pc)
		exit(1);// Terminate the program
	strcpy(pc,s.pc);  // in the standard c-string library
	count = s.count;
}

String::String(const char c)
{
	pc = new char[DEFAULT_SIZE];
	if (!pc) 				// Memory manager out of space.
		exit(1);   			// Abort program.
	count = 1;
	pc[0] = c;
	pc[count] = '\0';	// Use this to mark the end of string
	size = DEFAULT_SIZE;
}

String::String(const char* cstr)
{
	// Compute how much space the new array needs
	// If n is the length of cstr then n+1 spaces are needed.
	int n = strlen(cstr); // Another c-string library function
	n = n+1;

	//Compute the number of multiples of the DEFAULT_SIZE
	// are needed.
	int numberOfBlocks = (n/DEFAULT_SIZE) + 1;
	// You have to add one because integer division truncates
	size = numberOfBlocks*DEFAULT_SIZE;
	pc = new char[size];
	if (!pc)
		exit (1);// Terminate the program
	strcpy(pc,cstr);  // in the standard c-string library
	count = n-1;      // The actual number of characters in
					  // String is one less than the number of spaces used.
}

String::~String(void)
{
	// The destructor must return to the memory manager
	// the dynamically allocated array used to store
	// the characters in the array.
	delete[] pc;	// indicate that pc  is an array.
}

int String::length(void) const
{
	 return count;
}

//  Some remarks about the comparison methods.
//  1. These methods are case sensitive.
//  2. These comparison methods use a library function, strcmp, in the
//  c-string library which can vary from platform to platform. Here is the
//  documentation for Borland's version.
//	Return Value:
//	The comparison methods return an int value as follows:
//		þ == 0  if s1 == s2
//		þ >  0  if s1 >  s2

int String::operator== (const String& rhs) const
{
	int n= strcmp(pc,rhs.pc);
	if (n == 0)
		return 1;	// they are  equal
	else
		return 0;	// They are not equal
}

int String::operator== (const char* rhs) const
{
	int n= strcmp(pc,rhs);
	if (n == 0)
		return 1;	// they are  equal
	else
		return 0;	// They are not equal
}

int String::operator<  (const String& rhs) const
{
	 int n = strcmp(pc,rhs.pc);
	 if ( n < 0)
		return 1;
	 else
		return 0;
}

int String::operator>  (const String& rhs) const
{
	int n= strcmp(pc,rhs.pc);
	if (n >0 )
		return 1;	// lhs > rhs
	else
		return 0;	// lhs is not > rhs
}

int String::operator<= (const String& rhs) const
{
	 int n = strcmp(pc,rhs.pc);
	 if ( n <= 0)
		return 1;
	 else
		return 0;
}

int String::operator>= (const String& rhs) const
{
	return !(*this < rhs);
}

int String::operator!= (const String & rhs) const
{
	return !( rhs == *this);
}

// The next two methods are for storage and retrieval of
// individual characters in the string. The second method
// is applies to constant strings and only permits retrieval.

char& String::operator[]  (int position)
{
	return pc[position];
}

const char& String::operator[]  (int position) const
{
	return pc[position];
}

String::operator const char* () const    // Casts a String as a c-str
{
	return pc;
}

String & String::operator= (const String & s)
{

	// First make sure that the rhs and lhs are
	// not the same object.
	if (*this == s )

		return *this;    //lhs and rhs are already the same object.

	else {

		delete[] pc;	// free up array space in lhs

		// Make the lhs a copy of the rhs.
		// This method is similar to the copy constructor method.
		size = s.size;
		pc = new char[size];
		if (!pc)
			exit(1);		// Terminate the program
		strcpy(pc,s.pc);	// in the standard c-string library
		count = s.count;
		return *this;		// Return the lhs
	}
}

String  String::operator+ (const String& rhs) const
{
	//Calculate how large the array should be. You need an
	// extra space for the '\0'.
	// This is a lot like the copy constructor method.
	int n = rhs.count + count +1;
	int numberOfBlocks = (n/DEFAULT_SIZE) + 1;
	int tempsize = numberOfBlocks*DEFAULT_SIZE;

	// Allocate that space as in the copy constructor
	// However, don't put its address in pc because pc already
	// points to some allocated memory.
	char* ptemp = new char[tempsize];
	if (!ptemp)
		exit(1);// Terminate the program

	else {	// ok. Memory found
		// copy the lhs into *temp
		strcpy(ptemp,pc);
		// concatenate the rhs onto what is currently in *temp
		strcat(ptemp,rhs.pc);	// another c-string library function
	}

	// Although temp is not a String object
	// you can return it and the conversion constructor
	// automatically converts it to the required String return type.
	String temp(ptemp);
	delete[] ptemp;
	return temp;
}

String& String::operator+= (const String& rhs)
{
	String temp;
	temp = *this + rhs;
	*this = temp;
	return *this;
}

void String::upper(void)         //Make the characters all uppercase
{
  for (int i = 0; i < count; i++)
	pc[i] = toupper(pc[i]);
  return;

}

void String::lower(void) 			//Make the characters all lowercase
{
	for (int i = 0; i < count; i++)
		pc[i] = tolower(pc[i]);
	return;
}

// The next method overloads the insertion
// method in order to input a String object.
std::istream& operator>> (std::istream& i, String &s) {
	const int MAX = 81; // Can read upto MAX-1 characters
	char buffer[MAX];   // Positions in buffer run from 0 to MAX-1
	i.getline(buffer,MAX,'\n');
	s = buffer;         // Use the conversion constructor from c-str to String method
	return i;
}


int strmatch(const char *s, const char *t)
{
      --s; --t;
      do {
         ++s; ++t;
         if (*t == '*'  ||  (*s == '\0'  &&  *t == '\0'))
            return 0;
      } while (*s == *t  ||  *t == '?');
      return (*s-*t);
}


void prefix_justify(char *s)
{
      char c;

      if (isdigit(*s))
         right_justify(s);
      else if (*s == '\'') {
         *s = ' ';
         left_justify(s);
      } else if (*s == '\"') {
         *s = ' ';
         right_justify(s);
      } else if (*s == '^') {
         *s = ' ';
         center_justify(s);
      } else if (*s == '\\') {
         c = *(s+1);
         while (*s) *s++ = c;
      }
}


void right_justify(char *s)
{
      size_t len;

      len = strlen(s);
      while (*s == ' '  ||  *s == '0'  &&  len) {
         len--;
         *s++ = ' ';
      }
      if (len)
         while (*(s+(len-1)) == ' ') {
            memmove(s+1,s,len-1);
            *s = ' ';
         }
}


void right_justify_zero_fill(char *s)
{
      char *c=s;
      size_t len;

      while (*c == ' ') c++;
      if (!*c)
         return;
      len = strlen(s);
      while (*(s+(len-1)) == ' ') {
         memmove(s+1,s,len-1);
         *s = '0';
      }
}


void left_justify(char *s)
{
      size_t len;

      len = strlen(s);
      while (*s == ' ') {
         memmove(s,s+1,len-1);
         *(s+(len-1)) = ' ';
      }
}


void center_justify(char *s)
{
      size_t len;
      int count;
      char *t;

      len = strlen(s);
      right_justify(s);
      t = s;
      while (*t == ' ')
        ++t;
      count = (t-s) / 2;
      while (count) {
         memmove(s,s+1,len-1);
         *(s+(len-1)) = ' ';
         --count;
       }
}


void trim(char *s)
{
      size_t len;

      len = strlen(s);
      while (*s == ' ') {
         memmove(s,s+1,len);
		 len = strlen(s);
      }
}
