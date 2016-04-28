#include "stdafx.h"

#define MAX_GET_COMMAND 255

int HttpSend(int connection, const char * line)
{
	if (send(connection, line, strlen(line), 0) == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int HttpPrintf(int connection, const char * fmt, ...)
{
	va_list args;

	char buf[MAX_GET_COMMAND + 1];

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	return HttpSend(connection, buf);
}

int HttpGet(int connection, const char *path, const char *host)
{
	return HttpPrintf(connection, "GET /%s HTTP/1.1\r\n", path) ||
	HttpPrintf(connection, "Host: %s\r\n", host) ||
	HttpPrintf(connection, "Connection: close\r\n\r\n");
}