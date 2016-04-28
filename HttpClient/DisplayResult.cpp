#include "stdafx.h"

#define BUFFER_SIZE 255

void DisplayResult(int connection)
{
	int received = 0;

	static char recv_buf[BUFFER_SIZE + 1];

	while ((received = recv(connection, recv_buf, BUFFER_SIZE, 0)) > 0)
	{
		recv_buf[received] = '\0';
		printf("%s", recv_buf);
	}

	printf("\n");
}