// HttpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HttpHandler.h"

#define HTTP_PORT 80

int main()
{
	int listen_sock;
	int connect_sock;
	char on = 1;
	struct sockaddr_in local_addr;
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(client_addr);
#ifdef WIN32
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		perror("Unable to initialise winsock");
		return 0;
	}
#endif

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Unable to create listening socket");
		return 0;
	}

	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		perror("Setting socket options");
		return 0;
	}

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(HTTP_PORT);
	local_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	// local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listen_sock, (struct sockaddr *) &local_addr, sizeof(local_addr)) == -1)
	{
		perror("Unable to bind to port");
		return 0;
	}

	if (listen(listen_sock, 5) == -1)
	{
		perror("Unable to set socket backlog");
		return 0;
	}

	while ((connect_sock = accept(listen_sock, (struct sockaddr *) &client_addr, &client_addr_len)) != -1)
	{
		// TODO: ideally this would spawn a new thread
		ProcessHttpRequest(connect_sock);
	}

	if (connect_sock == -1)
	{
		perror("Unable to accept socket");
		return 0;
	}

    return 0;
}

