// HttpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ParseUrl.h"
#include "HttpGet.h"
#include "DisplayResult.h"
#include "Base64.h"

#define HTTP_PORT 80

int main(int argc, char **argv)
{
	int client_connection;

	char *url;
	char *host, *path;

	struct hostent *host_name;
	struct sockaddr_in host_address;
	PADDRINFOA host_addr_info;

#ifdef WIN32
	WSADATA wsaData;
#endif

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s: <URL>\n", argv[0]);
		return 1;
	}

	url = _strdup(argv[1]);

	if (ParseUrl(url, &host, &path) == -1)
	{
		fprintf(stderr, "Malformed URL '%s'.\n", argv[1]);
		return 1;
	}

	printf("Connecting to host '%s'.\n", host);

#ifdef WIN32
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		fprintf(stderr, "Error initialising winsock\n");
		return 2;
	}
#endif

	client_connection = socket(PF_INET, SOCK_STREAM, 0);

	if (!client_connection)
	{
		perror("Unable to create socket");
		return 2;
	}

	if (getaddrinfo(host, "http", NULL, &host_addr_info) != 0)
	{
		perror("Error resolving hostname");
		return 3;
	}

	if (host_addr_info == NULL)
	{
		fprintf(stderr, "No results returned for hostname lookup\n");
		return 3;
	}

	for (struct addrinfo *ptr = host_addr_info; ptr != NULL; ptr = ptr->ai_next)
	{
		switch (ptr->ai_family)
		{
		case AF_UNSPEC:
			printf("unspecified family\n");
			break;
		case AF_INET:
			printf("inet\n");
			break;
		case AF_INET6:
			printf("inet6\n");
			break;
		default:
			printf("other address family %d\n", ptr->ai_family);
		}
	}

	host_address.sin_family = AF_INET;
	host_address.sin_port = htons(HTTP_PORT);
	struct sockaddr_in * addr = (struct sockaddr_in*) host_addr_info->ai_addr;
	memcpy(&host_address.sin_addr, &addr->sin_addr, sizeof(struct in_addr));

	char buf[16];
	inet_ntop(AF_INET, &host_address.sin_addr, buf, sizeof(buf));
	printf("IPv4 address %s\n", buf);

	if (connect(client_connection, (struct sockaddr *) &host_address, sizeof(host_address)) == -1)
	{
		perror("Unable to connect to host");
		return 4;
	}

	printf("Retrieving document: '%s'\n", path);

	if (HttpGet(client_connection, path, host) == -1)
	{
		fprintf(stderr, "Error sending request\n");
		return 4;
	}

	DisplayResult(client_connection);

	printf("Shutting down.\n");

#ifdef WIN32
	if (closesocket(client_connection) == -1)
#else
	if (close(client_connection) == -1)
#endif
	{
		perror("Error closing connection");
		return 5;
	}

#ifdef WIN32
	WSACleanup();
#endif

    return 0;
}

