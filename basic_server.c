#include "pipe_networking.h"

static void sighandler(int n){
  if(n == SIGINT){
    printf("[Server] removing well known pipe\n");
    remove(WKP_NAME);
    printf("[Server] exiting...\n\n");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int i;
  int to_client;
  int from_client;
  char msg_buff[BUFFER_SIZE];

  while(1){
    from_client = server_handshake(&to_client);

    while(read(from_client, msg_buff, BUFFER_SIZE)){
      printf("[Server] recieved -%s-\n", msg_buff);

      for(i = 0; i < strlen(msg_buff); i++)
        msg_buff[i] += 5;

      write(to_client, msg_buff, BUFFER_SIZE);
      printf("[Server] sending back -%s-\n", msg_buff);
    }
    printf("[Server] Client disconnected. Restarting process\n");
  }
}
