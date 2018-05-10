#ifndef __ECC_HPP__
#define __ECC_HPP__

#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <string>
#include <cmath>

#define ECC_DEBUG_MODE 1
#define ECC_DEBUG_VERBOSE_MODE 0

// C function for extended Euclidean Algorithm
int64_t gcdExtended(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
	// Base Case
	if (a == 0)
	{
		*x = 0;
		*y = 1;
		return b;
	}

	int64_t x1, y1; // To store results of recursive call
	int64_t gcd = gcdExtended(b%a, a, &x1, &y1);

	// Update x and y using results of recursive
	// call
	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

int64_t mod(int64_t);
int64_t inverseModule(int64_t x);

class ECC {
public:
	struct point {
		point() {}
		point(int64_t x, int64_t y) : x{ x }, y{ y }{}

		int64_t x;
		int64_t y;

		bool operator==(const point& rhs) const {
			return x == rhs.x && y == rhs.y;
		}

		point doubl() const {
			if (ECC_DEBUG_VERBOSE_MODE)
				std::cout << "double operation" << std::endl;

			int64_t s;
			int64_t denom = mod(3 * x * x + ECC::a);
			int64_t mod2y = mod(2 * y);

			if (denom % mod2y == 0)
				s = mod(denom / mod2y);
			else
				s = mod(denom * inverseModule(mod2y));

			int64_t x3 = mod(s * s - x - x);
			return { x3, mod(s * (x - x3) - y) };
		}

		void print() const {
			std::cout << "point is (" << x << ", " << y << ")" << std::endl;
		}

		friend class ECC;
	};

	struct pointHash
	{
		std::size_t operator()(point const& pt) const noexcept
		{
			return std::hash<long long>{}(pt.x ^ (pt.y << 1));
		}
	};

	using CyclicGroup = std::unordered_set<point, pointHash>;

	void setStartPoint(point pt) { P = pt; }
	point getStartPoint() { return P; }

	point getKStepPoint(int64_t k);

	int64_t createCyclicGroup();

	int64_t getCyclicGroupSize() { return E.size(); }

	static int64_t a;
	static int64_t b;
	static int64_t p;

private:
	point P;
	CyclicGroup E;
};

int64_t mod(int64_t x)
{
	while (x < 0)
		x += ECC::p;
	return x % ECC::p;
}

int64_t inverseModule(int64_t x)
{
	if (ECC_DEBUG_VERBOSE_MODE)
		std::cout << "inverse modulus of " << x << ": ";
	int64_t ret, tmp;

	if (gcdExtended(x, ECC::p, &ret, &tmp) == 1)
	{
		if (ECC_DEBUG_VERBOSE_MODE)
			std::cout << "GCD Extended, result " << mod(ret) << std::endl;

		return mod(ret);
	}
	else
	{
		if (ECC_DEBUG_VERBOSE_MODE)
			std::cout << "Euler's theorem, result " << static_cast<int64_t>(mod(std::pow(x, ECC::p - 2))) << std::endl;

		return static_cast<int64_t>(mod(std::pow(x, ECC::p - 2)));
	}
}

inline ECC::point operator+(ECC::point p1, ECC::point p2)
{
	if (p1 == p2)
		return p1.doubl();

	if (p1.x == p2.x)
	{
		if (ECC_DEBUG_VERBOSE_MODE)
			std::cout << "encounter infinity" << std::endl;
		return { 0, 0 };
	}

	if (p1.x == 0 && p1.y == 0)
		return p2;

	if (p2.x == 0 && p2.y == 0)
		return p1;

	if (ECC_DEBUG_VERBOSE_MODE)
		std::cout << "add operation" << std::endl;

	int64_t s;
	int64_t modDeltaX = mod(p2.x - p1.x);
	int64_t modDeltaY = mod(p2.y - p1.y);

	if ((modDeltaY % modDeltaX) == 0)
		s = modDeltaY / modDeltaX;
	else
		s = mod(modDeltaY * inverseModule(modDeltaX));

	int64_t x3 = mod(s * s - p1.x - p2.x);
	return { x3, mod(s * (p1.x - x3) - p1.y) };
}

// Recursive implementation of double and add algorithm
inline ECC::point operator*(ECC::point pt, int64_t k)
{
	ECC::point ret;

	if (k == 0)
		ret = pt;
	else if (k == 1)
	{
		ret = pt;
	}
	else if (k % 2 == 1)
	{
		ret = pt + pt * (k - 1);

		if (ECC_DEBUG_MODE)
		{
			std::cout << "after add: ";
			ret.print();
		}
	}
	else
	{
		ret = (pt * (k / 2)).doubl();

		if (ECC_DEBUG_MODE)
		{
			std::cout << "after double: ";
			ret.print();
		}
	}

	return ret;
}

ECC::point ECC::getKStepPoint(int64_t k) { return P * k; };

int64_t ECC::createCyclicGroup() {
	point ppt = P;

	//int i = 0;
	while (1) {
		if (E.find(ppt) == E.end())
		{
			if (ECC_DEBUG_MODE)
				ppt.print();

			E.insert(ppt);
		}
		else
			break;

		ppt = ppt + P;
	}
	if (ECC_DEBUG_MODE)
		std::cout << "size is " << E.size() << std::endl;
	return E.size();
}

#endif // !__ECC_HPP__

