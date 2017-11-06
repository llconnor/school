/****************************************************************************
 *
 *  Filename: lab2.h
 *  Original Author: Herman Schmit
 *  
 *  History:
 *  Created: [herman: 9/17/01]
 *
 *  Copyright (c) 2001 Carnegie Mellon. 
 *
 *****************************************************************************/

#ifndef LAB2
#define LAB2

#define LARGE 0xffffffff
#define SIZE 10

typedef unsigned int uint32;
typedef uint32 bigint[SIZE];

uint32 bigmul(bigint A, uint32 B, bigint C);
/* Multiplies A and B.  Places results in C.  If there is an overflow
   (because C doesn't fit into a bigint), then the value of the NEXT
   most-significant word is returned.  If there is no overflow, return
   zero. */

#endif /* LAB2 */

