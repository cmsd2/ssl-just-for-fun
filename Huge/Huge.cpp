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
	int result_sign;

	if (Compare(h1, h2) > 0)
	{
		result_sign = h1->sign;

		if (h1->sign == h2->sign)
		{
			AddMagnitude(h1, h2);
		}
		else 
		{
			SubtractMagnitude(h1, h2);
		}
	}
	else
	{
		Huge tmp;

		SetHuge(&tmp, 0);
		CopyHuge(&tmp, h1);
		CopyHuge(h1, h2);

		if (h1->sign == tmp.sign)
		{
			result_sign = h1->sign;
			AddMagnitude(h1, &tmp);
		}
		else
		{
			result_sign = h2->sign;
			SubtractMagnitude(h1, &tmp);
		}

		FreeHuge(&tmp);
	}

	h1->sign = result_sign;
}

void Subtract(Huge *h1, Huge * h2)
{
	int result_sign;

	if (Compare(h1, h2) > 0)
	{
		result_sign = h1->sign;

		if (h1->sign == h2->sign)
		{
			SubtractMagnitude(h1, h2);
		}
		else
		{
			AddMagnitude(h1, h2);
		}
	}
	else
	{
		Huge tmp;

		SetHuge(&tmp, 0);
		CopyHuge(&tmp, h1);
		CopyHuge(h1, h2);

		if (h1->sign == tmp.sign)
		{
			result_sign = !(h1->sign);
			SubtractMagnitude(h1, &tmp);
		}
		else
		{
			result_sign = !(h2->sign);
			AddMagnitude(h1, &tmp);
		}

		FreeHuge(&tmp);
	}

	h1->sign = result_sign;
}

void AddMagnitude(Huge *h1, Huge *h2)
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

void SubtractMagnitude(Huge *h1, Huge *h2)
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
		if (h1->rep[i - 1])
		{
			h1->rep[i - 1]--;
		}
	}

	Compact(h1);
}

void Multiply(Huge * h1, Huge * h2)
{
	unsigned char mask;
	unsigned int i;
	int result_sign;
	Huge temp;

	SetHuge(&temp, 0);
	CopyHuge(&temp, h1);

	result_sign = !(h1->sign == h2->sign);

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

	h1->sign = result_sign;
}

void CopyHuge(Huge *tgt, Huge *src)
{
	FreeHuge(tgt);

	tgt->sign = src->sign;
	tgt->size = src->size;
	tgt->rep = (unsigned char*)calloc(src->size, sizeof(unsigned char));
	memcpy(tgt->rep, src->rep, (src->size * sizeof(unsigned char)));
}

void SetHuge(Huge * h, unsigned int val)
{
	unsigned int mask, i, shift;

	h->sign = 0;
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
		quotient->sign = !(divisor->sign == dividend->sign);
		quotient->size = (bit_size / 8) + 1;
		quotient->rep = (unsigned char*)calloc(quotient->size, sizeof(unsigned char));
		memset(quotient->rep, 0, quotient->size);
	}

	bit_position = 8 - (bit_size % 8) - 1;

	do
	{
		if (Compare(divisor, dividend) <= 0)
		{
			SubtractMagnitude(dividend, divisor); // dividend -= divisor

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
		if (h->sign)
		{
			fprintf(f, "- ");
		}

		for (unsigned int i = 0; i < h->size; i++)
		{
			fprintf(f, "%hhu ", h->rep[i]);
		}
	}
	else
	{
		fprintf(f, "NULL");
	}
}

void Power(Huge * mantissa, Huge * exponent)
{
	Huge counter;
	Huge one;
	Huge result;

	SetHuge(&result, 1);
	SetHuge(&counter, 1);
	SetHuge(&one, 1);
	while (Compare(&counter, exponent) <= 0)
	{
		//PrintHuge(stdout, &counter); printf("\n");
		//PrintHuge(stdout, result); printf("\n\n");
		Multiply(&result, mantissa);
		Add(&counter, &one);
	}
	//PrintHuge(stdout, &counter); printf("\n");
	//PrintHuge(stdout, result); printf("\n\n");

	FreeHuge(&counter);
	FreeHuge(&one);

	FreeHuge(mantissa);
	*mantissa = result;
}

// c = m^e mod n
void ModPower(Huge *mantissa, Huge *exponent, Huge * n, Huge *c)
{
	unsigned int i = exponent->size;
	unsigned char mask;

	assert(i);

	Huge tmp1, tmp2, tmp3, tmp4;

	SetHuge(&tmp1, 0);
	SetHuge(&tmp2, 0);
	SetHuge(&tmp3, 0);
	SetHuge(&tmp4, 0);

	CopyHuge(&tmp1, mantissa);
	SetHuge(c, 1);

	do
	{
		i--;
		for (mask = 1; mask; mask <<= 1)
		{
			// compute both sides of the branch below
			// should resist timing attack

			CopyHuge(&tmp3, c);
			CopyHuge(&tmp4, c);

			Multiply(&tmp3, &tmp1);
			Divide(&tmp3, n, NULL);

			if (exponent->rep[i] & mask)
			{
				CopyHuge(c, &tmp3);
			}
			else
			{
				CopyHuge(c, &tmp4);
			}

			// square tmp1
			CopyHuge(&tmp2, &tmp1);
			Multiply(&tmp1, &tmp2);
			Divide(&tmp1, n, NULL);
		}
	} while (i);

	FreeHuge(&tmp1);
	FreeHuge(&tmp2);
	FreeHuge(&tmp3);
	FreeHuge(&tmp4);
}

void LoadHuge(Huge *h, const unsigned char *bytes, int length)
{
	// skip over leading zeroes
	while (!(*bytes))
	{
		bytes++;
		length--;
	}

	h->sign = 0;
	h->size = length;
	h->rep = (unsigned char*)malloc(length);
	memcpy(h->rep, bytes, length);
}

void UnloadHuge(const Huge *h, unsigned char *bytes, int length)
{
	memcpy(bytes + (length - h->size), h->rep, length);
}

void MultiplicativeInverse(Huge *z, Huge *a)
{
	Huge i, j, y2, y1, y, quotient, remainder, a_temp;

	SetHuge(&i, 1);
	SetHuge(&j, 1);
	SetHuge(&remainder, 1);
	SetHuge(&y, 1);

	SetHuge(&a_temp, 1);

	SetHuge(&y2, 0);
	SetHuge(&y1, 1);

	CopyHuge(&i, a);
	CopyHuge(&j, z);

	if (z->sign)
	{
		Divide(&j, a, NULL);
		j.sign = 0; // force positive remainder
		Subtract(&j, a);
	}

	while ((j.size != 1) || j.rep[0]) // while j != 0
	{
		CopyHuge(&remainder, &i);
		CopyHuge(&i, &j);
		Divide(&remainder, &j, &quotient); // quotient = i / j. with remainder

		Multiply(&quotient, &y1);
		CopyHuge(&y, &y2);
		Subtract(&y, &quotient); // y = y2 - (y1 * quotient)

		CopyHuge(&j, &remainder);
		CopyHuge(&y2, &y1);
		CopyHuge(&y1, &y);
	}

	CopyHuge(z, &y2);
	CopyHuge(&a_temp, a);
	Divide(z, &a_temp, NULL); // z := y2 % a

	if (z->sign)
	{
		z->sign = 0;
		Subtract(z, &a_temp);
		if (z->sign)
		{
			z->sign = 0;
		}
	}
}