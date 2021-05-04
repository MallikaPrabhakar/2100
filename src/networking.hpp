#ifndef NETWORKING_H
#define NETWORKING_H

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 4263 // equals GAME on keypad

struct Network
{
	static const int SOCKSIZE = sizeof(sockaddr_in);
	static int sockfd;
	static sockaddr_in Server, Client;
	static bool done;
	/*
	 * error codes
	 * 1 = unable to init socket
	 * 2 = unable to bind socket
	 * 3 = unable to send info
	 * 4 = unable to recv info
	 * 5 = unable to set timeout
	*/

	static int initServer();
	static int makeServer();
	static int makeClient();
	static int lookForClient();
	static int sendRequest(sockaddr_in &to, char *msg, int size);
	static int recvRequest(sockaddr_in &from, char *ret, int size);
};

#endif
