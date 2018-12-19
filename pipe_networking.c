#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  // make a well known pipe
  printf("\n[Server] handshake: creating a well known pipe\n");
  remove(WKP_NAME);
  int wkp = mkfifo(WKP_NAME, 0666);

  if(wkp == -1){
    printf("[Server] handshake: something went wrong! Exiting...\n");
    exit(0);
  }
  printf("[Server] handshake: well known pipe created! Opening on read end and waiting for client to connect\n");

  int downstream_pipe = open(WKP_NAME, O_RDONLY);

  printf("[Server] handshake: client connected! removing well known pipe\n");
  remove(WKP_NAME);
  printf("[Server] handshake: well known pipe removed!\n");

  printf("[Server] handshake: waiting for private pipe name from client\n");

  int pp;
  char pp_str[15];
  read(downstream_pipe, &pp, 4);
  snprintf(pp_str, 10,"%d", pp);

  printf("[Server] handshake: private pipe name recieved!\n");
  printf("[Server] handshake: connecting to private pipe\n");

  int upstream_pipe = open(pp_str, O_WRONLY);

  printf("[Server] handshake: connected! writing acknowledgement.\n");

  write(upstream_pipe, ACK, strlen(ACK) + 1);

  printf("[Server] handshake: written! waiting for client acknowledgement\n");

  char client_message[HANDSHAKE_BUFFER_SIZE];

  read(upstream_pipe, client_message, HANDSHAKE_BUFFER_SIZE);

  printf("[Server] handshake: recieved with message %s\n", client_message);
  printf("[Server] handshake: three way handshake is done!\n");

  *to_client = upstream_pipe;
  return downstream_pipe;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("[Client] handshake: Creating private pipe\n");

  char pid_str[15];
  int pid = getpid();
  snprintf(pid_str, 10,"%d",(int)getpid());

  remove(pid_str);
  int pp = mkfifo(pid_str, 0666);

  if(pp == -1){
    printf("[Client] handshake: Something went wrong! Exiting...\n");
    exit(0);
  }

  printf("[Client] handshake: Connecting to server's well known pipe\n");

  int downstream_pipe = open(WKP_NAME, O_WRONLY);

  printf("[Client] handshake: Connected! Sending private pipe name\n");

  write(downstream_pipe, &pid, sizeof(int));

  printf("[Client] handshake: Sent! Waiting for server connection to private pipe\n");

  int upstream_pipe = open(pid_str, O_RDONLY);

  char server_message[HANDSHAKE_BUFFER_SIZE];

  read(upstream_pipe, server_message, HANDSHAKE_BUFFER_SIZE);

  printf("[Client] handshake: Server connected with message %s!\n", server_message);
  printf("[Client] handshake: Removing private pipe\n");
  remove(pid_str);
  printf("[Client] handshake: Private Pipe Removed!\n");

  printf("[Client] handshake: Writing acknowledgement to server\n");

  write(upstream_pipe, ACK, strlen(ACK) + 1);

  printf("[Client] handshake: Written! Three way handshake is done!\n");

  // set values
  *to_server = downstream_pipe;
  return upstream_pipe;
}
