// Des.Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <Aes/Aes.h>
#include <Utils/Hex.h>
#include <Utils/Base64.h>

int main(int argc, char **argv)
{
	unsigned char *key;
	unsigned char *iv;
	unsigned char *input;
	unsigned char *output;

	int key_len;
	int out_len;
	int input_len;
	int iv_len;

	if (argc < 5)
	{
		fprintf(stderr, "Usage %s [-e|-d] <key> <iv> <input>\n", argv[0]);
		return 1;
	}

	key_len = HexDecode(argv[2], (char**)&key);
	iv_len = HexDecode(argv[3], (char**)&iv);
	input_len = HexDecode(argv[4], (char**)&input);

	out_len = input_len;
	output = (unsigned char*)malloc(out_len + 1);

	if (!(strcmp(argv[1], "-e")))
	{
		if (key_len == 16)
		{
			Aes128Encrypt(input, input_len, output, iv, key);
		}
		else if(key_len == 32)
		{
			Aes256Encrypt(input, input_len, output, iv, key);
		}
		else
		{
			fprintf(stderr, "Unsupported key length: %d\n", key_len);
			return 1;
		}

		ShowHex((char*)output, out_len);
	}
	else if (!(strcmp(argv[1], "-d")))
	{
		if (key_len == 16)
		{
			Aes128Decrypt(input, input_len, output, iv, key);
		}
		else if(key_len == 32)
		{
			Aes256Decrypt(input, input_len, output, iv, key);
		}
		else
		{
			fprintf(stderr, "Unsupported key length: %d\n", key_len);
			return 1;
		}

		ShowHex((char*)output, out_len);
	}
	else
	{
		fprintf(stderr, "Usage %s [-e|-d] <key> <iv> <input>\n", argv[0]);
		return 1;
	}

	return 0;
}

