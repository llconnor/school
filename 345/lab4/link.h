/* link.h -- Implementation file for LList<T> class
 *
 * Larry Connor
 * This class is primarily used for Huffman encoding.
 * It contains the following important functions
 * char Build(int argc, char** argv); //For building a tree from a file.
 * Node<T>* gethead(); //This returns the "first" pointer to the tree
 * 
 * //This is used to output the compressed data (t&v are for interface -t&-v)
 * void EncodeHuff(string filenamein, ofstream &outfile, int t, int v);
 * 
 * //This is used to decode the compressed data
 * void decode(string filename, ifstream &infile);
 * void singledecode(T newelm); //This is to decode a file with delm = 1
 * void makemin(LList<T> &A, LList<T> &B); //Used to build the tree from a linked list

 * void singledelm(LList<T> &A, LList<T> &B); //Call if there is only one elm
 * //This rebuilds the tree from the compressed data.
 * void rebuild(char incoming, int incoming2);
 * string getbin(Node<T>* where); //This returns bin at a certain node.
 * string inttobin(int rep);//This converts a base 10 number to a binary string
 * int getdelm(); //This returns the number of distinct elements.
 * int bintoint(string binary);//This converts a string to a base 10 number
 * T getelm(Node<T>* where); //This returns the element at Node where.
 * void Display(Node<T>* start); //This displays all of the items in a linkedlist
 *
*/

#ifndef _LINK_H_
#define _LINK_H_


//Required Libraries
#include <iostream.h>
#include <string>
#include <fstream.h>
#include <assert.h>
#include <math.h>

// defined within
template <class T> class LList;
template <class T> class Node;


//The definition for class Node
template <class T> class Node
{
  //To allow LList access to the Nodes
  friend class LList<T>;
 private:
  T elm; //The character that is in the files.
  string bin; //The binary representation of the character in the tree.
  int freq; //The number of times the character appears in the file.
  Node<T>* next; //This is for constructing a list from the file (which is
                 //broken down and make into the huffman tree.
  Node<T>* left; //These are the pointer for the Huffman tree.
  Node<T>* right; //*
  int check; //This is a bin value that tells whether a Node has been visited.
 
 public:
 Node(); 
 ~Node();
};

//The linklist class (note this is actually a linkedlist of trees and is used
//in building the Huffman tree.
template <class T> class LList
{
 private:
  Node<T>* first; //The first pointer in the tree (or list)
  int delm; //The number of distinct elements in the tree.

 public:                  
  LList();
  ~LList();
  
  //All of the functions contained here are defined above (and in link.cc)
  char Build(int argc, char** argv);
  void Remove(Node<T>* doomed);
  void AddBack(T newelm);
  Node<T>* gethead();
  Node<T>* search(T finder);
  Node<T>* SearchHuff(Node<T>* start, T finder);
  Node<T>* getmin(LList<T> A, LList<T> B, char &which);
  Node<T>* getnext(Node<T>* tofind);
  void EncodeHuff(string filenamein, ofstream &outfile, int t, int v);
  void decode(string filename, ifstream &infile);
  void singledecode(T newelm);
  void Print(Node<T>* Print);
  void Printout(ofstream &outfile, Node<T>* start);
  void makemin(LList<T> &A, LList<T> &B);
  void findmin(LList<T> &A, LList<T> &B);
  void insert(Node<T> *&inserter);
  void setbin(string binary, Node<T>* curr);
  void singledelm(LList<T> &A, LList<T> &B);
  void setsingle(string binary, Node<T>* curr);
  void setdelm(int setter);
  void rebuild(char incoming, int incoming2);
  void setfirst();
  void rebuildtree(string newbin, char newelm, Node<T>*& start, string temp, int len);
  string getbin(Node<T>* where);
  string inttobin(int rep);
  int getdelm();
  int bintoint(string binary);
  T getelm(Node<T>* where);
  T Pop(int &freq);
  T Popper();
  void Display(Node<T>* start);
};

#include "link.cc"
#endif
