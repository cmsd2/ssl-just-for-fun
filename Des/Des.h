#pragma once

typedef enum OpTypeTag { 
	OP_ENCRYPT, 
	OP_DECRYPT 
} OpType;

typedef enum PaddingTypeTag {
	PAD_NIST_800_3A,
	PAD_PKCS_5
} PaddingType;

void DesEncrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char *ciphertext,
	const unsigned char *iv,
	const unsigned char *key,
	PaddingType padding_type);

void DesDecrypt(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char *plaintext,
	const unsigned char *iv,
	const unsigned char *key);

void Des3Encrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char *ciphertext,
	const unsigned char *iv,
	const unsigned char *key,
	PaddingType padding_type);

void Des3Decrypt(const unsigned char *ciphertext,
	const int ciphertext_len,
	unsigned char *plaintext,
	const unsigned char *iv,
	const unsigned char *key);

unsigned char * DesPadPlaintext(const unsigned char * plaintext, int * plaintext_len, PaddingType padding_type);