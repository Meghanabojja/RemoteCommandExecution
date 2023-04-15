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
