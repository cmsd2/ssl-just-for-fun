#include "stdafx.h"

int ParseUrl(char *uri, char **host, char **path)
{
	char *pos;

	pos = strstr(uri, "//");

	if (!pos) 
	{
		return -1;
	}

	*host = pos + 2;

	pos = strchr(*host, '/');

	if (!pos) 
	{
		*path = NULL;
	}
	else
	{
		*pos = '\0';
		*path = pos + 1;
	}

	return 0;
}