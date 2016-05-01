#include "stdafx.h"
#include "Rc4.h"
#include <stdlib.h>

Rc4State * Rc4NewState()
{
	return (Rc4State*)calloc(1, sizeof(Rc4State));
}

void Rc4FreeState(Rc4State * state)
{
	free(state);
}

void Rc4Operate(const unsigned char *plaintext,
	int plaintext_len,
	unsigned char *ciphertext,
	const unsigned char *key,
	int key_len,
	Rc4State * state)
{
	int i = state->i;
	int j = state->j;
	unsigned char * S = state->S;

	unsigned char tmp;

	if (S[0] == 0 && S[1] == 0)
	{
		// key scheduling algorithm.
		// ksa initialisation:
		for (i = 0; i < 256; i++)
		{
			S[i] = i;
		}

		// ksa stuff:
		j = 0;
		for (i = 0; i < 256; i++)
		{
			j = (j + S[i] + key[i % key_len]) % 256;
			tmp = S[i];
			S[i] = S[j];
			S[j] = tmp;
		}

		i = 0;
		j = 0;
	}

	while (plaintext_len--)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		tmp = S[i];
		S[i] = S[j];
		S[j] = tmp;
		*(ciphertext++) = S[(S[i] + S[j]) % 256] ^ *(plaintext++);
	}

	state->i = i;
	state->j = j;
}

void Rc4Encrypt40(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	void *state,
	const unsigned char *key)
{
	Rc4Operate(plaintext, plaintext_len, ciphertext, key, 5, (Rc4State*)state);
}

void Rc4Decrypt40(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char plaintext[],
	void *state,
	const unsigned char *key)
{
	Rc4Operate(ciphertext, ciphertext_len, plaintext, key, 5, (Rc4State*)state);
}

void Rc4Encrypt128(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	void *state,
	const unsigned char *key)
{
	Rc4Operate(plaintext, plaintext_len, ciphertext, key, 16, (Rc4State*)state);
}

void Rc4Decrypt128(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char plaintext[],
	void *state,
	const unsigned char *key)
{
	Rc4Operate(ciphertext, ciphertext_len, plaintext, key, 16, (Rc4State*)state);
}
