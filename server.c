#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
/* This function takes three integer inputs, and returns an integer output.
   The input x is used as the 'operator', which determines the operation
   to be performed on inputs a and b. */
int calc(int x, int a , int b){
  int y=0;
  switch(x) {
    case 1:
      y = a*b;
      break;
    case 2:
      y = a - b;
      break;
    case 3:
      y = a + b;
      break;
    case 4:
      if (b != 0) {
        y = a/b;
      }
      
      break;
    default:
      exit(1); // Exit the program for invalid input
  }
  return y;
}


int main(int argc, char **argv) {


    int udp_sock_fd = 0, len, addr_len, opt = 1, reply_msg_size = 0;
    struct sockaddr_in server_addr, client_addr;

    if ((udp_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Socket creation failed\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    addr_len = sizeof(struct sockaddr);

    if (setsockopt(udp_sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(udp_sock_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(udp_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        printf("Socket bind failed\n");
        exit(1);
    }

    /* Prepare server memory buffers to send and receive serialized data */

    buffer_t *server_recv_ser_buffer = NULL,
               *server_send_ser_buffer = NULL;

    init_buffer_of_size(&server_recv_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

    init_buffer_of_size(&server_send_ser_buffer, MAX_RECV_SEND_BUFF_SIZE);

    printf("Server ready to service RPC calls\n");

