#define MAX_STR_LEN    80
#define SERVER_PORT    1080
#define CLIENT_PORT    1081
#define ERR_TIMEOUT    2
#define DROP_PROB      0.0
#define CORRUPT_PROB   0.0
#define MAX_PACKET_LEN 271
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

int init_sock( int mode );

unsigned get_addr_by_name( char *hostname );

int send_packet( char *p_data, int p_length, unsigned address );

int recv_packet( char *p_data, int *p_length, float t_out );

