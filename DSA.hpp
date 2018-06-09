#ifndef __DSA_HPP__
#define __DSA_HPP__

#include "MathUtils.hpp"

using namespace MathUtil;

#define DSA_DEBUG_MODE 1

template<class T>
class DSA{
public:
	DSA() { randSeedinit(); }
	void find_p_q() 
	{
		if (DSA_DEBUG_MODE) std::cout << "find_p_q...";
		while(1)
		{
			_q = findRandomPrime((T)UINT16_MAX >> 1, (T)UINT16_MAX, 10);
			//find_prime(_q, 5, 1 << 15);
			for (int i = 1; i < 128; i++)
			{
				T M = rand() % (UINT32_MAX >> 1) + (UINT32_MAX >> 1);
				//T M = rand() % (1 << 15);

				T Mr = mod(M, 2 * _q);

				_p = M - Mr + 1;
				if (isPrime(_p, 10))
				{
					if (DSA_DEBUG_MODE) std::cout << "OK\n";
					return;
				}

				if (DSA_DEBUG_MODE && i % 10 == 0) std::cout << ".";
			}
		}
	}

	void find_g()
	{
		if (DSA_DEBUG_MODE) std::cout << "find_g\n";
		T h = 2;
		while (1)
		{
			_g = power_mod(h, (_p - 1) / _q, _p);
			if (_g != 1)
				return;
			h = rand() % (_p - 3) + 2;
		}
	}

	void find_x()
	{
		if (DSA_DEBUG_MODE) std::cout << "find_x\n";
		_x = rand() % _q;
		_X = power_mod(_g, _x, _p);
	}

	void key_generation()
	{
		find_p_q();
		find_g();
		find_x();
	}

	void signing(T m)
	{
		if (DSA_DEBUG_MODE) std::cout << "signing\n";
		_m = m;
		while (1)
		{
			_k = rand() % (_q - 2) + 1;
			_r = mod(power_mod(_g, _k, _p), _q);
			if (_r == 0)
				continue;
			_s = mod(inverse_mod(_k, _q) * (sha1Hash(m) + _x * _r), _q);
			if (_s == 0)
				continue;
			return;
		}
	}

	bool checking(T p, T q, T g, T X, T m, T r, T s)
	{
		if (!(0 < r && r < q && 0 < s && s < q))
			return false;
		T w = inverse_mod(s, q);
		T u1 = mod(hash_fn(m) * w, q);
		T u2 = mod(r * w, q);
		T v = mod(mod(power_mod(g, u1, p) * power_mod(X, u2, p), p), q);
		if (v == r)
		{
			std::cout << "Checked true" << std::endl;
			return true;
		}
		else
		{
			std::cout << "Checked false" << std::endl;
			return false;
		}
	}

	T calculate_x(T s1, T s2, T m1, T m2, T r, T q)
	{
		T di = mod(inverse_mod(s1, q) * s2, q);
		T no = mod(hash_fn(m2) - di * hash_fn(m1), q);
		T de = mod((di - 1) * r, q);
		T ret = mod(inverse_mod(de, q) * no, q);

		std::cout << "private key retrieved: " << ret << std::endl;

		return ret;
	}

	void reuse_k(T m)
	{
		_m = m;
		while (1)
		{
			_r = mod(power_mod(_g, _k, _p), _q);
			if (_r == 0)
				continue;
			_s = mod(inverse_mod(_k, _q) * (hash_fn(m) + _x * _r), _q);
			if (_s == 0)
				continue;
			return;
		}
	}

	void print(const std::string& filename)
	{
		if (DSA_DEBUG_MODE) std::cout << "print\n";
		std::ofstream myfile;
		myfile.open(filename);
		myfile << "(p, q, g), X, (m, r, s)    k, x\n"
			   << "(" << _p
			   << ", " << _q
			   << ", " << _g
			   << "), " << _X
			   << ", (" << _m
			   << ", " << _r
			   << ", " << _s
			   << ")    " << _k
			   << ", " << _x
			   << "\n";
		myfile.close();
	}

	// public param
	T _p;
	T _q;
	T _g;

	// private key
	T _x;

	// public key
	T _X;

	// message
	T _m;

	// signature
	T _r;
	T _s;

	// temperal
	T _k;
};

#endif
