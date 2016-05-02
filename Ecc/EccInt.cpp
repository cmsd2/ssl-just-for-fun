#include "stdafx.h"
#include "EccInt.h"
#include <Utils/Gcd.h>

namespace EccInt {

	int Invert(int x, int y, int p)
	{
		int inverse = ExtendedEuclid(y, p);

		return x * inverse;
	}

	/*
	y^2 = x^3 + ax + b

	addition of distinct points:
	x3 = (lambda - x1 - x2) % p
	y3 = (lamda(x1 - x3) - y1) % p
	lambda = ( (y2 - y1) * (x2 - x1)^-1 ) % p

	lambda is the slope of the line connecting the 3 points.
	*/
	void AddPoints(Point *p1, Point *p2, int p)
	{
		Point p3;
		int lambda = Invert(p2->y - p1->y, p2->x - p1->x, p);

		p3.x = ((lambda * lambda) - p1->x - p2->x) % p;
		p3.y = ((lambda * (p1->x - p3.x)) - p1->y) % p;

		p1->x = p3.x;
		p1->y = p3.y;
	}

	/*
	y^2 = x^3 + ax + b

	addition of point with itself using tangent of the curve itself:
	x3 = (lambda^2 - 2x1) % p
	y3 = (lambda(x1 - x3) - y1) % p
	dy/dx = lambda = ( (3 * x1^2 + a) * (2 * y1)^-1 ) % p
	*/
	void DoublePoint(Point *p1, int p, int a)
	{
		Point p3;
		int lambda = Invert(3 * (p1->x * p1->x) + a, 2 * p1->y, p);

		p3.x = ((lambda * lambda) - (2 * p1->x)) % p;
		p3.y = ((lambda * (p1->x - p3.x)) - p1->y) % p;

		p1->x = p3.x;
		p1->y = p3.y;
	}

	void MultiplyPoint(Point *p1, int k, int a, int p)
	{
		Point dp;
		int mask;
		
		/* flag to say that p1 is "zero" for the first addition. 
		zero doesn't lie on the curve so there's no value we could have used.
		*/
		int paf = 1;

		dp.x = p1->x;
		dp.y = p1->y;

		for (mask = 1; mask; mask <<= 1)
		{
			if (mask & k)
			{
				if (paf)
				{
					paf = 0;
					p1->x = dp.x;
					p1->y = dp.y;
				}
				else
				{
					AddPoints(p1, &dp, p);
				}
			}

			DoublePoint(&dp, p, a);
		}
	}
}