#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include "server.h"
#include "functions.h"
/* This function takes three integer inputs, and returns an integer output.
   The input x is used as the 'operator', which determines the operation
   to be performed on inputs a and b. */
int calc(int x, int a , int b){
  int y=0;
  switch(x) {
    case 1:
      y = a+b;
      break;
    case 2:
      y = a - b;
      break;
    case 3:
      y = a * b;
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

void server_msg(buffer_t *recv_buffer, buffer_t *send_buffer){

    /*  Deserializing of Arguments */
    /* Signature: <rpc return type> server deserialize (buffer_t *ser_data) */
    /* Deserializing of Arguments is done here. Reconstruct the Arguments */

    int result = server_deserialize(recv_buffer);

    /* Serialize the result */
    server_serialize(result, send_buffer);
    /*printf("result");*/
}

int server_deserialize(buffer_t *recv_buffer){

    int x, a, b;
    deserialize((char *)&x, recv_buffer, sizeof(int));
    deserialize((char *)&a, recv_buffer, sizeof(int));   
    deserialize((char *)&b, recv_buffer, sizeof(int));

    /* Call the Actual RPC and return its result */
    return calc(x, a, b);
}

void server_serialize(int result, buffer_t *send_buffer){

    serialize(send_buffer, (char *)&result, sizeof(int));
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

READ:
    reset_serialize_buffer(server_recv_ser_buffer);

    /* Receive the data from client in local buffer */

    len = recvfrom(udp_sock_fd, server_recv_ser_buffer->b, get_buffer_length(server_recv_ser_buffer),
                   0, (struct sockaddr *)&client_addr, &addr_len);
    printf("No of bytes received from client = %d\n", len);

    /* Prepare the buffer to store the reply message to be sent to client */
    reset_serialize_buffer(server_send_ser_buffer);

    server_msg(server_recv_ser_buffer, /* Serialized data which came from client */
                           server_send_ser_buffer); /* Empty serialized buffer */

    /* Send the serialized result data back to client */
    len = sendto(udp_sock_fd, server_send_ser_buffer->b, get_buffer_datasize(server_send_ser_buffer),
                 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

    printf("RPC server replied with %d bytes message\n", len);
    goto READ;
    return 0;
}
