#include "stdafx.h"
#include "ReadLine.h"
#include <SslLib\Buffer.h>

#define DEFAULT_LINE_LEN 255

char * ReadLine(int connection)
{
	static Buffer buffer;

	static int line_len = DEFAULT_LINE_LEN;
	static char *line = NULL;

	char c;
	int pos = 0;

	if (!line)
	{
		line = (char*)malloc(line_len);
	}

	while (buffer.recv(connection) > 0)
	{
		buffer.flip();

		while ((c = buffer.getc()) > 0)
		{
			if ((c == '\n') && pos > 0 && (line[pos - 1] == '\r'))
			{
				line[pos - 1] = '\0';

				buffer.compact();

				return line;
			}

			line[pos++] = c;

			if (pos > line_len)
			{
				line_len *= 2;
				line = (char *)realloc(line, line_len);
				if (!line)
				{
					return NULL;
				}
			}
		}
	}

	buffer.clear();
}