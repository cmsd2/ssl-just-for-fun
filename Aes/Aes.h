#pragma once

#define AES_BLOCK_SIZE 16

void AesBlockEncrypt(const unsigned char * input_block,
	unsigned char * output_block,
	const unsigned char * key,
	int key_size);

void AesBlockDecrypt(const unsigned char * input_block,
	unsigned char * output_block,
	const unsigned char * key,
	int key_size);

void AesEncrypt(const unsigned char *input,
	int input_len,
	unsigned char * output,
	const unsigned char *iv,
	const unsigned char *key,
	int key_length);

void Aes128Encrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	const unsigned char *iv,
	const unsigned char *key);

void Aes128Decrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	const unsigned char *iv,
	const unsigned char *key);

void Aes256Encrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	const unsigned char *iv,
	const unsigned char *key);

void Aes256Decrypt(const unsigned char *plaintext,
	const int plaintext_len,
	unsigned char ciphertext[],
	const unsigned char *iv,
	const unsigned char *key);
