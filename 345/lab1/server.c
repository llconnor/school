/*  Larry Connor
 *  Project 1 (server code)
 *  18-345
 */
#include "sock.c"
#include <iostream.h>

void main()
{
  char stuff[15];
  int *len;
  int result = 1;
  sock = init_sock(SERVER_MODE); //Initialize the ports
  result = recv_packet(stuff,len,10); //Wait 10 seconds to received a packet
  if(result == 0) //If we got everything okay
    cout << stuff << " Received" << endl;
  else if(result == -1) //If there was some error
    cout << "ERROR with Transmission" << endl;
  else if(result == 2) //If ten seconds
    cout << "ERROR: Operation timed out" << endl;
}
