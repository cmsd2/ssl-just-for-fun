#pragma once

#define RC4_STATE_ARRAY_LEN 256

typedef struct Rc4StateTag
{
	int i;
	int j;
	unsigned char S[RC4_STATE_ARRAY_LEN];
} Rc4State;

Rc4State * Rc4NewState();
void Rc4FreeState(Rc4State * state);

void Rc4Operate(const unsigned char *plaintext,
	int plaintext_len,
	unsigned char *ciphertext,
	const unsigned char *key,
	int key_len,
	Rc4State * state);

void Rc4Encrypt40(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	void *state,
	const unsigned char *key);

void Rc4Decrypt40(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char plaintext[],
	void *state,
	const unsigned char *key);

void Rc4Encrypt128(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	void *state,
	const unsigned char *key);

void Rc4Decrypt128(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char plaintext[],
	void *state,
	const unsigned char *key);