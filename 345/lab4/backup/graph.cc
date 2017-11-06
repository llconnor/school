//constructor for the Anode class
template <class T> Anode<T>::Anode()
{
  down = NULL;
  hub = -1;
  cost = 0;
}

//Destructor for the Anode class
template <class T> Anode<T>::~Anode()
{
  down = NULL;
  hub = -1;
  cost = 0;
}

//Constructor for the Alist class.
template <class T> Alist<T>::Alist()
{
  head = NULL;
  parent = -1;
  distfroms = -1; //The final "correct" distance is set to -1 (infinity).
  mark = 0;
}

template <class T> Alist<T>::~Alist()
{
  head = NULL;
  parent = -1;
  distfroms = -1;
  mark = 0;
}

//This function adds a new neighbor to a specific place in the array (to
//create an adjacency list).
template <class T> int Alist<T>::Addneighbor(int neigh, double dist, int node)
{
  // create a new Node (to be hooked into the list shortly)
  Anode<T>* tempNode;
  Anode<T>* curr, prev; //These "walk" through the list to find the location.
  tempNode = new Anode<T>;
  tempNode->hub = neigh;
  tempNode->cost = dist;

  // is the list empty?
  if (head == NULL)
    {
      head = tempNode;
      return 0;
    }
  else
    {
      //This loop "walks" the pointers to the end of the list.
    for(curr = head; curr -> down != NULL; curr = curr -> down)
      {
	//If the node is in the list...exit
        if(curr -> down -> hub == neigh)
          {         
	    cout << "edge already exists:  (" << node << "," << neigh;
	    cout << ")" << endl;
	    return -1; //This tells Build that something went wrong.
          }
      }
    tempNode -> down = NULL; //Add the node to the end.
    curr -> down = tempNode;
    }
  return 0; //This tells Build that everything is okay.
}    

//This checks a slot in the adjacency list to see if it is empty.
template <class T> bool Alist<T>::IsEmpty()
{
  if(head == NULL)
    return 1;
  return 0;
}

//This checks to see if a slot in the list has been marked (visited).
template <class T> bool Alist<T>::IsMarked()
{
  return mark;
}

//This marks a slot in the list (telling the program that it has been visited)
template <class T> void Alist<T>::Mark()
{
  mark = 1;
}

//This prints out a single slot in the adjacency list (for debugging).
template <class T> void Alist<T>::Print()
{
  Anode<T> *curr;
  //Walk down the list and print the information at each node.
  for (curr = head; curr != NULL; curr = curr -> down)
    {
      cout << " " << curr -> hub << "(" << curr -> cost << ")";
    }
  cout << endl;
}

//This checks to see if all of the slots in the list have been marked.
template <class T> bool Graph<T>::AllMarked()
{
  for (unsigned int loop = 1; loop < Lists.capacity(); loop++)
    {
      if(Lists[loop].IsMarked() == 0)
	return 0;
    }
  return 1;
}

//This builds a graph from a file (and checks to make sure everything is
//legitimate) if this function returns -1 then main exits (a 0 continues the
//program).
template <class T> int Graph<T>::Build(FILE *infile, int source, int destination)
{
  string checker;
  int size, neigh, node;
  double dist;
  int num = 1;
  int loopnum;
  unsigned char getter[1];

  fread(getter, 1, 1, infile);
  size = getter[0];
  //This expands the graph to the size that it will need for the file.
  vector<Alist<int> > v(size + 1);
  (*this).Lists = v;
  //This loop builds the graph (using Addneighbor)
  while(num < size)
    {
      loopnum = num + 1;
      node = num;
      while(loopnum <= size)
	{
	  neigh = loopnum;
	  fread(getter, 1, 1, infile);
	  dist = getter[0];
	  if(dist < 0) //If the cost of a node is negitive then the prog. exits.
	    {
	      cout << "negativ edge cost:  (" << node << "," << neigh << ")";
	      cout << " --> " << dist << endl;
	      return -1;
	    }  
	  
	  //If there was a problem with Adding a neighbor (exit).
	  if(dist > 0)
	    {
	      if(Lists[node].Addneighbor(neigh, dist, node) == -1) //changed**
		return -1;
	      else
		{
		  //		  cout << node << " " << neigh << " " << dist << endl;
		  Lists[neigh].Addneighbor(node, dist, neigh);
		  //cout << neigh << " " << node << " " << dist << endl;
		}
	    }
	  loopnum++;
	}
      num++;
    }
  //  if(fread(getter, 1, 1, infile) > 0)
  //  cout << "Not empty " << endl;
  return 1;
}

//This is the function that finds the shortest distance to each node.
//It also finds the path to take to go to each node.
template <class T> void Graph<T>::Paths(int vertex, int p, int destination)
{
  Pque<double> dist;

  //This sets the proper size of Table and From (the vectors that keep
  //track of where the nodes are in the Priority Queue).
  dist.FromTablesize(Lists.capacity() - 1);
  Anode<T>* curr;
  int newnode;
  double distnew;
  int prevnode;

  Lists[vertex].distfroms = 0; //The distance from the source to itself = 0
  Lists[vertex].Mark(); //Mark the vertex

  //Add the neighbors of the vertex to the Pque.
  for(curr = Lists[vertex].head; curr != NULL; curr = curr -> down)
    {
      dist.Addnew(curr -> cost, curr -> hub, vertex);
    }
  
  prevnode = vertex; //The last node we were at was the vertex.
  while(dist.TableEmp() == 0) //While the Pque is not empty.
    {
      if(AllMarked() == 1) //If all of the nodes are marked then leave.
	{
	  //Print the info and leave.
	  //  PrintFinal(vertex, destination, p);
	  //	  dist.Print();
	  Print(vertex);
	  return;
	}

      //Otherwise get the next nearest node and repeat the process.
      //dist.Print();
      //      char fg;
      // cin >> fg;
      newnode = dist.TableFirst();
      prevnode = dist.FromFirst(); //Find what the parent of the new node is.
      distnew = dist.getmin(); //Find what the cost(x,y) is.

      //If the distance between the two nodes is infinity, update the distance
      if(Lists[newnode].distfroms == -1)
	{
	  Lists[newnode].distfroms = distnew;
	  Lists[newnode].parent = prevnode; //Update the parent of the node.
	}

      //Otherwise if the cost(x,y) + distance to the parent < current distance
      //from the vertex, update the node.
      else if(Lists[newnode].distfroms > distnew && distnew != -1)
	{
	  Lists[newnode].distfroms = distnew;
	  Lists[newnode].parent = prevnode;
	}
      
      //If this node is not marked.
      if(Lists[newnode].IsMarked() == 0)
	{
	  //Mark it and if it isn't empty add its neighbors to the Pque.
	  Lists[newnode].Mark(); 
	  if(Lists[newnode].IsEmpty() == 0)
	    {
	      //	      cout << newnode << " Isn't Empty" << endl;
	      for(curr = Lists[newnode].head; curr != NULL; curr = curr -> down)
		{
		  //cout << "Adding " << curr -> hub << endl;
		  dist.Addnew(curr -> cost + Lists[newnode].distfroms, curr -> hub, newnode);
		}
	    }
	}
    }
  //Finally print all of the results.
  //  PrintFinal(vertex, destination, p);
  Print(vertex);
}  

//This function prints all of the (hopefully) shortest distances. 
//Used primarily for debugging.
template <class T> void Graph<T>::Print(int vertex)
{
  cout << "Source Node: " << vertex << endl;
  //Walk through the loop and print all of the distances.
  for(unsigned int loop = 1; loop < Lists.capacity(); loop++)
    {
      if(loop != vertex)
	{
	  if(Lists[loop].parent != -1)
	    {
	      cout << loop << ", " << Lists[loop].distfroms << ", ";
	      printpath(loop, vertex);
	      cout << endl;
	    }
	  else
	    cout << loop << "Infin " << endl;
	}
    }
  cout << endl;
}

/*template <class T> void Graph<T>::Print()
{
  cout << "Distance from vertex " << endl;

  //Walk through the loop and print all of the distances.
  for(unsigned int loop = 1; loop < Lists.capacity(); loop++)
    {
      cout << "Node # " << loop << " is this " << Lists[loop].distfroms; 
      cout << ", " << Lists[loop].parent << " dis ";
    }
  cout << endl;
  }*/

//This prints out the shortest distance between the source and the destination
//and if the user selects the -p option it also prints out the shortest path.
template <class T> void Graph<T>::PrintFinal(int source,int destination, int p)
{

  //If the distance is 0 or infinity
  if(Lists[destination].distfroms == -1 || Lists[destination].distfroms == 0)
    {
      //If the source is the same as the destination --> print 0.
      if(source == destination)
	{	
	  cout << "0" << endl;
	  cout << source << endl;
	  return;
	}
      
      //Otherwise --> print infinity.
      cout << "Infinity" << endl;
      return;
    }

  //If the distance is a normal integer --> print it.
  cout << Lists[destination].distfroms << endl;

  if(p == 0)
    return;

  //If the -p option is selected --> print the shortest path between the two.
  else if(p == 1)
    {
      int moonwalk = destination; //This is a temp variable for the curr parent

      //This holds all of the parents so they can be printed backwards.
      vector<int> paths(0);
      paths.push_back(destination);

      //Add all of the parents from the destination to the source (in reverse
      //order).
      while(moonwalk != -1)
	{
	  moonwalk = Lists[moonwalk].parent; 
	  paths.push_back(moonwalk);
	}

      //Then print them in the correct order.
      for(unsigned int loopnum = paths.size() - 1; loopnum > 0; loopnum--)
	{
	  if(paths[loopnum] != 0 && paths[loopnum] != -1)
	    {
	      cout << paths[loopnum] << " ";
	    }
	}
       cout << destination << endl;
    }
}

template <class T> void Graph<T>::printpath(int curr, int vertex)
{
  if(curr == vertex)
    return;
  else
    {
      printpath(Lists[curr].parent, vertex);
      cout << curr << " ";
    }
}
