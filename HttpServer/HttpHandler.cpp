#include "stdafx.h"
#include "HttpHandler.h"
#include "ReadLine.h"

#define BUF_LEN 255

int SendLine(int connection, const char * line)
{
	//TODO: account for short writes.
	if (send(connection, line, strlen(line), 0) < (int)strlen(line))
	{
		perror("Trying to respond");
		return -1;
	}
	else
	{
		return 0;
	}
}

void RespondMethodNotAllowed(int connection)
{
	char buf[BUF_LEN];
	snprintf(buf, sizeof(buf), "HTTP/1.1 405 Method Not Allowed\r\nAllow: GET\r\nConnection: Close\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n");

	SendLine(connection, buf);
}

void RespondOk(int connection)
{
	char buf[BUF_LEN];
	snprintf(buf, sizeof(buf), "HTTP/1.1 200 Ok\r\nConnection: Close\r\nContent-Type:text/plain\r\n\r\nHello, world!\r\n");

	SendLine(connection, buf);
}

void ProcessHttpRequest(int connection)
{
	char *request_line;

	request_line = ReadLine(connection);

	if (strncmp(request_line, "GET", 3) != 0)
	{
		RespondMethodNotAllowed(connection);
	}
	else
	{
		while (strcmp(ReadLine(connection), "") != 0)
		{
			// pass
		}

		RespondOk(connection);
	}

#ifdef WIN32
	if (closesocket(connection) == -1)
#else
	if (close(connection) == -1)
#endif
	{
		perror("Unable to close connection");
	}
}