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