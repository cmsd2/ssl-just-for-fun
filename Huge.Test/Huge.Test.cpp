// Huge.Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Huge/Huge.h>
#include <string.h>
#include <assert.h>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <RPN expression>\n", argv[0]);
		return 1;
	}

	const int MAX_STACK = 10;

	Huge ha[MAX_STACK];

	int stack_size = 0;

	for (int i = 1; i < argc; i++)
	{
		char c = argv[i][0];

		Huge * v0 = stack_size >= 2 ? &ha[stack_size - 2] : NULL;
		Huge * v1 = stack_size >= 1 ? &ha[stack_size - 1] : NULL;

		switch (c)
		{
		case '+':
			assert(stack_size >= 2);
			Add(v0, v1);
			stack_size--;
			break;
		case '-':
			assert(stack_size >= 2);
			Subtract(v0, v1);
			stack_size--;
			break;
		case '*':
			assert(stack_size >= 2);
			Multiply(v0, v1);
			stack_size--;
			break;
		case '/':
			assert(stack_size >= 2);
			Huge hc;
			Divide(v0, v1, &hc);
			CopyHuge(v0, &hc);
			stack_size--;
			break;
		case '%':
			assert(stack_size >= 2);
			Divide(v0, v1, NULL);
			stack_size--;
			break;
		case '<':
			assert(stack_size >= 1);
			LeftShift(v1);
			break;
		case '^':
			assert(stack_size >= 2);
			Huge result;
			Power(v0, v1, &result);
			FreeHuge(v0);
			*v0 = result;
			stack_size--;
			break;
		default:
			assert(stack_size < MAX_STACK);
			unsigned value = atoi(argv[i]);
			SetHuge(&ha[stack_size], value);
			stack_size++;
		}
	}

	for (int i = 0; i < stack_size; i++)
	{
		PrintHuge(stdout, &ha[i]);
		printf("\n");
	}

    return 0;
}

