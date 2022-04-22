// linktest.cpp

#include <iostream.h>
#include "linklist.h"

// declare a Cell class for spreadsheet cells
class Cell {

public:

   // constructor and destructor
   Cell(int r, char c) { row = r; col = c; }
   ~Cell() {}

   // get and set methods for setting the value of the cell
   inline void Value(double v) { myValue = v; }
   inline double Value() const { return myValue; }

   // print method for the cell
   inline void Print() { cout << myValue << endl; }

private:

   int row;             // row and column of cell
   char col;

   double myValue;      // floating point value

};


// main program
void main()
{
   // declare a spreadsheet as a linked list of Cells
   LinkedList<Cell> spreadsheet;

   // add cells
   for (int i = 1; i <= 10; i++) {
      // allocate a new cell and initialize its position
      Cell *p = new Cell(i, 'A');

      // set value to the row number
      p->Value(i);

      // add the cell to the spreadsheet
      spreadsheet.Add(p);
   }

   // delete the 4th entry
   Cell *p = spreadsheet.Get(4);
   spreadsheet.Remove(p);

   // print the remaining values using the Cell::Print() method

   for (int index = 0L; index < spreadsheet.Count(); index++) {
      Cell *p = (Cell *)spreadsheet.Get(index);

      if (p)
         p->Print();
   }
}
