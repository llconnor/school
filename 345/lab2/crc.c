#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <values.h>
#include <unistd.h>
#include <stdlib.h>
#include "sock.h"

int
check_crc(unsigned char *p)
{
  unsigned crc = 0;
  unsigned char y;
  int i, j, xor;
  
  for (i = 0; i < 271; i++) {
    y = p[i];
    for(j = 0; j < 8; j++) {
      /* If MSb of CRC is 1, then we need to XOR after the shift */
      xor = crc & 0x80000000;
      
      /* shift CRC, this makes LSb 0 */
      crc = crc << 1;
      
      /* if MSb of data is 1, set LSb of CRC */
      if (y & (unsigned char)0x80)
	crc = crc | 0x00000001;
      
      /* do the XOR if needed */
      if (xor)
	crc = crc ^ CRC32;
      
      /* shift out MSb of y */
      y = y << 1;
    }
  }
  printf("CRC: %X\n", crc);
  return (crc ? 0 : 1);
}
/*
int
main(void)
{
  struct str_packet p;
  unsigned char * ptr = (unsigned char *)&p;
  int i;
  unsigned crc;
  
  printf("sizeof(str_packet) = %d\n", sizeof(struct str_packet));
  
  srand(getpid());
  for(i=0; i<271-4; i++) {
    ptr[i] = (unsigned char)rand();
  }
  ptr[267] = ptr[268] = ptr[269] = ptr[270] = 0;
  
  crc = check_crc(&p);
  printf("CRC was %X\n", crc);
  
  ptr[267] = (crc & 0xFF000000) >> 24;
  ptr[268] = (crc & 0x00FF0000) >> 16;
  ptr[269] = (crc & 0x0000FF00) >> 8;
  ptr[270] = (crc & 0x000000FF);
  
  crc = check_crc(&p);
  printf("CRC was %X\n", crc);
  
  return 0;
}
*/
