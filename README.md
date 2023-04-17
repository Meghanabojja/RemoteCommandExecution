# RemoteCommandExecution

RPC stands for Remote Procedure Call. It is a protocol that allows one computer program to request a service from another program on a remote computer without requiring the programmer to understand the details of the network.

The basic idea behind RPC is that a client program sends a request to a server program using a procedure call. The server receives the request, performs the requested service, and sends a response back to the client.

RPC is often used in distributed computing environments where multiple computers need to work together to perform a task. For example, in a web application, the front-end client might use RPC to communicate with a back-end server to retrieve data or perform a computation.

we use serialization and De serialization for this procedure calls.

compiling serializing  should be executed on both client and server machines.

gcc -g -c serialize.c -o serialize.o

client side command execution should be executed on client side machine.

gcc -g -c calc.c -o calc.o

gcc -g calc.o serialize.o -o client

server side command execution should be done on server machine whose IP address we gave in server.h file.

gcc -g -c server.c -o server.o

gcc -g server.o serialize.o -o server

Now,run
./server

and 
./client 
on client machine. Both our client and server are now up and ready to communicate with each other.
