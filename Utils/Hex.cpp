#include "stdafx.h"
#include "Hex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
if input starts with "0x" decode remainder as hex byte array
otherwise pass input on unmodified
*/
int HexDecode(const char *input, char **decoded)
{
	unsigned i;
	int len;

	if (strncmp("0x", input, 2))
	{
		len = strlen(input) + 1;
		*decoded = (char*)malloc(len);
		strcpy_s(*decoded, len, input);
		len--;
	}
	else
	{
		len = (strlen(input) >> 1) - 1;
		*decoded = (char*)malloc(len);

		for (i = 2; i < strlen(input); i += 2)
		{
			(*decoded)[((i / 2) - 1)] =
				(((input[i] <= '9') ? input[i] - '0' :
					((tolower(input[i])) - 'a' + 10)) << 4) |
					((input[i + 1] <= '9') ? input[i + 1] - '0' :
						((tolower(input[i + 1])) - 'a' + 10));
		}
	}

	return len;
}

void ShowHex(const char *array, int len)
{
	while (len--)
	{
		printf("%.02hhx", *array++);
	}
	printf("\n");
}