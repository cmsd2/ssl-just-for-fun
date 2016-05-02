#include "stdafx.h"
#include <Huge/Huge.h>
#include "Rsa.h"

/*
((m ^ e) ^ d) mod n == m

m: message
e: public key
d: private key
n: public prime number (>= 1024 bits long)
c: ciphertext

encryption: c := (m ^ e) mod n
decryption: m == (c ^ d) mod n
*/
void RsaCompute(Huge *m, Huge *e, Huge *n, Huge *c)
{
	ModPower(m, e, n, c);
}

/*
Input is broken into n-bit blocks where n is length in bits of the modulus.
Output is n-bits or less.
RFC2313 mandates >= 8 bytes of padding to prevent attacker brute forcing padding.
RSA pads at the start.
output will be malloced with enough space for result. caller frees.
 */
int RsaEncrypt(unsigned char *input,
	unsigned int len,
	unsigned char **output,
	RsaKey *public_key)
{
	unsigned int i;
	Huge c, m;

	unsigned int modulus_length = public_key->modulus->size;
	int block_size;
	unsigned char *padded_block = (unsigned char*)malloc(modulus_length);
	int encrypted_size = 0;

	*output = NULL; // for realloc

	while (len)
	{
		encrypted_size += modulus_length;
		block_size = (len < modulus_length - 11) ? len : (modulus_length - 11);

		// init block
		memset(padded_block, 0, modulus_length);

		// paste data, aligning with the end of the block
		memcpy(padded_block + (modulus_length - block_size), input, block_size);

		// padded_block[0] = 0; // header. redundant due to memset above.
		padded_block[1] = RSA_PAD_TYPE_PKCS_1_5;

		// fill padding bytes with non-zeroes at beginning of the block,
		// leaving a single zero before the data.
		for (i = 2; i < (modulus_length - block_size - 1); i++)
		{
			// TODO make random
			padded_block[i] = i;
		}

		LoadHuge(&m, padded_block, modulus_length);
		ModPower(&m, public_key->exponent, public_key->modulus, &c);

		*output = (unsigned char*)realloc(*output, encrypted_size);

		// encrypted_size was already adjusted for this block
		UnloadHuge(&c, *output + (encrypted_size - modulus_length), modulus_length);

		len -= block_size;
		input += block_size;

		FreeHuge(&m);
		FreeHuge(&c);
	}

	free(padded_block);

	return encrypted_size;
}

/*
convert input into key-length blocks and decrypt, removing padding.
return -1 if the input is not a multiple of the key modulus length,
or if the padding type is not 2.
otherwise return the length of the decrypted data.
*/
int RsaDecrypt(unsigned char *input,
	unsigned int len,
	unsigned char **output,
	RsaKey *private_key)
{
	unsigned int i;
	int out_len = 0;
	Huge c, m;
	unsigned int modulus_length = private_key->modulus->size;
	unsigned char *padded_block = (unsigned char*)malloc(modulus_length);

	*output = NULL; // for realloc

	while (len)
	{
		if (len < modulus_length)
		{
			fprintf(stderr, "Error - input must be an even multiple of key modulus %d (got %d)\n",
				private_key->modulus->size, len);
			free(padded_block);
			return -1;
		}

		LoadHuge(&c, input, modulus_length);
		ModPower(&c, private_key->exponent, private_key->modulus, &m);

		UnloadHuge(&m, padded_block, modulus_length);

		if (padded_block[1] != RSA_PAD_TYPE_PKCS_1_5)
		{
			fprintf(stderr, "Decryption error or unrecognised block type %hhd.\n", padded_block[1]);
			FreeHuge(&c);
			FreeHuge(&m);
			free(padded_block);
			return -1;
		}

		// skip padding: look for next 0 byte.
		for (i = 2; i < modulus_length; i++)
		{
			if (padded_block[i] == 0)
			{
				break;
			}
		}

		if (i == modulus_length)
		{
			fprintf(stderr, "Decryption error while looking for end of padding.\n");
			FreeHuge(&c);
			FreeHuge(&m);
			free(padded_block);
			return -1;
		}

		i++;

		// now padded_block+i is the start of data and i <= modulus_length.

		out_len += modulus_length - i;

		*output = (unsigned char *)realloc(*output, out_len);
		memcpy(*output + (out_len - (modulus_length - i)), padded_block + i, modulus_length - i);

		len -= modulus_length;
		input += modulus_length;
		
		FreeHuge(&c);
		FreeHuge(&m);
	}

	free(padded_block);

	return out_len;
}