// ****************************************************************************
//
// Module:  List.h
// Author:  Paul Dovenor
//
// Purpose: C++ template class header file for List
//
// Notes:  This is a base template class.  It implements a linked list of
//         entries of class T, and uses a private link template
//         class to provide doubly-linked list functionality for each element
//         of the list.
//
// Usage:  Users of this class should declare an instance of the class with
//         whatever data type will be used for each individual list entry.
//         For example, to declare a linked list of Windows (where the linked
//         list keeps the Window pointers), use:
//
//            List<Window> myList;
//
//            Window *p = new Window;
//            myList.Enqueue(p);
//
//         If the List is created with the destroy flag on (the default),
//         the list elements will be automatically destroyed when the list is
//         deleted.  Enqueue() always appends entries to the list, and Remove()
//         will delete the current entry from the list.
//
//         Use the First(), Last() and Get() routines to retrieve the first,
//         last or any entry from the list (Get() uses index numbers from 0
//         to Count - 1).  Prev() retrieves the previous entry, based on
//         where the internal list pointer is. Next()
//         increments the list pointer to the next element in the list.  Thus,
//         you can access list elements with loops such as this:
//
//         Window *p = First();
//         while (p) {
//            p = Next();
//         }
//
//         Or, you can use a for loop:
//
//         for ( Window *p = First(); p != 0; p = Next() ) {
//         }
//
// ****************************************************************************

#ifndef __List_H
#define __List_H

#include <stdio.h>
#include "linklist.h"

// ****************************************************************************

// class declaration
template <class T>
class List : public linkedList
{

public:

   // constructor and destructor
   List(int delete_entries=1) : linkedList(delete_entries) {}
   List(const List& l) : linkedList(l) {}
   virtual ~List() {}

   // methods to add or remove entries from the list (Purge() removes
   // all of the list entries)
   int  Enqueue(const T *entry) { return addTail((BaseType *)entry); }
   int  Remove() { return remove(); }
   void Purge() { freeContents(); }

   // methods to retrieve items from the list
   T *Retrieve() { return (T *)retrieve(); }
   T *First()    { return (T *)head(); }
   T *Prev()     { return (T *)previous(); }
   T *Next()     { return (T *)next(); }
   T *Last()     { return (T *)tail(); }
   T *Get(long index) { return (T *)get(index); }
   T *Find(void *lookfor) { return (T *)locate(lookfor); }

   // methods to access the linked list
   int Member(const T *entry) { return member((BaseType *)entry); }
   inline long Count() const { return m_Count; }
   inline long Index() { return m_Index; }

};

#endif

// ****************************************************************************

// end of list.h
