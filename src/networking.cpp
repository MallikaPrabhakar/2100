#include "networking.hpp"

int Network::sockfd;
sockaddr_in Network::Server, Network::Client, Network::other;
bool Network::done;

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
	timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
		return 5;
	if (sendto(sockfd, "init", 5, MSG_CONFIRM, (const sockaddr *)&Server, SOCKSIZE) == -1)
		return 3;
	char ret[5];
	if (recvfrom(sockfd, ret, 5, MSG_WAITALL, (sockaddr *)&Server, &size) == -1)
		return 4;
	return 0;
}

int Network::lookForClient()
{
	char ret[5];
	socklen_t size = SOCKSIZE;
	timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
		return 5;
	std::memset(&Client, 0, SOCKSIZE);
	if (recvfrom(sockfd, ret, 5, MSG_WAITALL, (sockaddr *)&Client, &size) == -1)
		return 4;
	if (sendto(sockfd, "done", 5, MSG_CONFIRM, (const sockaddr *)&Client, SOCKSIZE) == -1)
		return 3;
	return 0;
}

int Network::sendRequest(char *msg, int size)
{
	if (sendto(sockfd, msg, size, MSG_CONFIRM, (const sockaddr *)&other, SOCKSIZE) == -1)
		return 3;
	return 0;
}

int Network::recvRequest(char *ret, int size)
{
	socklen_t socklen;
	if (recvfrom(sockfd, ret, size, MSG_WAITALL, (sockaddr *)&other, &socklen) == -1)
		return 4;
	return 0;
}
