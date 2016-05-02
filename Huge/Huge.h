#pragma once

#include <stdlib.h>

/*
array of bytes of length size.
numeric value of Huge is b0 + 256*b1 + 256*256*b2 + ...
memory layout is MSB, i.e. as you would write the number on paper.
*/
typedef struct HugeTag 
{
	unsigned int size;
	unsigned char *rep;
} Huge;

void FreeHuge(Huge * h);

void CopyHuge(Huge *tgt, Huge *src);

void Add(Huge *h1, Huge *h2);

void Subtract(Huge *h1, Huge *h2);

void Expand(Huge *h);

void Contract(Huge * h, unsigned int new_size);

void Multiply(Huge * h1, Huge * h2);

void SetHuge(Huge * h, unsigned int val);

void LeftShift(Huge *h1);

void RightShift(Huge *h1);

// dividend is numerator
// divisor is denominator
// remainder will be returned in dividend
// divisor will be mutated but will end where it started
// quotient will be the result
void Divide(/* inout */ Huge *dividend, /* in */ Huge *divisor, /* out */ Huge *quotient);

int Compare(Huge *h1, Huge *h2);

void PrintHuge(FILE *f, Huge *h);

void Power(Huge * mantissa, Huge * exponent, Huge * result);