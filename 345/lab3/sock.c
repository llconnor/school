#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <values.h>
#include "sock.h"

static int sock;

static int recv_port;
static int send_port;

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



