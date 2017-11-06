#include <iostream.h>
#include <math.h>
#include "lab2.h"

void print(bigint print);

void main()
{
  bigint a, c, temp;
  uint32 b;
  for(int loop = 0; loop < SIZE; loop++)
    {
      a[loop] = 0;
      c[loop] = 0;
    }
  a[0] = 1;
  for(b = 1; b < 34; b++)
    {
      bigmul(a,b,temp);
      cout << b << ": ";
      for(int i = 0; i < SIZE; i++)
	a[i] = temp[i];
      print(a);
      cout << endl;
    }
  bigmul(a,b,c);
  cout << "34: ";
  print(c);
  cout << endl;
}
