#pragma once

#include <Huge/Huge.h>

#define RSA_PAD_TYPE_PKCS_1_5 2

typedef struct RsaKeyTag
{
	Huge * modulus;
	Huge * exponent;
} RsaKey;

void RsaCompute(Huge *m, Huge *e, Huge *n, Huge *c);

int RsaEncrypt(unsigned char *input,
	unsigned int len,
	unsigned char **output,
	RsaKey *public_key);

int RsaDecrypt(unsigned char *input,
	unsigned int len,
	unsigned char **output,
	RsaKey *private_key);
