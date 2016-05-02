// Ecc.Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Ecc/EccInt.h>

using namespace EccInt;

int main()
{
	/*
	Diffie-Hellman key exchange using ECC with small values as an example
	*/
	DomainParameters T;
	KeyPair A;
	KeyPair B;
	Point Z1, Z2;

	// Alice and Bob mutually agree domain parameters
	T.p = 23;
	T.a = 1;
	T.b = 1;
	T.G.x = 5;
	T.G.y = 19;

	A.private_key = 4;
	B.private_key = 2;

	A.public_key.x = T.G.x;
	A.public_key.y = T.G.y;
	MultiplyPoint(&A.public_key, A.private_key, T.a, T.p);
	// A.public_key now == (13, 16)
	// Alice sends this to Bob

	B.public_key.x = T.G.x;
	B.public_key.y = T.G.y;
	MultiplyPoint(&B.public_key, B.private_key, T.a, T.p);
	// B.public_key now == (17, 3)
	// Bob sends this to Alice

	Z1.x = A.public_key.x;
	Z1.y = A.public_key.y;
	MultiplyPoint(&Z1, B.private_key, T.a, T.p);
	// Bob calculates Z1 for the shared key

	Z2.x = B.public_key.x;
	Z2.y = B.public_key.y;
	MultiplyPoint(&Z2, A.private_key, T.a, T.p);
	// Alice calculates Z2 for the shared key
	
	// and since Z1 == Z2 they have agreed a shared key.

    return 0;
}

