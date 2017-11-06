/*     18-345
*      Proj 3
*      Larry Connor and Scott Cape
*/
#include <iostream.h>
#include <unistd.h>
#include <fstream.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <values.h>
//#include "sock.h"

#define MAX_STR_LEN    80
#define SERVER_PORT    1080
#define CLIENT_PORT    1081
#define ERR_TIMEOUT    2
#define DROP_PROB      0.05
#define CORRUPT_PROB   0.00005
#define MAX_PACKET_LEN 272
#define SERVER_MODE    1
#define CLIENT_MODE    0

/* CRC32 generator */
#define CRC32 0x04C11DB7

/* Packet type constants */
#define C_REQUEST 0x01
#define C_CONFIRM 0x02
#define D_DATA    0x41
#define D_ACK     0x42
#define D_NACK    0x43
#define T_CLOSE   0x81
#define T_CONFIRM 0x82

typedef unsigned int in_addr_t;

struct str_packet{

  in_addr_t dest;
  in_addr_t source;
  unsigned char PT;
  unsigned char SEQ;
  unsigned char ACK;
  unsigned char LENGTH;
  char data[255];
  unsigned int CRC;
};

int init_sock( int mode );

unsigned get_addr_by_name( char *hostname );

int send_packet( char *p_data, int p_length, unsigned address );

int recv_packet( char *p_data, int *p_length, float t_out );



static int sock;

static int recv_port;
static int send_port;
unsigned crc = 0;

#define TIME_OUT 1

/************************************************************************/
int yes_no( double prob ) {
  double r;

  r = (double)random()/MAXLONG;
  return( (r <= prob) );
}

/************************************************************************/
int r_range( int max ) {
  double r;
  int i;

  r = (double)random()/MAXLONG*max;
  i = r;
  return( i );
}

/************************************************************************/
int init_sock( int mode )
{
  char               hostname[MAX_STR_LEN];
  int                host_addr_len;
  struct hostent     *hostentry;
  struct sockaddr_in *host_addr;
  struct timeval     tp;

  /* Set up the send and recv ports */
  if (mode == SERVER_MODE) {
    send_port = CLIENT_PORT;
    recv_port = SERVER_PORT;
  } else {
    send_port = SERVER_PORT;
    recv_port = CLIENT_PORT;
  }

  /* Initialize data structures */
  host_addr = (struct sockaddr_in *)malloc( sizeof( struct sockaddr_in ) );
  if (!host_addr)
    { fprintf( stderr, "Unable to allocate memory.\n" ); exit( 1 ); }

  /* Create the socket */
  if ((sock = socket( AF_INET, SOCK_DGRAM, 0 ))==-1)
    { perror( "socket " ); exit( -1 ); }

  /* Get the local hostname and bind the socket */
  gethostname( hostname, MAX_STR_LEN );
  hostentry = gethostbyname( hostname );
  host_addr->sin_family = hostentry->h_addrtype;
  if (mode == SERVER_MODE)
    host_addr->sin_port = htons( SERVER_PORT );
  else
    host_addr->sin_port = htons( CLIENT_PORT );
  bcopy( hostentry->h_addr, &host_addr->sin_addr.s_addr,
         hostentry->h_length );
  host_addr_len = sizeof( struct sockaddr_in );
  if (bind( sock, (struct sockaddr *)host_addr, host_addr_len )==-1)
    { perror( "init_sock " ); exit( -1 ); }

  /* Initialize the random number generator */
  gettimeofday( &tp, NULL );
  srandom( tp.tv_sec ^ tp.tv_usec );

  free( host_addr );

  return( sock );
}
/************************************************************************/
unsigned get_addr_by_name( char *hostname )
{
  struct hostent *hostentry;

  hostentry = gethostbyname( hostname );
  if (!hostentry)
    return( 0xffffffff );

  return( ntohl( *(unsigned*)hostentry->h_addr_list[0] ) );
}

/************************************************************************/
int send_packet(
    char     *p_data,
    int      p_length,
    unsigned address )
{
  struct sockaddr_in *dest_addr;
  int                dest_addr_len;

  /* Check input values */
  if (!p_data)
    return( -1 );
  if (p_length>MAX_PACKET_LEN)
    return( -1 );

  /* Create the data structures */
  dest_addr = (struct sockaddr_in *)malloc( sizeof( struct sockaddr_in ) );
  if (!dest_addr)
    { fprintf( stderr, "Unable to allocate memory.\n" ); exit( 1 ); }

  /* Set up the address */
  dest_addr->sin_family = AF_INET;
  dest_addr->sin_port = htons( send_port );
  dest_addr->sin_addr.s_addr = htonl( address );
  dest_addr_len = sizeof( struct sockaddr_in );

  /* Send the packet */
  if (sendto( sock, p_data, p_length, 0, (struct sockaddr *)dest_addr, dest_addr_len )==-1)
    { perror( "send_sdlp_packet " ); exit( 1 ); }

  /* Free up the data structures */
  free( dest_addr );

  return( 0 );
}

/************************************************************************/
int recv_packet(
    char               *p_data,
    int                *p_length,
    float              t_out )
{
  struct sockaddr_in *source_addr;
  int                source_addr_len;
  fd_set             readfds[1];
  int                i;
  int                nfds;
  char               buf[MAX_PACKET_LEN];
  int                buflen = MAX_PACKET_LEN;
  int                datalen;
  struct timeval     timeout;
  int                error;

  /* Check input values */
  if (!p_data)
    return( -1 );

  /* Create the data structures */
  source_addr = (struct sockaddr_in *)malloc( sizeof( struct
sockaddr_in ) );
  if (!source_addr)
    { fprintf( stderr, "recv_packet : out of memory\n" ); return( -1 ); }
  nfds = getdtablesize();

  timeout.tv_sec = t_out;
  timeout.tv_usec = (t_out - timeout.tv_sec)*100000;

  FD_ZERO( readfds );
  FD_SET( sock, readfds );

  /* Wait for socket to have a packet */
  error = select( nfds, readfds, 0, 0, &timeout );
  if (error==-1)
    { perror( "recv_packet " ); exit( 1 ); }
  if (error==0)
   return( ERR_TIMEOUT );

  source_addr->sin_family      = AF_INET;
  source_addr->sin_port        = htons( recv_port );
  source_addr->sin_addr.s_addr = INADDR_ANY;
  source_addr_len = sizeof( struct sockaddr_in );

  datalen = recvfrom( sock, buf, buflen, 0, (struct sockaddr *)source_addr, &source_addr_len );
  if (datalen==-1)
    { perror( "recv_packet " ); exit( 1 ); }

#ifdef DEBUG
  printf( "Packet received from 0x%8x, Payload type 0x%2x\n",
          ntohl( *(unsigned *)(buf+4) ), buf[8] & 0xff );
#endif

  for (i=0;i<(buflen*8);i++)
    if (yes_no(CORRUPT_PROB)) {
#ifdef DEBUG
      printf( "Bit corrupted\n" );
#endif
      buf[i/8] ^= (1 << (i%8));
    }

  *p_length = datalen;
  bcopy( buf, p_data, *p_length );

  free( source_addr );

  /* There's a packet, but we're going to drop it */
  if (yes_no(DROP_PROB)) {
#ifdef DEBUG
    printf( "Packet dropped.\n" );
#endif
    return( ERR_TIMEOUT );
  }

  return( 0 );
}

unsigned int calc_crc(struct str_packet *packet)
{
  unsigned char *p = (unsigned char *) packet;
  unsigned char y;
  int i, j, xor;
  
  crc = 0;

  for (i = 0; i < 267; i++) {
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
  //printf("CRC: %X\n", crc);
  return crc;
}

int check_crc(struct str_packet *packet)
{
  return(calc_crc(packet) == packet -> CRC);
}

int send_pack(char pack_type, str_packet *data, int length, int sequence)
{
  data -> LENGTH = 0;
  data -> SEQ = 0;
  data -> ACK = 0;
  if(pack_type == 'r')
    {
      data -> PT = C_REQUEST;
      data -> LENGTH = length;
      //      errorint = send_packet(p_data, len, addr);
    }
  else if (pack_type == 'c')
    {
      data -> PT = C_CONFIRM;
    }
  else if (pack_type == 't')
    {
      data -> PT = T_CLOSE;
    }
  else if (pack_type == 'T')
    {
      data -> PT = T_CONFIRM;
    }
  else if (pack_type == 'd')
    {
      data -> PT = D_DATA;
      data -> LENGTH = length;
      data -> SEQ = sequence;
    }
  else if (pack_type == 'a')
    {
      data -> PT = D_ACK;
      data -> ACK = sequence;
    }
  else if (pack_type == 'n')
    {
      data -> PT = D_NACK;
      data -> ACK = sequence;
    }

  data -> CRC = calc_crc(data);

  //  cout << data -> data << " " << data -> dest << " " << data -> crc << endl;
  if(send_packet((char *)data, 272, data -> dest) != 0)
    {
      //      cout << "Failed" << endl;
      return 0;
    }
  cout << "Sent #" << (int) data -> SEQ << " Length = " << (int)data -> LENGTH << endl;
  return 1;
}

void equals(str_packet &one, str_packet &two)
{
  one.source = two.source;
  one.dest = two.dest;
  one.ACK = two.ACK;
  one.PT = two.PT;
  one.SEQ = two.SEQ;
  one.LENGTH = two.LENGTH;
  one.data = two.data;
}

int fill_data(str_packet &sender, FILE *infile/*ifstream &infile*/, int &canfill)
{
  int loop;
  if(canfill < 3)
    {
      //cout << "Can fill == 0" << endl;
      //canfill = 0;
//      cin >> loop;
      return 1;
    }

  for(loop = 0; loop < 255 && fread(&sender.data[loop], 1, 1, infile) == 1/*infile >> sender.data[loop]*/; loop++)
    {
    }
  sender.LENGTH = loop;
  if(loop == 0)
    return 0;
//  cout << "Packet size = " << loop;
  return 1;  
}

int main()
{  
  char *p_data;
//  char data[275];
  int packtype, packlength;
  int len;
  int length;
  int sizerec;
  int numbadpackets = 0;
  unsigned addr, thisaddr;
  int errorint;
  char thisname[255];
  int thisnamelen = 255;
  char servername[255];
  str_packet sender, sender2, sender3, sender4, sender5, sender6;
  str_packet *senderptr;
  FILE *infile;
  sock = init_sock(CLIENT_MODE);
  senderptr = &sender;

  gethostname(thisname, thisnamelen); 
  thisaddr = htonl(get_addr_by_name(thisname));
  cout << "What computer would you like to connect to?" << endl;
  cin >> servername;
  addr = htonl(get_addr_by_name(servername)); 
  if(addr == 0xFFFFFFFF) 
    cout << "Address Not Found" << endl;
  else
    {
      //      printf("addr = %08X\n", addr);
      infile = fopen("University_Center.gif", "r");
      int loop;
      for(loop = 0; loop < 5; loop++)
	{      
	  sender.dest = addr;
	  sender.source = thisaddr;
      
      sender.data[0] = 'U';
      sender.data[1] = 'n';
      sender.data[2] = 'i';
      sender.data[3] = 'v';
      sender.data[4] = 'e';
      sender.data[5] = 'r';
      sender.data[6] = 's';
      sender.data[7] = 'i';
      sender.data[8] = 't';
      sender.data[9] = 'y';
      sender.data[10] = '_';
      sender.data[11] = 'C';
      sender.data[12] = 'e';
      sender.data[13] = 'n';
      sender.data[14] = 't';
      sender.data[15] = 'e';
      sender.data[16] = 'r';
      sender.data[17] = '.';
      sender.data[18] = 'g';
      sender.data[19] = 'i';
      sender.data[20] = 'f';
      sender.data[21] = '\0';

      for(int startloop = 22; startloop < 255; startloop++)
	sender.data[startloop] = 0;
      sender.dest = addr;
      sender.source = thisaddr;
      send_pack('r', &sender, 22, 0);


	  length = 272;
	  if(recv_packet((char *)&sender, &length, TIME_OUT) == 0 && check_crc(senderptr) == 1)
	    {
	      if(senderptr -> PT == C_CONFIRM)
		{
		  break;
		}
	    }
	  for(long timeloop = 0; timeloop < 30000000; timeloop++) //+/- 3sec
	    timeloop = timeloop;
	}
      if(loop == 5)
	{
	  cout << "Could not connect...program exiting" << endl;
	  return -1;
	}
      
      int sequence = 0;
      int canfill = 3;
      int wasbad = 0;
      char breaker;
      while(1)
	{
	  while(wasbad > 0)
	    {
	      wasbad--;
	      length = 272;
	      recv_packet((char *)&sender5, &length, TIME_OUT);
	    }
	  cout << "SEQ at beginning " << sequence << endl;
	  if(fill_data(sender, infile, canfill) == 1)
	    {
	      sender.dest = addr;
	      sender.source = thisaddr;
	      send_pack('d', &sender, sender.LENGTH, sequence % 256);
	      sequence++;
	      canfill++;
	      if(fill_data(sender2, infile, canfill) == 1)
		{
		  sender2.dest = addr;
		  sender2.source = thisaddr;
		  send_pack('d', &sender2, sender2.LENGTH, sequence % 256);
		  sequence++;
		  canfill++;
		  if(fill_data(sender3, infile, canfill) == 1)
		    {
		      sender3.dest = addr;
		      sender3.source = thisaddr;
		      send_pack('d', &sender3, sender3.LENGTH, sequence % 256);
		      sequence++;
		      length = 272;
		      canfill++;
		      if(recv_packet((char *)&sender4, &length, TIME_OUT) == 0 && check_crc(&sender4))
			{		  
			  if(sender4.PT == D_ACK && sender4.ACK == sequence - 2)
			    {
			    here:
			      length = 272;
			      if(recv_packet((char *)&sender5, &length, TIME_OUT) == 0 && check_crc(&sender5))
				{
				  if(sender5.PT == D_ACK && sender5.ACK == sequence - 1)
				    {
				      here1:
				      length = 272;
				      if(recv_packet((char *)&sender6, &length, TIME_OUT) == 0 && check_crc(&sender6))
					{
					  if(sender6.PT == D_ACK && sender6.ACK == sequence)
					    {
					      canfill = 3;
					    } //All three were okay
					  else if(sender6.PT == D_NACK)
					    {
					      //cout << "D_NACK1 " << (int)sender6.ACK << endl;
					      canfill = 2;
					      equals(sender, sender3);
					      //cin >> breaker;
					      sequence = sender6.ACK; //D_NACK or random pack
					      
					    }
					  else
					    {
					      //cout << "Else1 " << (int) sender6.ACK << " " << (int)sender6.PT << " " << (int) sender6.SEQ << " " << sequence << endl;
					      canfill = 2;
					      equals(sender, sender3);
					      // cin >> breaker;
					      sequence -= 1;// timeout
					    }
					}
				      else
					{
					  canfill = 2;
					  equals(sender, sender3);
					  //cout << "Timeout on 3rd" << endl;
					  // cin >> breaker;
					  sequence -= 1;// timeout
					}
				    }
				  else if(sender5.PT == D_NACK)
				    {
				      canfill = 1;
				      //cout << "D_NACK2 " << (int)sender5.ACK << endl;
				      equals(sender, sender2);
				      equals(sender2, sender3);
				      //cin >> breaker;
				      sequence = sender5.ACK; //D_NACK
				    }
				  else
				    {
				      //cout << "Else 2"<< endl;
				      wasbad = 1;
				      canfill = 1;
				      equals(sender, sender2);
				      equals(sender2, sender3);
				      //cin >> breaker;
				      sequence -= 2; //timeout
				    }
				}
			      else
				{
				  wasbad = 1;
				  canfill = 1;
				  equals(sender, sender2);
				  equals(sender2, sender3);
				  //cout << "Timeout 2nd" << endl;
				  //cin >> breaker;
				  sequence -= 2; //timeout
				}
			    }	  
			  else if(sender4.PT == D_NACK)
			    {
			      canfill = 0;
			      //cout << "D_NACK3 " << (int)sender4.ACK << endl;
			      //cin >> breaker;
			      sequence = sender4.ACK; //D_NACK
			      
			    }
			  else
			    {
			      //cout << "Else3 " << (int) sender4.ACK << " " << (int)sender4.PT << " " << (int) sender4.SEQ << " " << sequence << endl;
			      canfill = 0;
			      wasbad = 2;
			      sequence -= 3;//timeout
			      //cin >> breaker
			    }
			}
		      else
			{
			  wasbad = 2;
			  canfill = 0;
			  sequence -= 3;//timeout

			  //cin >> breaker;
			}
		    }
		  else //If only two packets could be sent
		    {
		      length = 272;
		      if(recv_packet((char *)&sender4, &length, TIME_OUT) == 0 && check_crc(&sender4))
			{
			  if(sender4.PT == D_ACK && sender4.ACK == sequence - 1)
			    {
			      length = 272;
			      if(recv_packet((char *)&sender5, &length, TIME_OUT) == 0 && check_crc(&sender5))
				{
				  if(sender5.PT == D_ACK && sender5.ACK == sequence)
				    {
				      break;
				    }
				  else if(sender5.PT == D_NACK)//Incorrect ACK
				    {	
				      canfill = 2;

				      cout << "D_NACK4 " << (int)sender5.ACK << endl;
				      //cin >> breaker;
				      equals(sender, sender2);
				      sequence = sender5.ACK;
				    }
				  else
				    {
				      cout << "Else4" << (int)sender5.ACK << endl;
				      canfill = 2;
				      //cin >> breaker;
				      equals(sender, sender2);
				      sequence -= 2;
				    }
				}
			      else //Timeout
				{
				  canfill = 2;
				  cout << "Timeout on 2nd (2)" << endl;
				  //cin >> breaker;
				  equals(sender, sender2);
				  sequence -= 2;
				}
			    }
			  else if(sender4.PT == D_NACK)
			    {
			      canfill = 1;
			      cout << "D_NACK5 " << (int)sender4.ACK << endl;
			      //cin >> breaker;
			      sequence = sender4.ACK;
			    }
			  else
			    {
			      canfill = 1;
			      wasbad = 1;
			      cout << "Else5" << (int)sender4.ACK << endl;
			      //cin >> breaker;
			      sequence -= 1;
			    }
			}
		      else
			{
			  recv_packet((char *)&sender4, &length, TIME_OUT);
			  canfill = 1;
			  wasbad = 1;
			  cout << "Timeout on 1st (2)" << endl;
			  //cin >> breaker;
			  sequence -= 1;
			}
		    }
		}
	      else //If only one packet could be sent
		{
		  length = 272;
		  if(recv_packet((char *)&sender4, &length, TIME_OUT) == 0 && check_crc(&sender4))
		    {
		      if(sender4.PT == D_ACK && sender4.ACK == sequence)
			break;
		      else if(sender4.PT == D_NACK)//Incorrect ACK
			{
			  canfill = 2;
			  //cout << "D_NACK6 " << (int)sender4.ACK << endl;
			  //cin >> breaker;
			  sequence = sender4.ACK;
			}
		      else
			{
			  canfill = 2;
			  //cout << "Else6 " << (int) sender4.ACK << " " << (int)sender4.PT << " " << (int) sender4.SEQ << " " << sequence << endl;
			  //cin >> breaker;
			  sequence -= 1;
			}
		    }
		  else
		    {
		      canfill = 2;
		      //cout << "Timeout on 1st (3) " << endl;
		      //cin >> breaker;
		      sequence -= 1;
		    }
		}
	    }
	  else
	    break;
	}
      for(int closeloop = 0; closeloop < 5; closeloop++)
	{
	  sender.dest = addr;
	  sender.source = thisaddr;
	  send_pack('t', &sender, 0, 0);
	  length = 272;
	  if(recv_packet((char*)&sender, &length, TIME_OUT) == 0 && check_crc(senderptr))
	    {
	      if(sender.PT = T_CONFIRM)
		{
		  return 0;
		}
	    }
	  for(int timerloop = 0; timerloop < 10000000; timerloop++)
	    timerloop = timerloop;
	}
    }
  //  cout << "Failed to Close Properly" << endl;
  return 1;
}

/*struct str_packet{

  //  in_addr_t dest;
  //  in_addr_t source;
  unsigned char PT;
  unsigned char SEQ;
  unsigned char ACK;
  unsigned char LENGTH;
  char data[255];
  unsigned char CRC[4];

}str_packet;*/


