#include "stdafx.h"
#include <Huge/Huge.h>

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
	Power(m, e, c);

	Divide(c, n, NULL);
}