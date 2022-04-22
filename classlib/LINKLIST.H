// ****************************************************************************
//
// Module:  linklist.h
// Author:  Paul Dovenor
//
// Purpose: C++ template class header file for linkedList
//
// Notes:
//
// ****************************************************************************

#ifndef __linkedList_H
#define __linkedList_H

#include "bt.h"

// ****************************************************************************

class link {

friend class linkedList;     // linkedList class has access to pointers

private:

	// constructor and destructor
	link(BaseType *entry) : m_Entry(entry), m_Prev(0), m_Next(0) {}
	virtual ~link() {}

	BaseType *m_Entry;        // pointer to information being stored
	link     *m_Prev;		 // in the list, and the previous and
	link     *m_Next;		 // next list elements

	// private copy constructor and operator= (define these and make them
	// public to enable copy and assignment of the class)
	link(const link&);
	link& operator=(const link&);

};


// ****************************************************************************

// object state data types
enum state {
	good = 0, bad = 1,
    IllegalArgument = 2,
    DuplicateEntry = 3,
    NotFound = 4,
    MemoryAllocation = 5,
    InternalListError = 6
};

// class declaration
class linkedList {

public:

	// constructor and destructor (note that the default is for the list
	// to automatically delete the list entries when the list is destroyed)
	linkedList(int delete_entries = 1);
    linkedList(const linkedList&);
	virtual ~linkedList();
    linkedList& operator=(const linkedList&);

	// methods to add or remove entries from the list
	int  addTail(BaseType *entry);
	int  remove();
	void freeContents();

	// methods to retrieve items from the list
	BaseType *retrieve() { return m_Current? m_Current->m_Entry: 0; }
	BaseType *head();
	BaseType *previous();
	BaseType *next();
	BaseType *tail();
	BaseType *get(long index);
	BaseType *locate(void *lookfor);

	// methods to access the list
	int member(const BaseType *entry);

	// method to print the list
	void print(FILE *output);

protected:

	link *m_Head;           // head of linked list
	link *m_Tail;           // tail of linked list
	link *m_Current;	    // current element

	long m_Index;			// current index-th element of the list
	long m_Count;			// number of elements in the list
	int doDeleteEntries;    // set to true if entries should be deleted
                            // when the linked list instance is deleted

};

#endif

// ****************************************************************************

// end of linklist.h
