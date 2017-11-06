/*  Larry Connor
 *  Project 1 (client program)
 *  18-345
 */

#include "sock.c"
#include <iostream.h>

void main()
{
  char *p_data;
  int len;
  unsigned addr;
  int errorint;

  p_data = "Hello There"; //The data that we are sending
  len = 12; //The length of the message (including the /0 at the end)

  sock = init_sock(CLIENT_MODE); //Initialize the port
  addr = get_addr_by_name("sandpiper.ww.andrew.cmu.edu"); //Get our servers address
  if(addr == 0xFFFFFFFF) //If we didn't get a valid address
    cout << "Address not found" << endl;
  else //If we found an address
    {
      errorint = send_packet(p_data, len, addr); //Send the packet
      if (errorint == -1) //If something went wrong
	cout << "Error in Transmission" << endl;
      else //Otherwise print the message
	cout << "Transmission successful: " << p_data << " sent " << endl;
    }
}
