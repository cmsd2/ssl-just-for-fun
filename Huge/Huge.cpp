#include "stdafx.h"
#include "Huge.h"
#include <Utils/Bits.h>

void FreeHuge(Huge * h)
{
	if (h->rep)
	{
		free(h->rep);
		h->size = 0;
	}
}

void Add(Huge *h1, Huge *h2)
{
	unsigned int i, j, sum;
	unsigned int carry = 0;

	// resize if necessary
	if (h2->size > h1->size)
	{
		unsigned char *tmp = h1->rep;
		h1->rep = (unsigned char *)realloc(h1->rep, h2->size * sizeof(unsigned char));
		assert(h1->rep);
		memmove(h1->rep + (h2->size - h1->size), tmp, h1->size);
		memset(h1->rep, 0, (h2->size - h1->size));
		h1->size = h2->size;
	}

	i = h1->size;
	j = h2->size;

	// invariant: i >= j

	do
	{
		i--;
		if (j)
		{
			j--;
			sum = h1->rep[i] + h2->rep[j] + carry;
		}
		else
		{
			sum = h1->rep[i] + carry;
		}

		carry = sum > 0xFF;
		h1->rep[i] = sum;

	} while (i);

	if (carry)
	{
		Expand(h1);
		h1->rep[0] = 1;
	}
}

void Expand(Huge * h)
{
	unsigned int increase = 1;
	unsigned char *tmp = h->rep;
	h->rep = (unsigned char*)realloc(h->rep, (h->size + increase) * sizeof(unsigned char));
	assert(h->rep);
	memmove(h->rep + increase, tmp, h->size * sizeof(unsigned char));
	h->size += increase;
	h->rep[0] = 1;
}

// compact leading 0 bytes
void Compact(Huge * h)
{
	unsigned int i = 0;

	while (!(h->rep[i]) && (i < h->size)) { i++; }

	if (i)
	{
		if (i == h->size)
		{
			Contract(h, 1);
		}
		else
		{
			Contract(h, h->size - i);
		}
	}
}

void Contract(Huge * h, unsigned int new_size)
{
	assert(new_size <= h->size);
	int decrease = h->size - new_size;
	unsigned char * tmp = &h->rep[decrease];
	h->rep = (unsigned char*)calloc(new_size, sizeof(unsigned char));
	assert(h->rep);
	memcpy(h->rep, tmp, new_size);
	h->size = new_size;
}

void Subtract(Huge *h1, Huge *h2)
{
	int i = h1->size;
	int j = h2->size;
	int diff;
	unsigned int borrow = 0;

	do
	{
		i--;

		if (j)
		{
			j--;
			diff = h1->rep[i] - h2->rep[j] - borrow;
		}
		else
		{
			diff = h1->rep[i] - borrow;
		}

		borrow = (diff < 0);
		h1->rep[i] = diff;
		
	} while (i);

	if (borrow && i)
	{
		if (!(h1->rep[i - 1])) // don't borrow i
		{
			fprintf(stderr, "Error, subtraction result is negative\n");
			assert(0);
		}
		h1->rep[i - 1]--;
	}

	Compact(h1);
}

void Multiply(Huge * h1, Huge * h2)
{
	unsigned char mask;
	unsigned int i;
	Huge temp;

	SetHuge(&temp, 0);
	CopyHuge(&temp, h1);

	SetHuge(h1, 0);

	i = h2->size;
	do
	{
		i--;
		for (mask = 1; mask; mask <<= 1)
		{
			if (mask & h2->rep[i])
			{
				Add(h1, &temp);
			}

			LeftShift(&temp);
		}
	} while (i);
}

void CopyHuge(Huge *tgt, Huge *src)
{
	FreeHuge(tgt);

	tgt->size = src->size;
	tgt->rep = (unsigned char*)calloc(src->size, sizeof(unsigned char));
	memcpy(tgt->rep, src->rep, (src->size * sizeof(unsigned char)));
}

void SetHuge(Huge * h, unsigned int val)
{
	unsigned int mask, i, shift;

	h->size = 4;

	for (mask = 0xFF000000; mask > 0xFF; mask >>= 8)
	{
		if (val & mask)
		{
			break;
		}

		h->size--;
	}

	h->rep = (unsigned char*)malloc(h->size);

	mask = 0xFF;
	shift = 0;
	for (i = h->size; i; i--)
	{
		h->rep[i - 1] = (val & mask) >> shift;
		mask <<= 8;
		shift += 8;
	}
}

void LeftShift(Huge *h1)
{
	int i;
	int old_carry;
	int carry = 0;

	i = h1->size;
	do
	{
		i--;
		old_carry = carry;
		carry = (h1->rep[i] & 0x80) == 0x80;
		h1->rep[i] = (h1->rep[i] << 1) | old_carry;
	} while (i);

	if (carry)
	{
		Expand(h1);
	}
}

void Divide(Huge *dividend, Huge *divisor, Huge *quotient)
{
	int bit_size;
	int bit_position; // bit in the quotient being operated on

	bit_size = bit_position = 0;

	while (Compare(divisor, dividend) < 0)
	{
		LeftShift(divisor);
		bit_size++;
	}

	if (quotient)
	{
		// may overestimate
		quotient->size = (bit_size / 8) + 1;
		quotient->rep = (unsigned char*)calloc(quotient->size, sizeof(unsigned char));
		memset(quotient->rep, 0, quotient->size);
	}

	bit_position = 8 - (bit_size % 8) - 1;

	do
	{
		if (Compare(divisor, dividend) <= 0)
		{
			Subtract(dividend, divisor); // dividend -= divisor

			if (quotient)
			{
				SET_BIT(quotient->rep, bit_position);
			}
		}

		if (bit_size)
		{
			RightShift(divisor);
		}

		bit_position++;

	} while (bit_size--);
}

int Compare(Huge *h1, Huge *h2)
{
	unsigned int i, j;

	if (h1->size > h2->size)
	{
		return 1;
	}

	if (h1->size < h2->size)
	{
		return -1;
	}

	i = j = 0;

	while (i < h1->size && j < h2->size)
	{
		if (h1->rep[i] < h2->rep[j])
		{
			return -1;
		}
		else if (h1->rep[i] > h2->rep[j])
		{
			return 1;
		}
		
		i++;
		j++;
	}

	return 0;
}

void RightShift(Huge *h1)
{
	unsigned int i;
	unsigned int old_carry;
	unsigned int carry = 0;

	i = 0;
	do
	{
		old_carry = carry;
		carry = (h1->rep[i] & 0x1) << 7;
		h1->rep[i] = (h1->rep[i] >> 1) | old_carry;
	} while (++i < h1->size);

	Compact(h1);
}

// TODO print in base 10 instead of base 256
void PrintHuge(FILE *f, Huge *h)
{
	if (h->rep)
	{
		for (int i = 0; i < h->size; i++)
		{
			fprintf(f, "%hhu ", h->rep[i]);
		}
	}
	else
	{
		fprintf(f, "NULL");
	}
}

void Power(Huge * mantissa, Huge * exponent, Huge * result)
{
	Huge counter;
	Huge one;

	SetHuge(result, 1);
	SetHuge(&counter, 1);
	SetHuge(&one, 1);
	while (Compare(&counter, exponent) <= 0)
	{
		//PrintHuge(stdout, &counter); printf("\n");
		//PrintHuge(stdout, result); printf("\n\n");
		Multiply(result, mantissa);
		Add(&counter, &one);
	}
	//PrintHuge(stdout, &counter); printf("\n");
	//PrintHuge(stdout, result); printf("\n\n");

	FreeHuge(&counter);
	FreeHuge(&one);
}