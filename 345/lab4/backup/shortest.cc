/*  Larry L. Connor
 *  11/21/99
 *  shortest.cc
 *
 *  This is the main file for the shortest paths program for Lab#4.  This file
 *  basically takes care of the arguments passed in by the user and passes
 *  them to graph.h to be build into a graph (see graph.h for more info.)
 *
*/

//Required Libraries
#include <iostream.h>
#include <fstream.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

//User created libraries
#include "pque.h"
#include "graph.h"


int main(){
  //ifstream infile;
  Graph<int> trial;
  int source, destination, p;
  char tester[255];
  char *type = "rb";

  cout << "What file would you like to open?" << endl;
  cin >> tester;
  FILE *infile;
  infile = fopen(tester, type);
  cout << "What is the source node?" << endl;
  cin >> source;
  destination = 1;
  //  cout << "What is the destination node?" << endl;
  //cin >> destination;
  //infile.open(tester, ios::in);
  //Build the list if there are any problems (aka the actual file is
  //incorrect, or the source or destination are out of bounds) then exit.
  if(trial.Build(infile, source, destination) == -1)
    return -1;

  //Run the algorithm and find the shortest path to all of the nodes.
  trial.Paths(source, p, destination);
  return 0;
}
