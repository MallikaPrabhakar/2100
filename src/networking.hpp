#ifndef NETWORKING_H
#define NETWORKING_H

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#define PORT 4263 // equals GAME on keypad

struct Network
{
	static const int SOCKSIZE = sizeof(sockaddr_in);
	static int sockfd;
	static sockaddr_in Server, Client;

	static int initServer();
	static int makeServer();
	static int makeClient();
	static int lookForClient();
};

#endif
