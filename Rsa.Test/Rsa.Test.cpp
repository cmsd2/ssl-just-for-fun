#include "stdafx.h"
#include <Rsa/Rsa.h>
#include <Utils/Hex.h>

const unsigned char TestModulus[] = {
	0xC4, 0xF8, 0xE9, 0xE1, 0x5D, 0xCA, 0xDF, 0x2B,
	0x96, 0xC7, 0x63, 0xD9, 0x81, 0x00, 0x6A, 0x64,
	0x4F, 0xFB, 0x44, 0x15, 0x03, 0x0A, 0x16, 0xED,
	0x12, 0x83, 0x88, 0x33, 0x40, 0xF2, 0xAA, 0x0E,
	0x2B, 0xE2, 0xBE, 0x8F, 0xA6, 0x01, 0x50, 0xB9,
	0x04, 0x69, 0x65, 0x83, 0x7C, 0x3E, 0x7D, 0x15,
	0x1B, 0x7D, 0xE2, 0x37, 0xEB, 0xB9, 0x57, 0xC2,
	0x06, 0x63, 0x89, 0x82, 0x50, 0x70, 0x3B, 0x3F
};

const unsigned char TestPrivateKey[] = {
	0x8a, 0x7e, 0x79, 0xf3, 0xfb, 0xfe, 0xa8, 0xeb,
	0xfd, 0x18, 0x35, 0x1c, 0xb9, 0x97, 0x91, 0x36,
	0xf7, 0x05, 0xb4, 0xd9, 0x11, 0x4a, 0x06, 0xd4,
	0xaa, 0x2f, 0xd1, 0x94, 0x38, 0x16, 0x67, 0x7a,
	0x53, 0x74, 0x66, 0x18, 0x46, 0xa3, 0x0c, 0x45,
	0xb3, 0x0a, 0x02, 0x4b, 0x4d, 0x22, 0xb1, 0x5a,
	0xb3, 0x23, 0x62, 0x2b, 0x2d, 0xe4, 0x7b, 0xa2,
	0x91, 0x15, 0xf0, 0x6e, 0xe4, 0x2c, 0x41
};

const unsigned char TestPublicKey[] = { 0x01, 0x00, 0x01 };

int main(int argc, char *argv[])
{
	int exponent_len;
	int modulus_len;
	int data_len;
	const unsigned char *exponent;
	const unsigned char *modulus;
	unsigned char *data;
	RsaKey public_key;
	RsaKey private_key;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: rsa [-e|-d] [<modulus> <exponent>] <data>\n");
		exit(0);
	}

	if (argc == 5)
	{
		modulus_len = HexDecode(argv[2], (char**)&modulus);
		exponent_len = HexDecode(argv[3], (char**)&exponent);
		data_len = HexDecode(argv[4], (char**)&data);
	}
	else
	{
		data_len = HexDecode(argv[2], (char**)&data);
		modulus_len = sizeof(TestModulus);
		modulus = TestModulus;
		if (!strcmp("-e", argv[1]))
		{
			exponent_len = sizeof(TestPublicKey);
			exponent = TestPublicKey;
		}
		else
		{
			exponent_len = sizeof(TestPrivateKey);
			exponent = TestPrivateKey;
		}
	}

	public_key.modulus = (Huge *)malloc(sizeof(Huge));
	public_key.exponent = (Huge *)malloc(sizeof(Huge));
	private_key.modulus = (Huge *)malloc(sizeof(Huge));
	private_key.exponent = (Huge *)malloc(sizeof(Huge));

	if (!strcmp(argv[1], "-e"))
	{
		unsigned char *encrypted;
		int encrypted_len;

		LoadHuge(public_key.modulus, modulus, modulus_len);
		LoadHuge(public_key.exponent, exponent, exponent_len);

		encrypted_len = RsaEncrypt(data, data_len, &encrypted, &public_key);
		ShowHex((char*)encrypted, encrypted_len);
		free(encrypted);
	}
	else if (!strcmp(argv[1], "-d"))
	{
		int decrypted_len;
		unsigned char *decrypted;

		LoadHuge(private_key.modulus, modulus, modulus_len);
		LoadHuge(private_key.exponent, exponent, exponent_len);

		decrypted_len = RsaDecrypt(data, data_len, &decrypted, &private_key);

		ShowHex((char*)decrypted, decrypted_len);

		free(decrypted);
	}
	else
	{
		fprintf(stderr, "unrecognized option flag '%s'\n", argv[1]);
	}

	free(data);
	if (argc == 5)
	{
		free((void*)modulus);
		free((void*)exponent);
	}
}