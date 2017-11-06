/* link.cc -- Implementation file for the LList and Node classes
 *
 * Larry Connor
 *
 * Note LList is also capable of building tree (see link.h for more info)
 */

//Node() constructor
template <class T>Node<T>::Node()
{ 
  //reinitialize values.
  freq = 0;
  elm = NULL;
  bin = '0';
  next = NULL;
  left = NULL;
  right = NULL;
  check = 0;
}

//Node<T> destructor
template <class T> Node<T>::~Node()
{
  //reinitialize values
  freq = 0;
  elm = NULL;
  bin = '0';
  next = NULL;
  left = NULL;
  right = NULL;
  check = 0;
}

//This returns the binary representation of an element at Node where
template <class T> string LList<T>::getbin(Node<T>* where)
{
  return where -> bin;
}

//This returns the element at Node where
template <class T> T LList<T>::getelm(Node<T>* where)
{
  return where -> elm;
}

//Llist constructor
template <class T> LList<T>::LList()
{
  //Initialization
  first = NULL;
  delm = 0;
}

template <class T> LList<T>::~LList()
{
  first = NULL;
  delm = 0;
}

//This sets up the first Node in the tree (called from Build)
template <class T> void LList<T>::setfirst()
{
  first = new Node<T>;
  first -> right = NULL;
  first -> left = NULL;
  first -> next = NULL;
}

//This builds a new LList of elements in a file
template <class T> char LList<T>::Build(int argc, char** argv) 
{
  ifstream infile;
  char out;
  char newelm;
  string t = "-t", v = "-v"; //Interface markers (see problem definition)
  if(argc == 2) //If 2 arguments are passed
    {
      infile.open(argv[1], ios::in); //The second is the filename
      if(!infile) //Check it.
	{
	  cout << "Cant find " << argv[1] << endl;
	  return 'x'; //Program bombs
	}
      out = 'n'; //This denotes that the used didn't type in "-t" or "-v"
    }
  else if(argc == 3)
    {
      infile.open(argv[2], ios::in);//The third arg. is the filename
      if(!infile) //check filename
	{
	  cout << "Can't find " << argv[2] << endl;
	  return 'x'; //Program bombs
	}
      if(argv[1] == t) //If the users wants "-t"
	{
	  cout << "-t" << endl;
	  out = 't';
	}
      else if(argv[1] == v) //If the user wants "-v"
	{
	  cout << "-v" << endl;
	  out = 'v';
	}
      else //Default case
	{
	  cout << "Improper arguments passed " << endl;
	  return 'x'; //Program bombs
	}
    }
  else //Other catch
    {
      cout << "Improper number of arguments passed " << endl;
      return 'x'; //Program bombs
    }
  while (infile.get(newelm))//Build list by adding to the back.
    {
      AddBack(newelm);
    }
  cout << endl;
  infile.close();//close file
  return out; //Lets us know what to do with the interface
}             

//This returns the next pointer to a Node
template <class T> Node<T>* LList<T>::getnext(Node<T>* tofind)
{
  return tofind -> next;
}

//This allows the user to set the number of distinct elm (should not be
//called from main)  ***internal use***
template <class T> void LList<T>::setdelm(int setter)
{
  if(setter >= 0)
  delm = setter;
}

//This returns the number of distinct elements
template <class T> int LList<T>::getdelm()
{
  return delm;
}

//This is for making a huffman tree from a List
template <class T> void LList<T>::makemin(LList<T> &A, LList<T>&B)
{
  for (int loop = 0; loop < A.getdelm() - 1; loop++) 
  {
    B.findmin(A, B);
   }
  delm = A.getdelm();
}

//This finding the min 2 freq from two lists and adds them to the huffman
//tree (via the huffman algorithm).
template <class T> void LList<T>::findmin(LList<T> &A, LList<T> &B)
{
  Node<T>* min;
  Node<T>* min2;
  char which;
  min = getmin(A,B,which); //The smallest freq in A and B 
  if(which == 'A') //If it was from A
    A.Remove (min); //Remove it from A
  else if(which == 'B') //If it was from B
    B.Remove (min); //Remove it from B
  
  min2 = getmin(A,B,which); //The second smallest freq in A and B
  if(which == 'A') //If it was in A
    A.Remove(min2); //Remove it from A
  else if(which == 'B')//Otherwise
    B.Remove(min2); //Remove it from B

  Node<T>* z = new Node<T>;

  //A new Node with freq = the sums of the two smallest freq
  z -> freq = min -> freq + min2 -> freq;
  if (min -> freq < min2 -> freq) //This orders the two freq
    {
      z -> left = min;
      z -> right = min2;
    }
  else
    {
      z->left = min2;
      z->right = min;
    }
  B.insert(z); //Insert new Node in list.
}

//This inserts a Node into a list (recursively)
template <class T> void LList<T>::insert(Node<T> *&inserter)
{
  if(first == NULL) //if the list is empty
    {
      first = inserter; //This node is the first node
      inserter -> next = NULL;
      return;
    }
  else //Otherwise go to the end of the list and insert the node
    {
      Node<T>* curr;
      for(curr = first; curr -> next != NULL; curr = curr -> next)
	{
	}
      curr -> next = inserter; //The new node is at the end.
      inserter -> next = NULL;
      return;
    }
}

//This returns a pointer to the smallest freq in lists A and B
//This is for internal use only (do not call from main)
template <class T> Node<T>* LList<T>::getmin(LList<T> A, LList<T> B, char &which)
{
  Node<T>* tempA;
  Node<T>* tempB;
  Node<T>* min;
  int small = 10000;
  tempA = A.gethead(); //Find the beginning of each list.
  tempB = B.gethead();

  for(; tempA != NULL; tempA = tempA -> next) //Finds the smallest in A
    {
      if(tempA -> freq < small)
	{
	  min = tempA;
	  small = tempA -> freq;
	  which = 'A';
	}
    }
  //Sees if B has any freqs smaller
  for(; tempB != NULL; tempB = tempB -> next)
    {
      if(tempB -> freq < small)
	{
	  min = tempB;
	  small = tempB -> freq;
	  which = 'B';
	}
    }
  return min; //Returns the location of the smallest freq.
}

//This removes a Node from a list
//Used internally
template <class T> void LList<T>::Remove(Node<T>* doomed)
{
  Node<T>* prev; //A pointer to the previous node
  if (first == doomed) //If the first node is to be deleted
    {
      first = doomed -> next; //set the first pointer to the next node
      return;
    }
  //Otherwise find the doomed Node
  for(prev = first; prev != NULL; prev = prev -> next)
    {
      //Once you find it
      if(prev -> next -> elm == doomed -> elm)
	{
	  prev -> next = doomed -> next;//Remove it
	  doomed -> next = NULL;
	  return;
	}
    }
}

//This returns the location of an element in a list.
template <class T> Node<T>* LList<T>::search(T finder)
{
  Node<T>* temp;
  //Walk through the list until you find it.
  for (temp = first; temp != NULL; temp = temp -> next)
    {
      //If you find it...return it.
      if (temp -> elm == finder)
	{
	  return temp;
	}
    }
  return NULL; //Otherwise return NULL
}

//This returns a pointer to the first node in a tree
template <class T> Node<T>* LList<T>::gethead()
{
  return first;
}

//This is for constructing a tree that only has one distinct element.
//This MUST be used if there is only one element.
//This sets up the first pointer (setsingle sets up the rest).
template <class T> void LList<T>::singledelm(LList<T> &A, LList<T> &B)
{
  Node<T>* temp;
  temp = A.gethead();
  (*this).first = temp;
  A.Remove (A.gethead());
}

//This also must be used when constructing a tree with a single element
//This sets up the information in the first (and only) node
template <class T> void LList<T>::setsingle(string binary, Node<T>* curr)
{
  first = curr;
  curr -> bin = '1';
  first -> right = NULL;
  first -> left = NULL;
  delm = 1;
  return;
}
 
//This is used to decode a list with delm == 1
//This MUST be used when delm == 1
template <class T> void LList<T>::singledecode(T newelm)
{
  //This sets up a new node
  Node<T>* curr = new Node<T>;
  string binary = "1";

  //Gives it binary value of 1 and makes it the first in the list
  first = curr;
  curr -> left = NULL;
  curr -> right = NULL;
  curr -> elm = newelm;
  curr -> bin = binary;
}
  

//This sets the binary value for a specific node.
//This is used internally (this should not be called from main).
template <class T> void LList<T>::setbin(string binary, Node<T>* curr)
{
  if (curr -> left == NULL && curr -> right == NULL)
    {
      curr -> bin = binary;
      return;
    }
  setbin(binary + '0', curr -> left);
  setbin(binary + '1', curr -> right);
}
      
//This prints out all of the elements in a tree (used mostly for debugging).
template <class T> void LList<T>::Print(Node<T>* start)
{
  if (start == NULL) //If we are at the end...go back
    {
      return;
    }
  Print (start -> left); //Otherwise go left...print the values
  cout << "Elm = " << start -> elm << " freq = " << start -> freq;
  cout << " Bin = " << start -> bin << endl;
  Print (start -> right); //Then go to the right.
}

//This is used for encoding the data in the tree so that it can be outputed
template <class T> void LList<T>::EncodeHuff(string filenamein, ofstream &outfile, int t, int v)
{
  ifstream infile(filenamein.data()); //Reopen the file
  char incoming, outgoing, temp;
  Node<T>* finder;
  int sender = 0, aleft = 8;
  outfile << getdelm() << endl;
  while (infile >> incoming) //Take the first char
    {
      finder = SearchHuff(first, incoming); //Find it binary representation.
      if (finder -> check == 0) //If it hasn't been printed in the file
	{
	  outfile << finder -> elm; //Print it
	  sender = bintoint(finder -> bin);
	  outfile << sender << endl; //And it's binary representation.
	  finder -> check = 1;
	  if(v == 1) //Also if "-v" then print the info to the screen.
	    {
	      cout << finder -> elm << " " << bintoint(finder -> bin);
	      cout << " " << finder -> bin << endl;
	    }
	}
    }
  infile.close(); //Close the file
  infile.open(filenamein.data(), ios::in);//Reopen the file from the start.
  while(infile >> incoming) //Take in a character
    {
      if (aleft == 0) //If we need to output bits (because our mask is full)
	{
	  if (t == 1)//If the "-t" option is selected 
	    //show the outgoing bits on the screen (for debug).
	    {
	      for (int loop = 8; loop > 0; loop--)
		{
		  cout << ((outgoing >> loop - 1) & 1U) << " ";
		  //Then write out
		  outfile << outgoing;
		}
	      cout << endl;
	      outfile << endl;
	    }
	  else //Then write to the file
	    {
	      outfile << outgoing;
	    }
	  aleft = 8; //reset the ammount of space
	  outgoing = outgoing & 0; //and clear the mask
	}
      if (aleft > 0) //If we still have space (fill it)
	{
	  finder = SearchHuff(first, incoming);
	  for(int loop = 0; loop < (finder -> bin).length(); loop++)
	    {
	      temp = (finder -> bin[loop]) - '0';
	      if(aleft == 0) //If we run out of space...write out
		{
		  if(t == 1)
		    {
		      for (int loop = 8; loop > 0; loop--)
			{
			  cout << ((outgoing >> loop - 1) & 1U) << " ";
			  outfile << outgoing;
			}               
		      outfile << endl;
		      cout << endl;
		    }
		  else
		    outfile << outgoing;
		  aleft = 8;
		  outgoing = outgoing & 0;
		}
	      outgoing = outgoing << 1; //Shift a bit
	      outgoing = outgoing | temp; //Input new bit
	      temp = 0;
	      aleft--; //Decrement the space we have
	    }
	}
    }
  if(outgoing > 0) //If we have anything left over at the end, fire it out.
    {
      if (t == 1) //Also output it to the screen.
	{
	  for (int loop = 8; loop > 0; loop--)
	    {
	      cout << ((outgoing >> loop - 1) & 1U) << " ";
	      outfile << outgoing;
	    }        
	  outfile << endl;
	  cout << endl;
	}       
      else
	{
	  outfile << outgoing;
	}
    }
  cout << endl;
}

//This function decodes a compressed file and outputs the data
template <class T> void LList<T>::decode(string filename, ifstream &infile)
{
  ofstream outfile(filename.data());
  char incoming;
  Node<T>* curr = first;
  int aleft = 8; //The space remaining in our mask
  while (infile >> incoming) //Take in a new char
    {
      aleft = 8;
      while(aleft > 0) //Until we run out of bits
	{
	  //Search for the data
	  if(curr -> left != NULL && curr -> right != NULL)
	    {
	      //If we get a 1 go right.
	      if((incoming >> (aleft - 1) & 1U) == 1)
		{
		  curr = curr -> right;
		}
	      //If we get a 0 go left.
	      else if ((incoming >> (aleft - 1) & 1U) == 0)
		{
		  curr = curr -> left;
		}	      
	      aleft--; //and decrement.
	    }
	  //Once we find the data (output it)
	  else if(curr -> right == NULL && curr -> left == NULL)
	    {
	      outfile << curr -> elm;
	      curr = first;
	    }
	  else //This is a catch case for errors.
	    {
	      aleft--;
	      return;
	    }
	}
    }
  //Note this section is the same as the above section.
  if(aleft > 0) //If there are any left overs at the end.
    {
      curr = first;
      while(aleft > 0) //Find their representative data and output it.
	{
          if(curr -> left != NULL && curr -> right != NULL)
            {
              if((incoming >> (aleft - 1) & 1U) == 1)
                {
                  curr = curr -> right;
                }
              else if ((incoming >> (aleft - 1) & 1U) == 0)
                {
                  curr = curr -> left;
                }         
              aleft--;
            }
          else if(curr -> right == NULL && curr -> left == NULL)
            {
              cout << "Firing out " << curr -> elm << endl;
              outfile << curr -> elm;
              curr = first;
	      aleft--;
            }
          else
            {
              cout << "curr->elm = " << curr -> elm << endl;
              aleft--;
              return;
            }                       
	}
    }
  cout << endl;
}

//This converts and int to a binary string.
template <class T> string LList<T>::inttobin(int rep)
{
  string binary = "";
  string switcher = "";
  if (rep == 0)
    return "0";
  while(rep > 0) //While we have numbers.
    {
      switcher += rep%2 + '0'; //Add them to the string.
      rep = rep/2;
    }
  //Then reverse the order for ease in reading.
  for(int loop = 0; loop < switcher.length(); loop++)
    {
      binary += switcher[(switcher.length() - 1) - loop];
    }
  return binary;
}

//This converts a binary string to a base 10 number (the inverse to inttobin)
template <class T> int LList<T>::bintoint(string binary)
{
  int len = binary.length();
  int rep = 0;
  //Walk through the string and add the decimal representation to the total.
  for(int loop = 0; loop < len; loop++)
    {
      rep += (((binary[(len - 1) - loop] - '0')) * ((pow(2, loop))));
    }
  return rep; //and return the total.
}

//This rebuilds a tree from the information in a compressed file.
template <class T> void LList<T>::rebuild(char incoming, int incoming2)
{
  Node<T>* point = new Node<T>; //create a new node with an element.
  string newbin;
  point -> elm = incoming;
  newbin = inttobin(incoming2);
  //add it to the tree.
  rebuildtree(newbin, point -> elm, first, newbin, newbin.length());
}

//This adds a node to a tree (in it's binary position).
template <class T> void LList<T>::rebuildtree(string newbin, char newelm, Node<T>*& start, string temp, int len)
{
  // NOTICE** len = newbin.length()
  Node<T>* maker = new Node<T>;//create a new node
  if(len == 0)//If we are in position place the Node
    {
      start -> elm = newelm;
      start -> bin = newbin;
      return;
    }
  else if(len != 0) //Otherwise keeping look for the proper place
    {
      if(temp[temp.length() - len] == '1') //go right.
	{
	  if (start -> right == NULL) //If you need a new node (create one).
	    {
	      start -> right = maker;
	    }
	  len --; //Decrement the binary number (so we can find the position).
	  rebuildtree(newbin, newelm, start -> right, temp, len);
	}
      else if (temp[temp.length() - len] == '0') //go left.
	{
	  if (start -> left == NULL)
	    {
	      start -> left = maker;
	    }
	  len--; //Decrement the binary number
	  rebuildtree(newbin, newelm, start -> left, temp, len);
	}
    }
}

//This searches a tree for a specific character and returns its location.
template <class T> Node<T>* LList<T>::SearchHuff(Node<T>* start, T finder)
{
  Node<T> *l;
  Node<T> *r;

  if (start -> elm == finder) //If we found it return its location
    return start;
  //If we didn't find it return NULL;
  if (start -> left == NULL && start -> right == NULL)
    {
      return NULL;
    }
  //Otherwise if it is on the left...go left
  l = SearchHuff(start -> left, finder);
  if(l != NULL)
    return l;
  //Otherwise go right.
  r = SearchHuff(start -> right, finder);
  return r;
}    

//This displays all of the info in a linkedlist (for debugging before the
//tree is built.
template <class T> void LList<T>::Display(Node<T>* start)
{
  //If we are not at the end
  if (NULL != start) 
    {
      // show this elm
      cout << start->elm << " " << start -> freq << start -> bin << endl;
      // and move along
      Display(start->next);
    } 
}    

//This adds a new element to the back of a linkedlist (called from Build).
template <class T> void LList<T>::AddBack(T newelm) {
  Node<T>* tempNode;
  Node<T>* curr, prev;
  // create a new Node (to be hooked into the list shortly)
  tempNode = new Node<T>;
  // copy over the data
  tempNode->elm = newelm;

  // is the list empty?
  if (first == NULL)
    {
      first = tempNode;
      first -> freq = first -> freq + 1;
      delm++;
      return;
    }
  else 
    {
      //If the item is in the list
      if (first -> elm == newelm)
	{
	  //Increment it's freq.
	  first -> freq = first -> freq + 1;
	  delete tempNode;
	  return;
	}
    // otherwise, put the new node in at the end
    for(curr = first; curr -> next != NULL; curr = curr -> next)
      {
	if(curr -> next -> elm == newelm)
	  {
	    curr -> next -> freq = curr -> next -> freq + 1;
	    delete tempNode;
	    return;
	  }
	    
      }
    tempNode -> next = NULL;
    tempNode -> freq = tempNode -> freq + 1;
    curr -> next = tempNode;
    delm++; //Increment the number of distinct elements.
    }
}         

