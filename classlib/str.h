// FileName: str.h
 

#ifndef STRING_H
#define STRING_H

#include <istream>    // Collaborator for overloaded extraction operator

namespace std {
typedef basic_istream<char, char_traits<char> > istream;
}

int strmatch(const char*, const char*);
void prefix_justify(char *s);
void right_justify(char *s);
void right_justify_zero_fill(char *s);
void left_justify(char *s);
void center_justify(char *s);
void trim(char *s);

class String
{
public:

//Modifiers

	//makes the String object into a copy of rhs
	String& operator= (const String & rhs);

	//augments the String object by the rhs
	String& operator+= (const String& rhs);

	void upper(void);     	//changes all the string characters to uppercase
	void lower(void);       //changes all the string characters to lowercase



//Accessors

	// Returns the number of characters in the String
	int length(void) const;

	// The next two members access a character in the String.
	// The first can be sent only to a non-constant String object
	// and provides storage as well as retrieval.
	// The second can be sent to an String, constant or not,
	// and provides retrieval only.
	// Note: The argument must non-negative and
	// less than the length of characters in the string.
	char& operator[] ( int position);
	const char& operator[] ( int position) const;
	// precondition:  position >= 0  and
	//				  postion < number of characters in the String
	// postcondition: if position equals 0 then this method
	//				  returns the first character in the String
	//				  if position equals 1 then this method
	//				  returns the second character in the String
	//				  and so on.


//Implementors

	// Return the concatenation of its operands
	String operator+  (const String& rhs) const;

	// The following comparison operators are case sensitive
	// Return 0 for false and 1 for true
	int operator<  (const String & rhs) const;
	int operator>  (const String & rhs) const;
	int operator<= (const String & rhs) const;
	int operator>= (const String & rhs) const;
	int operator== (const String & rhs) const;
    int operator== (const char* rhs) const;
	int operator!= (const String & rhs) const;

	// Overloaded extraction operator
	friend std::istream& operator>> (std::istream& i, String& s);
	
//Backbone Members

	String(void);
	String(const String& s);
	String(const char* cstr);
	String(const char c);
	~String(void);

	// Converts a String object to a constant c-str.
	operator const  char* (void) const;

	// Match strings
	int match(const String & rhs) const { return strmatch(pc,rhs); }

private:
	char *pc;	// points to first character of the String array.
	int size;	// number of elements (used and unused) in the array.
	int count;	// number of characters the String has.
};

#endif

