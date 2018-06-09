#ifndef __ECC_HPP__
#define __ECC_HPP__

#include <unordered_set>
#include <cmath>

#include "MathUtils.hpp"
using namespace MathUtil;

template<class T>
class ECC {
public:
	ECC() {

	}
	ECC(T _p, T _a, T _b, T _q, T _x, T _y) {
		p = _p;
		a = _a;
		b = _b;
		q = _q;
		P.x = _x;
		P.y = _y;
	}
	ECC(T min, T max) {
		generate(min, max);
	}

	struct point {
		point() {}
		point(T x, T y) : x{ x }, y{ y }{}

		T x;
		T y;

		bool operator==(const point& rhs) const {
			return x == rhs.x && y == rhs.y;
		}

		point operator+(point p2);
		point operator*(T k);
		point doubl() const;

		void print() const {
			std::cout << "point is (" << x << ", " << y << ")" << std::endl;
		}
	};

	struct pointHash
	{
		std::size_t operator()(point const& pt) const noexcept
		{
			return std::hash<long long>{}(pt.x ^ (pt.y << 1));
		}
	};

	using CyclicGroup = std::unordered_set<point, pointHash>;

	void generate(T min, T max)
	{
		while (1)
		{
			p = findRandomPrime(min, max, 1 << 15);
			MOD<T> = p;

			P.x = rd() % p;
			P.y = rd() % p;
			a = rd() % p;
			b = mod(P.y * P.y - P.x * P.x * P.x - P.x * a);

			if (mod(4 * a * a * a + 27 * b * b) != 0)
			{
				createCyclicGroup();
				auto sqrtp = sqrt(p);
				if (isPrime(q, 1 << 15) && p + 1 - 2 * sqrtp < q && p + 1 + 2 * sqrtp > q)
					break;
			}
		}	
		return;
	}

	void printInfo()
	{
		cout << "ECC params: a = " << a
			<< ", b = " << b
			<< ", p = " << p << endl
			<< "Start point: (" << P.x
			<< ", " << P.y
			<< ")" << endl
			<< "|E| = " << q << endl;
	}

	T createCyclicGroup();

	static T a;
	static T b;
	static T p;

	point P;
	CyclicGroup E;
	T q;
};

template<class T>
typename ECC<T>::point ECC<T>::point::doubl() const {

	if (y == 0)
	{
		return { 0, 0 };
	}

	T s = mod(mod(3 * x * x + a) * inverse_mod(2 * y));

	T x3 = mod(s * s - x - x);
	return { x3, mod(s * (x - x3) - y) };
}

template<class T>
typename ECC<T>::point ECC<T>::point::operator+(typename ECC<T>::point p2)
{
	typename ECC<T>::point &p1 = *this;

	if (p1 == p2)
		return p1.doubl();

	if (p1.x == p2.x)
	{
		return { 0, 0 };
	}

	if (p1.x == 0 && p1.y == 0)
		return p2;

	if (p2.x == 0 && p2.y == 0)
		return p1;


	T s = mod(mod(p2.y - p1.y) * inverse_mod(p2.x - p1.x));

	T x3 = mod(s * s - p1.x - p2.x);

	return { x3, mod(s * (p1.x - x3) - p1.y) };
}

template<class T>
typename ECC<T>::point ECC<T>::point::operator*(T k)
{
	if (k == 0)
		return typename ECC<T>::point(0, 0);
	else if (k == 1)
		return *this;
	else if (k % 2 == 1)
		return *this + *this * (k - 1);
	else
		return this->doubl() * (k / 2);
}

template<class T>
typename T ECC<T>::createCyclicGroup() {
	typename ECC<T>::point ppt = P;
	const typename ECC<T>::point infi(0, 0);
	E.clear();

	while (1) {	
		E.insert(ppt);
		if (ppt == infi)
			break;	

		ppt = ppt + P;
	}
	q = E.size();

	return q;
}

#endif // !__ECC_HPP__

