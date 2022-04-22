#include <math.h>

#include "linklist.h"

// ****************************************************************************

// linkedList - constructor

linkedList::linkedList(int delete_entries)
{
	// init instance variables
	if (delete_entries)
	  doDeleteEntries = 1;
	else
	  doDeleteEntries = 0;

	m_Head = 0;
	m_Tail = 0;
	m_Current = 0;
	m_Index = 0;
	m_Count = 0L;
}

// ----------------------------------------------------------------------------

// linkedList - copy constructor

linkedList::linkedList(const linkedList& ll) :
	m_Head(0),
	m_Tail(0),
	m_Current(0),
	m_Index(0),
	m_Count(0),
	doDeleteEntries(ll.doDeleteEntries)
{
	link *pLink = ll.m_Head;
	long n = ll.m_Count;
	for (long i=0; i<n; ++i) {
		BaseType *pEntry = pLink->m_Entry;
		this->addTail(pEntry);
		pLink = pLink->m_Next;
	}
	this->head();
}

// ----------------------------------------------------------------------------

// linkedList - assignment operator

linkedList& linkedList::operator=(const linkedList& ll)
{
	freeContents();     // removes all items from the list
	doDeleteEntries = ll.doDeleteEntries;

	link *pLink = ll.m_Head;
	long n = ll.m_Count;
	for (long i=0; i<n; ++i) {
		BaseType *pEntry = pLink->m_Entry;
		this->addTail(pEntry);
		pLink = pLink->m_Next;
	}
	this->head();

	return *this;
}

// ----------------------------------------------------------------------------

// ~linkedList - destructor

linkedList::~linkedList()
{
	freeContents();     // removes all items from the list
}

// ----------------------------------------------------------------------------

// addTail - adds an item to the end of the list


int linkedList::addTail(BaseType *entry)
{
	// test for illegal argument
	if (!entry)
      return IllegalArgument;

	// allocate memory for a new list item
	link *p = new link(entry);
	if (!p)
      return MemoryAllocation;

	// add the element to the list (if no head of list is defined, make this
	// new entry the head of the list)
	if (!m_Head) {

      m_Head = m_Tail = p;
	  p->m_Next = p->m_Prev = 0;
	  
	} else {

      p->m_Prev = m_Tail;
      p->m_Next = 0;
      m_Tail->m_Next = p;   // p is now the new last element of the list
	  m_Tail = p;

	}

	m_Current = p;		// reset the current pointer
	m_Index = m_Count;	// reset the current index to end of list
	m_Count += 1L;		// increment the count	

	return good;
}

// ----------------------------------------------------------------------------

// remove - removes current element from the list

int linkedList::remove(void)
{
	// test for no entries
	if (m_Count == 0L)
      return bad;

	// remove the element
	link *p = m_Current;
	m_Count -= 1L;       // decrement the count
	if (m_Count == 0) {	// only one link in list
	  m_Current = m_Head =  m_Tail = 0;
	  m_Index = 0;
	} else {
	  if (m_Tail == p) m_Tail = p->m_Prev;
	  if (m_Head == p) m_Head = p->m_Next;
      if (p->m_Prev) p->m_Prev->m_Next = p->m_Next;
	  if (p->m_Next) p->m_Next->m_Prev = p->m_Prev;
	  if (p->m_Next) { m_Current = m_Current->m_Next; ++m_Index; }
	  else           { m_Current = m_Current->m_Prev; --m_Index; } 
	}

	// and delete the element
	if (doDeleteEntries) delete p->m_Entry;
	delete p;

	return good;
}

// ----------------------------------------------------------------------------

// freeContents - removes all items from the list

void linkedList::freeContents()
{
	// loop through the list, deleting all of the list elements - if the
	// destroy flag was set when the list was created, also delete the
	// information contained in each element
	link *p;

	while (m_Head) {
      p = m_Head->m_Next;

      if (doDeleteEntries)
        delete m_Head->m_Entry; // delete the element's information

      delete m_Head;             // and delete the list element

      // set up the next element to be the head of the list
      if (p) p->m_Prev = 0;
      m_Head = p;
	}

	m_Head = 0;          // reset the head of the list
	m_Tail = 0;          // reset the tail of the list
	m_Count = 0L;        // reset the count
	m_Current = 0;       // reset the current pointer
	m_Index = 0;		 // reset the current index
}

// ----------------------------------------------------------------------------

// head - moves to and returns the first entry in the list

BaseType *linkedList::head()
{
	m_Current = m_Head;
	m_Index = 0;
	return m_Head? m_Head->m_Entry: 0;
}

// ----------------------------------------------------------------------------

// previous - moves to and returns the previous entry in the list

BaseType *linkedList::previous()
{
	if (m_Current != m_Head) {
	  m_Current = m_Current->m_Prev;
	  --m_Index;
	  return m_Current->m_Entry;
	} else
      return 0;
}

// ----------------------------------------------------------------------------

// next - moves to and returns the next entry in the list

BaseType *linkedList::next()
{
	if (m_Current != m_Tail) {
	  m_Current = m_Current->m_Next;
	  ++m_Index;
	  return m_Current->m_Entry;
	} else
      return 0;
}

// ----------------------------------------------------------------------------

// tail - moves to and returns the last entry in the list

BaseType *linkedList::tail()
{
	m_Current = m_Tail;
	m_Index = m_Count - 1;
	return m_Tail? m_Tail->m_Entry: 0;
}

// ----------------------------------------------------------------------------

// get - returns an entry from a specified position in the list

BaseType *linkedList::get(long index)

{
	// test for no entries or illegal argument
	if ((m_Count == 0L) || (index >= m_Count))
      return 0;

	// look through the list for the index(th) element
	long n = index;
	link *cur = m_Current;
    if (cur == 0  ||  labs(m_Index-n) >= n) {
	//
	// If there's no previously remembered index or the distance from
	// the index to the desired position is greater than the distance
	// from the head of the list to the desired position, then count 
	// over from the head of the list.
	// 
		for (cur=m_Head; cur && --n>=0; cur=cur->m_Next)
			;
	} else if ((n -= m_Index) < 0) {
	//
	// The desired element is to the left of the most recently
	// accessed element, go left.
	//
		while (cur && ++n<=0)
			cur = cur->m_Prev;
	} else {                      // go right
		while (cur && --n>=0)
			cur = cur->m_Next;
	}

	if (cur) {
		m_Current = cur;	// make the element current
		m_Index = index;
	}
	return cur? cur->m_Entry: 0;
}

// ----------------------------------------------------------------------------

// locate - finds a list element containing a particular entry

BaseType *linkedList::locate(void *lookfor)
{
	// test for no entries or illegal argument
	if ((m_Count == 0L) || !lookfor)
      return 0;

	// look through the list for the element containing the same entry pointer
	// as the one passed to this method
	long index = 0;
	link *p = m_Head;
	while (p) {
      if (p->m_Entry->match(lookfor)) {
		  m_Current = p;
		  m_Index = index;
		  return p->m_Entry;   // found it!
	  }
      p = p->m_Next;
	  ++index;
	}
	return 0;   // not found
}

// ----------------------------------------------------------------------------

// member - checks if an entry is already in the list

int linkedList::member(const BaseType *entry)
{
	link *p = m_Head;
	while (p && (p->m_Entry != entry))
      p = p->m_Next;
	return (p==0)? 0 : 1;
}

// ----------------------------------------------------------------------------

// print - prints each element in the list

void linkedList::print(FILE *output)
{
	link *p = m_Head;
	while (p) {
      p->m_Entry->print(output);
      p = p->m_Next;
	}
}

// ****************************************************************************

// end of linklist.cpp
