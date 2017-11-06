/*  Larry L. Connor
 *  3/15/99
 *  pque.h -- The header file for the priority queue class
 *
 *  The implementation file is pque.cc
 *
 *  Included classes:
 *  pque<T> - This is a simple priority queue with a couple of extras.  It is
 *            not a traditional tree, but actually a vector, with the parent
 *            of node 'n' at (n-1)/2.
 *
 *  Included functions:
 *
 *  This adds a new value to the priority queue
 *  void Addnew(T newvalue, int count, int from);
 *
 *  This returns the first element of From
 *  int FromFirst()
 *
 *  This sets the size of From and Table
 *  void FromTablesize(int newsize);
 *
 *  This returns (and deletes) the minimum value from the pque.
 *  T getmin();
 *
 *  This returns the number of slots in the priority queue that have been used
 *  int getused();
 *
 *  This returns a 1 if the list is empty.
 *  bool IsEmpty();
 *
 *  This is a demote operation (explained in more detail in pque.cc)
 *  void Organdown(int place);
 *
 *  This is a promote operation (explained in more detail in pque.h)  
 *  void Organize(int place);
 *    
 *  This prints out the entire pque (all of the values) (for debugging)
 *  void Print();
 *  
 *  This tells whether the Table is empty or not.
 *  bool TableEmp();  
 *
 *  This returns the first element of Table
 *  int TableFirst();
 *
 *  This updates the pque (including all reorganization)
 *  void Update(int place, T newvalue);  
*/
#ifndef _PQUE_H_
#define _PQUE_H_


//Required Libraries
#include <iostream.h>
#include <string>
#include <fstream.h>
#include <assert.h>
#include <stdlib.h>
#include <vector.h>


// defined within
template <class T> class Pque
{
 private:
  vector<T> Tree;  //Where the actual distances are stored
  int used;  //The number of elements Tree is using.

  //Each Table[n] tells what node corresponds to Tree[n]
  vector<int> Table;

  //From[n] tells where the shortest distance came from (for -p option). 
  vector<int> From;
 public:
  Pque();
  ~Pque();

  //These functions explained above.
  void Addnew(T newvalue, int count, int from);
  int FromFirst();
  void FromTablesize(int newsize);
  T getmin();
  int getused();
  bool IsEmpty();
  void Organdown(int place);
  void Organize(int place);
  void Print();
  bool TableEmp();
  int TableFirst();
  void Update(int place, T newvalue);
};

#include "pque.cc"
#endif
