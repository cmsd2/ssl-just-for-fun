// Rc4.Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Utils/Hex.h>
#include <Rc4/Rc4.h>

int main(int argc, char **argv)
{
	unsigned char *key;
	unsigned char *input;
	unsigned char *output;
	int key_len;
	int input_len;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <key> <input>\n", argv[0]);
		return 1;
	}

	key_len = HexDecode(argv[1], (char**)&key);
	input_len = HexDecode(argv[2], (char**)&input);

	output = (unsigned char *)malloc(input_len);

	Rc4State * state = Rc4NewState();
	Rc4Operate(input, input_len, output, key, key_len, state);

	ShowHex((char*)output, input_len);

    return 0;
}

