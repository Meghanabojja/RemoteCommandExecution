#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "server.h"
#include "functions.h"

int main(int argc, char **argv){

    int x,a, b;
    printf("Enter your choice\n1.Addition\n2.subraction\n3.multiply\n4.Division\n");
    scanf("%d",&x);
    
    printf("Enter Ist number : ");
    scanf("%d", &a);
    printf("Enter IInd number : ");
    scanf("%d", &b);

    /Invoke the rpc/
    int res = multiply_rpc(x,a, b);

    /Print the result/
    printf("res = %d\n", res);
    return 0; 

    
}
void msg_transfer(buffer_t *client_send_buffer, buffer_t *client_recv_buffer){
    struct sockaddr_in dest;
    int sockfd = 0, rc = 0, recv_size = 0;
    int addr_len;

    dest.sin_family = AF_INET;
    dest.sin_port = htons(SERVER_PORT);
    struct hostent *server_host = (struct hostent *)gethostbyname(SERVER_IP);
    dest.sin_addr = *((struct in_addr *)server_host->h_addr);
    addr_len = sizeof(struct sockaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1){
        printf("socket creation failed\n");
        return;
    }

    rc = sendto(sockfd, client_send_buffer->b, get_buffer_datasize(client_send_buffer), 
                0, (struct sockaddr *)&dest, sizeof(struct sockaddr));

    printf("%s() : %d bytes sent\n", _FUNCTION_, rc);

    recv_size = recvfrom(sockfd, client_recv_buffer->b, get_buffer_length(client_recv_buffer), 
                         0, (struct sockaddr *)&dest, &addr_len);

    printf("%s() : %d bytes received\n", _FUNCTION_, recv_size);
}
/* client_serialize() */
buffer_t *client_serialize(int num1, int num2, int num3){
    buffer_t *send_buffer = NULL;
    init_buffer_of_size(&send_buffer, MAX_RECV_SEND_BUFF_SIZE);

    /* Serialize the first argument */
    serialize(send_buffer, (char *)&num1, sizeof(int));

    /* Serialize the second argument */
    serialize(send_buffer, (char *)&num2, sizeof(int));

    /* Serialize the third argument */
    serialize(send_buffer, (char *)&num3, sizeof(int));

    return send_buffer;
}

int client_deserialize(buffer_t *recv_buffer){
    /* Reconstruct the result obtained from the server */
    int result = 0;
    deserialize((char *)&result, recv_buffer, sizeof(int));
    return result;
}

void init_rpc_infra(){
    /* Initialize any necessary infrastructure before starting RPC, if any */
}
int calculator(int x, int a, int b){
    /* Initialize RPC buffers */
    init_rpc_infra();

    /*Serialize/Marshal the arguments */
    /* Signature: buffer_t* ( client_serialize) <Arg1, Arg2, ...> */
    buffer_t *send_buffer = client_serialize(x, a, b);
    buffer_t *recv_buffer = NULL;

    /* Prepare for receiving the data from the server */
    init_buffer_of_size(&recv_buffer, MAX_RECV_SEND_BUFF_SIZE);

    /*Send the serialized data to the server and wait for the reply */
    /* Function that will work for all RPCs */
    /* Signature: void (msg_transfer) <buffer_t *, buffer_t *> */
    msg_transfer(send_buffer, recv_buffer);

    /* After sending the data to the server, client should free the memory
     * held by the send_buffer */
    free_buffer(send_buffer);
    send_buffer = NULL;

    

    /*Unmarshal the serialized data (result) received from the server,
     * and reconstruct the RPC return type */
    /* Signature: <rpc return type> (client_deserialize) <buffer_t *> */
    int result = client_deserialize(recv_buffer);

    /* Client has successfully reconstructed the result object from
     * serialized data received from the server. Time to free recv_buffer */
    free_buffer(recv_buffer);

    return result;
}
