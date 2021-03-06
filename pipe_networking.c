#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int from_client = 0;

  //make WKP
  mkfifo(WKP, 0644);
  printf("made WKP\n");

  //read sp from client message and removes WKP
  from_client = open(WKP, O_RDONLY, 0);
  remove(WKP);
  char sp[HANDSHAKE_BUFFER_SIZE];
  read(from_client, sp, sizeof(sp));
  printf("read sp\n");

  //send response to client
  *to_client = open(sp, O_WRONLY, 0);
  write(*to_client, ACK, sizeof(ACK));
  printf("sent ack\n");

  //receive final response from client
  char response[BUFFER_SIZE];
  int n = read(from_client, response, sizeof(response));
  if (n == -1) printf("read error %d: %s\n", errno, strerror(errno));
  int ra = atoi(response);
  printf("testing for ack");
  if (ra != (atoi(ACK)+1)) {
    printf("fail\n");
    exit(0);
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server = 0;

  //create sp
  char sp[HANDSHAKE_BUFFER_SIZE];
  int pid = getpid();
  int * pidp = &pid;
  sscanf(sp, "%d", pidp);
  mkfifo(sp, 0644);

  //send sp name to server
  *to_server = open(WKP, O_WRONLY);
  write(*to_server, sp, sizeof(sp));

  //get response from server
  from_server = open(sp, O_RDONLY, 0);
  char msg[HANDSHAKE_BUFFER_SIZE];
  int n = read(from_server, msg, sizeof(msg));
  if (n == -1) printf("read error %d: %s\n", errno, strerror(errno));
  //remove sp
  remove(sp);

  int r = atoi(msg) + 1;
  sprintf(msg, "%d", r);
  write(*to_server, msg, sizeof(msg));

  return from_server; 
}
