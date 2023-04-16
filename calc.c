#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/socket.h>
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
