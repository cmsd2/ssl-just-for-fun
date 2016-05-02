// Rsa.Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Rsa/Rsa.h>

int main()
{
	Huge e, d, n, m, c;

	SetHuge(&e, 79);
	SetHuge(&d, 1019);
	SetHuge(&n, 3337);
	SetHuge(&c, 0);

	SetHuge(&m, 688);
	RsaCompute(&m, &e, &n, &c);
	printf("Encrypted to: ");
	PrintHuge(stdout, &c);
	printf("\n");

	SetHuge(&m, 0);
	RsaCompute(&c, &d, &n, &m);
	printf("Decrypted to: ");
	PrintHuge(stdout, &m);
	printf("\n");

    return 0;
}

