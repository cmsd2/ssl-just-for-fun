#include "stdafx.h"
#include "Gcd.h"

#pragma once

// Example euclid gcd impl using simple types for clarity
int Gcd(int x, int y)
{
	if (x == 0) { return y; }
	if (y == 0) { return x; }

	if (x > y)
	{
		return Gcd(x - y, y);
	}
	else
	{
		return Gcd(y - x, x);
	}
}

/*
Example impl using simple types for clarity.
Used to calculate multiplicative inverse, modulo a.
z: a prime number do be inverted.
a: the prime modulus
*/
int ExtendedEuclid(int z, int a)
{
	int i, j, y2, y1, y, quotient, remainder;

	i = a;
	j = z;
	y2 = 0;
	y1 = 1;

	while (j > 0)
	{
		quotient = i / j;
		remainder = i % j;

		y = y2 - (y1 * quotient);
		i = j;
		j = remainder;
		y2 = y1;
		y1 = y;
	}

	return (y2 % a);
}