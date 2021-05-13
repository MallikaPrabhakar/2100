#include "networking.hpp"

int Network::sockfd, Network::othersockfd;
sockaddr_in Network::Server, Network::other;
bool Network::done;

void Network::initServer(const char *ip)
{
	std::memset(&Server, 0, SOCKSIZE);
	Server.sin_family = AF_INET;
	if (ip == NULL)
		Server.sin_addr.s_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, ip, &Server.sin_addr);
	Server.sin_port = htons(PORT);
}

int Network::makeServer()
{
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 1;
	timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
		return 5;
	initServer();
	if (bind(sockfd, (const sockaddr *)&Server, SOCKSIZE) == -1)
		return 2;
	return 0;
}

int Network::lookForClient()
{
	if (listen(sockfd, 5) == -1)
		return 6;
	socklen_t socklen;
	if ((othersockfd = accept(sockfd, (sockaddr *)&other, &socklen)) == -1)
		return 6;
	char ret;
	if (recvRequest(&ret, 1) == 4)
		return 4;
	if (ret != 'i')
		return 4;
	ret = 'd';
	if (sendRequest(&ret, 1) == 3)
		return 3;
	return 0;
}

int Network::makeClient(const char *ip)
{
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 1;
	othersockfd = sockfd;
	timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
		return 5;
	initServer(ip);
	if (connect(sockfd, (sockaddr *)&Server, SOCKSIZE) == -1)
		return 3;
	other = Server;
	char ret = 'i';
	if (sendRequest(&ret, 1) == 3)
		return 3;
	recvRequest(&ret, 1);
	if (ret != 'd')
		return 4;
	return 0;
}

int Network::sendRequest(const char *msg, int size)
{
	if (sendto(othersockfd, msg, size, MSG_NOSIGNAL, (const sockaddr *)&other, SOCKSIZE) == -1)
		return 3;
	return 0;
}

int Network::recvRequest(char *ret, int size)
{
	socklen_t socklen;
	if (recvfrom(othersockfd, ret, size, MSG_WAITALL, (sockaddr *)&other, &socklen) == -1)
		return 4;
	return 0;
}
