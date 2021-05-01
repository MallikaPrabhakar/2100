#include "networking.hpp"

int Network::sockfd;
sockaddr_in Network::Server, Network::Client;

int Network::initServer()
{
	std::memset(&Server, 0, SOCKSIZE);
	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = inet_addr("127.0.0.1");
	Server.sin_port = htons(PORT);
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return 1;
	return 0;
}

int Network::makeServer()
{
	if (initServer() == 1)
		return 1;
	if (bind(sockfd, (const sockaddr *)&Server, SOCKSIZE) == -1)
		return 2;
	return 0;
}

int Network::makeClient()
{
	if (initServer() == 1)
		return 1;
	socklen_t size = SOCKSIZE;
	if (sendto(sockfd, "init", 5, MSG_CONFIRM, (const sockaddr *)&Server, size) == -1)
		return 2;
	char ret[5];
	recvfrom(sockfd, ret, 5, MSG_WAITALL, (sockaddr *)&Server, &size);
	return 0;
}

int Network::lookForClient()
{
	char ret[5];
	socklen_t size = SOCKSIZE;
	std::memset(&Client, 0, SOCKSIZE);
	recvfrom(sockfd, ret, 5, MSG_WAITALL, (sockaddr *)&Client, &size);
	if (sendto(sockfd, "done", 5, MSG_CONFIRM, (const sockaddr *)&Client, size) == -1)
		return -1;
	return 0;
}
