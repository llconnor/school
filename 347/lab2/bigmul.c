#include <math.h>
#include <iostream.h>
#include <stdio.h>
#include "lab2.h"

void print(bigint printer)
{
  for(int loop = SIZE - 1; loop >= 0; loop--)
    {
      printf("%.8x", printer[loop]);
    }
  cout << endl;
}

uint32 bigmul(bigint a, uint32 b, bigint c)
{
  int loop = 0;
  uint32 returner = 0, checker = 0;
  uint32 overflow = 0;
  uint32 highbit = 0;
  
  //Make sure that c is initialized
  for(int i = 0; i < SIZE; i++)
    c[i] = 0;
  for(loop = 0; loop < SIZE; loop++)
    {
      //Add in the overflow from the previous round
      c[loop] += overflow;
      c[loop] += b * (a[loop] & 0x0000ffff);
      //Multiply high order a[loop] * b 
      highbit = ((b * ((a[loop] & 0xffff0000) >> 16)) << 16);
      //Save the high order bits from Aloop_H * b (for the next round)
      overflow = ((b * ((a[loop] & 0xffff0000) >> 16)) >> 16);
      //Add the low order bits from Aloop_H * b to c[loop]
      c[loop] += highbit; //& 0x0000ffff;
    }
  return overflow;
}

