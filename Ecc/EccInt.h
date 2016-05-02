#pragma once

/*
example ECC using int types for clarity
*/

namespace EccInt {

	typedef struct
	{
		int x;
		int y;
	} Point;

	typedef struct
	{
		int private_key;
		Point public_key;
	} KeyPair;

	// y^2 = (x^3 + ax + b) % p
	typedef struct
	{
		int p;
		int a;
		int b;
		Point G;
	} DomainParameters;

	int Invert(int x, int y, int p);

	void AddPoints(Point *p1, Point *p2, int p);

	void DoublePoint(Point *p1, int p, int a);

	void MultiplyPoint(Point *p1, int k, int a, int p);
}