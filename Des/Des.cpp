#include <string.h>
#include "stdafx.h"
#include "Utils/Bits.h"
#include "Des.h"

static const int IpTable[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

static const int FinalTable[] = {
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

static const int Pc1Table[] = {
	57, 49, 41, 33, 25, 17,  9,  1,
	58, 50, 42, 34, 26, 18, 10,  2,
	59, 51, 43, 35, 27, 19, 11,  3,
	60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7,
	62, 54, 46, 38, 30, 22, 14,  6,
	61, 53, 45, 37, 29, 21, 13,  5,
	28, 20, 12,  4
};

static const int Pc2Table[] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

static const int ExpansionTable[] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

static const int SBoxTables[8][64] = {
	{ 14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1,
	3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8,
	4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7,
	15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13 },
	{ 15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14,
	9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5,
	0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2,
	5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9 },
	{ 10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10,
	1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1,
	13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7,
	11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12 },
	{ 7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3,
	1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9,
	10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8,
	15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14 },
	{ 2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1,
	8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6,
	4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13,
	15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3 },
	{ 12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5,
	0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8,
	9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10,
	7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13 },
	{ 4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10,
	3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6,
	1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7,
	10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12 },
	{ 13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4,
	10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2,
	7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13,
	0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11 }
};

static const int PTable[] = { 
	16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25 
};

/*
Initial and final DES permutations.
permute_table must have len entries
target must have len * 8 entries
src will be shorter
permute_table is a 1-indexed array.
*/
void Permute(unsigned char target[],
	const unsigned char src[],
	const int permute_table[],
	int len)
{
	for (int i = 0; i < len * 8; i++)
	{
		if (GET_BIT(src, (permute_table[i] - 1)))
		{
			SET_BIT(target, i);
		}
		else
		{
			CLEAR_BIT(target, i);
		}
	}
}

/*
56 bit left key rotation.

rotate the two 28-bit halves of the key separately
*/
void RotateLeft(unsigned char *target)
{
	int carry_left, carry_right;

	carry_left = (target[0] & 0x80) >> 3;

	target[0] = (target[0] << 1) | ((target[1] & 0x80) >> 7);
	target[1] = (target[1] << 1) | ((target[2] & 0x80) >> 7);
	target[2] = (target[2] << 1) | ((target[3] & 0x80) >> 7);

	carry_right = (target[3] & 0x08) >> 3;
	target[3] = (((target[3] << 1) | ((target[4] & 0x80) >> 7)) & ~0x10) | carry_left;

	target[4] = (target[4] << 1) | ((target[5] & 0x80) >> 7);
	target[5] = (target[5] << 1) | ((target[6] & 0x80) >> 7);
	target[6] = (target[6] << 1) | carry_right;
}

void RotateRight(unsigned char *target)
{
	int carry_left, carry_right;

	carry_right = (target[6] & 0x01) << 3;

	target[6] = (target[6] >> 1) | ((target[5] & 0x01) << 7);
	target[5] = (target[5] >> 1) | ((target[4] & 0x01) << 7);
	target[4] = (target[4] >> 1) | ((target[3] & 0x01) << 7);

	carry_left = (target[3] & 0x10) << 3;
	target[3] = (((target[3] >> 1) | ((target[2] & 0x01) << 7)) & ~0x08) | carry_right;

	target[2] = (target[2] >> 1) | ((target[1] & 0x01) << 7);
	target[1] = (target[1] >> 1) | ((target[0] & 0x01) << 7);
	target[0] = (target[0] >> 1) | carry_left;
}

#define DES_BLOCK_SIZE 8
#define DES_KEY_SIZE 8
#define EXPANSION_BLOCK_SIZE 6
#define PC1_KEY_SIZE 7
#define SUBKEY_SIZE 6

void DesBlockOperate(const unsigned char plain_text[DES_BLOCK_SIZE],
	unsigned char cipher_text[DES_BLOCK_SIZE],
	const unsigned char key[DES_KEY_SIZE],
	OpType op_type)
{
	unsigned char ip_block[DES_BLOCK_SIZE];
	unsigned char expansion_block[EXPANSION_BLOCK_SIZE];
	unsigned char substitution_block[DES_BLOCK_SIZE / 2];
	unsigned char pbox_target[DES_BLOCK_SIZE / 2];
	unsigned char recomb_box[DES_BLOCK_SIZE / 2];

	unsigned char pc1key[PC1_KEY_SIZE];
	unsigned char subkey[SUBKEY_SIZE];
	int round;

	// initial permutation
	Permute(ip_block, plain_text, IpTable, DES_BLOCK_SIZE);

	// key schedule computation
	Permute(pc1key, key, Pc1Table, PC1_KEY_SIZE);

	for (round = 0; round < 16; round++)
	{
		// feistel function on the first half of the block in ip_block
		
		// expansion.
		Permute(expansion_block, ip_block + 4, ExpansionTable, 6);

		// key mixing
		if (op_type == OP_ENCRYPT)
		{
			// rotate twice ...
			RotateLeft(pc1key);
			// ... except in rounds 1, 2, 9 and 16
			if (!(round <= 1 || round == 8 || round == 15))
			{
				RotateLeft(pc1key);
			}
		}

		Permute(subkey, pc1key, Pc2Table, SUBKEY_SIZE);

		if (op_type == OP_DECRYPT)
		{
			// rotate twice...
			RotateRight(pc1key);
			// ...except in rounds 1, 2, 9 and 16
			if( !(round >= 14 || round == 7 || round == 0))
			{
				RotateRight(pc1key);
			}
		}

		xor (expansion_block, subkey, 6);

		// substitution.
		memset((void*)substitution_block, 0, DES_BLOCK_SIZE / 2);
		substitution_block[0] = SBoxTables[0][(expansion_block[0] & 0xFC) >> 2] << 4;
		substitution_block[0] |= SBoxTables[1][(expansion_block[0] & 0x03) << 4 | 
			(expansion_block[1] & 0xF0) >> 4];
		substitution_block[1] = SBoxTables[2][(expansion_block[1] & 0x0F) << 2 |
			(expansion_block[2] & 0xC0) >> 6] << 4;
		substitution_block[1] |= SBoxTables[3][(expansion_block[2] & 0x3F)];
		substitution_block[2] = SBoxTables[4][(expansion_block[3] & 0xFC) >> 2] << 4;
		substitution_block[2] |= SBoxTables[5][(expansion_block[3] & 0x03) << 4 |
			(expansion_block[4] & 0xF0) >> 4];
		substitution_block[3] = SBoxTables[6][(expansion_block[4] & 0x0F) << 2 |
			(expansion_block[5] & 0xC0) >> 6] << 4;
		substitution_block[3] |=
			SBoxTables[7][(expansion_block[5] & 0x3F)];

		// permutation
		Permute(pbox_target, substitution_block, PTable, DES_BLOCK_SIZE / 2);

		// recombination
		// xor the pbox with the left half and then switch sides
		memcpy((void*)recomb_box, (void*)ip_block, DES_BLOCK_SIZE / 2);
		memcpy((void*)ip_block, (void*)(ip_block + 4), DES_BLOCK_SIZE / 2);
		xor (recomb_box, pbox_target, DES_BLOCK_SIZE / 2);
		memcpy((void*)(ip_block + 4), (void*)recomb_box, DES_BLOCK_SIZE / 2);
	}

	// swap one last time
	memcpy((void*)recomb_box, (void*)ip_block, DES_BLOCK_SIZE / 2);
	memcpy((void*)ip_block, (void*)(ip_block + 4), DES_BLOCK_SIZE / 2);
	memcpy((void*)(ip_block + 4), (void*)recomb_box, DES_BLOCK_SIZE / 2);

	// final permutation (undo initial permuation)
	Permute(cipher_text, ip_block, FinalTable, DES_BLOCK_SIZE);
}

void DesOperate(const unsigned char *input,
	int input_len,
	unsigned char *output,
	const unsigned char *iv,
	const unsigned char *key,
	OpType op_type,
	int triplicate)
{
	unsigned char input_block[DES_BLOCK_SIZE];

	assert(0 == (input_len % DES_BLOCK_SIZE));

	while (input_len)
	{
		memcpy((void*)input_block, (void*)input, DES_BLOCK_SIZE);
		if (op_type == OP_ENCRYPT)
		{
			xor (input_block, iv, DES_BLOCK_SIZE); // implement CBC
			DesBlockOperate(input_block, output, key, op_type);
			
			if (triplicate)
			{
				memcpy(input_block, output, DES_BLOCK_SIZE);
				DesBlockOperate(input_block, output, key + DES_KEY_SIZE, OP_DECRYPT);

				memcpy(input_block, output, DES_BLOCK_SIZE);
				DesBlockOperate(input_block, output, key + (DES_KEY_SIZE * 2), op_type);
			}

			memcpy((void*)iv, (void*)output, DES_BLOCK_SIZE); // CBC
		}
		else if (op_type == OP_DECRYPT)
		{
			if (triplicate)
			{
				DesBlockOperate(input_block, output, key + (DES_KEY_SIZE * 2), op_type);
				memcpy(input_block, output, DES_BLOCK_SIZE);

				DesBlockOperate(input_block, output, key + DES_KEY_SIZE, OP_ENCRYPT);
				memcpy(input_block, output, DES_BLOCK_SIZE);
			}

			DesBlockOperate(input_block, output, key, op_type);
			xor (output, iv, DES_BLOCK_SIZE);
			memcpy((void*)iv, (void*)input, DES_BLOCK_SIZE); // CBC
		}

		input += DES_BLOCK_SIZE;
		output += DES_BLOCK_SIZE;
		input_len -= DES_BLOCK_SIZE;
	}
}

void DesEncrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char *ciphertext,
	void *iv,
	const unsigned char *key,
	PaddingType padding_type)
{
	int padded_plaintext_len = plaintext_len;

	unsigned char * padded_plaintext = DesPadPlaintext(plaintext, &padded_plaintext_len, padding_type);

	DesOperate(padded_plaintext, padded_plaintext_len, ciphertext, (unsigned char*)iv, key, OP_ENCRYPT, 0);

	free(padded_plaintext);
}

void DesDecrypt(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char *plaintext,
	void *iv,
	const unsigned char *key)
{
	DesOperate(ciphertext, ciphertext_len, plaintext, (unsigned char*)iv, key, OP_DECRYPT, 0);

	// remove any padding. SSL takes care of padding.

	//plaintext[ciphertext_len - plaintext[ciphertext_len - 1]] = 0x0;
}

unsigned char * DesPadPlaintext(const unsigned char * plaintext, int * plaintext_len, PaddingType padding_type)
{
	unsigned char *padded_plaintext;
	int padding_len;

	// pad to multiple of DES_BLOCK_SIZE
	padding_len = DES_BLOCK_SIZE - (*plaintext_len % DES_BLOCK_SIZE);
	padded_plaintext = (unsigned char *)malloc(*plaintext_len + padding_len);

	if (padding_type == PAD_NIST_800_3A)
	{
		memset(padded_plaintext, 0x0, *plaintext_len + padding_len);
		padded_plaintext[*plaintext_len] = 0x80;
	}
	else if (padding_type == PAD_PKCS_5)
	{
		memset(padded_plaintext, padding_len, *plaintext_len + padding_len);
	}

	memcpy(padded_plaintext, plaintext, *plaintext_len);

	*plaintext_len += padding_len;

	return padded_plaintext;
}

void Des3Encrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char *ciphertext,
	void *iv,
	const unsigned char *key,
	PaddingType padding_type)
{
	int padded_plaintext_len = plaintext_len;

	unsigned char * padded_plaintext = DesPadPlaintext(plaintext, &padded_plaintext_len, padding_type);

	DesOperate(padded_plaintext, padded_plaintext_len, ciphertext, (unsigned char*)iv, key, OP_ENCRYPT, 1);

	free(padded_plaintext);
}

void Des3Decrypt(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char *plaintext,
	void *iv,
	const unsigned char *key)
{
	DesOperate(ciphertext, ciphertext_len, plaintext, (unsigned char*)iv, key, OP_DECRYPT, 1);
}