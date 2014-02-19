//Papa Florin, 324CA

#ifndef LIB
#define LIB

#define HOST "127.0.0.1"
#define PORT1 10000
#define PORT2 10001

#define MSGSIZE 1400
#define PKTSIZE 1395
#define ACK 0
#define NACK 1
#define DATA 2

typedef struct {
  int len;
  char payload[MSGSIZE];
} msg;

typedef struct{
	int nr_seq;
	char type;
	char payload[PKTSIZE];
} pkt;

typedef struct{
	char sum;
	char payload[PKTSIZE+4];
} pkt2;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);
int recv_message_timeout(msg *m, int timeout);

#endif

