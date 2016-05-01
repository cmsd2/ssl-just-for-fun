#pragma once

#define BASE64_ENCODED_LEN(len) (((len + 2) / 3) * 4)
#define BASE64_DECODED_LEN(len) (3 * (len / 4))

void Base64Encode(const unsigned char *input, int len, unsigned char *output);

int Base64Decode(const unsigned char *input, int len, unsigned char *output);