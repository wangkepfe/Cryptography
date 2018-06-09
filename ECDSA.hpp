#ifndef __ECDSA_HPP__
#define __ECDSA_HPP__

#include "ECC.hpp"

template<class T>
class ECDSA : public ECC<T>{
public:
	ECDSA(T _p, T _a, T _b, T _q, T _x, T _y)
		: ecc(_p, _a, _b, _q, _x, _y)
		, p{ ECC<T>::p }
		, q{ ecc.q }
		, P{ ecc.P }
	{
		ecc.printInfo();
	}
	ECDSA(T min, T max) 
		: ecc(min, max)
		, p{ ECC<T>::p }
		, q{ ecc.q } 
		, P{ ecc.P }
	{
		ecc.printInfo();
	}

	void keygen()
	{
		MOD<T> = p;
		x = rd() % (q - 1) + 2;
		X = P * x;
	}

	void signatureGen(T msg)
	{
		MOD<T> = p;
		k = rd() % (q - 1) + 2;
		R = P * k;
		r = R.x;
		hm = sha1Hash(msg);
		s = mod((hm + x * r) * inverse_mod(k, q), q);

		printInfo();
	}

	void signatureGenKreuse(T msg)
	{
		MOD<T> = p;
		R = P * k;
		r = R.x;
		hm = sha1Hash(msg);
		s = mod((hm + x * r) * inverse_mod(k, q), q);

		printInfo();
	}

	void retrieveKey(T s1, T s2, T hm1, T hm2, T r)
	{
		MOD<T> = q;
		T key = mod(mod(s1 * hm2 - s2 * hm1) * inverse_mod((s2 - s1) * r));
		cout << "Private key retrieved: " << key << endl;
	}

	void verification(T msg)
	{
		MOD<T> = q;
		T w = inverse_mod(s);
		hm = sha1Hash(msg);
		T u1 = mod(w * hm);
		T u2 = mod(w * r);
		MOD<T> = p;
		auto V = (P * u1) + (X * u2);
		MOD<T> = q;
		if (mod(V.x) == mod(r))
		{
			cout << "Verification: True" << endl;
		}
		else
		{
			cout << "Verification: False" << endl;
		}
	}

	void printInfo()
	{
		cout << "ECDSA params:" << endl
			<< "private key: x = " << x
			<< ", ephemeral key: k = " << k << endl
			<< "public: X = (" << X.x << ", " << X.y
			<< "), r = " << r
			<< ", s = " << s << endl
			<< "hashed message: " << hm << endl;

		cout << "(" << p << ","
			<< ECC<T>::a << ","
			<< ECC<T>::b << ","
			<< q << ",("
			<< P.x << ","
			<< P.y << ")),("
			<< X.x << ","
			<< X.y << "),("
			<< hm << ","
			<< r << ","
			<< s << ")" << endl;
	}

	ECC<T> ecc;

	T &p;
	T &q;

	ECC<T>::point &P;

	T x;
	ECC<T>::point X;

	ECC<T>::point R;
	T k;
	T r;
	T s;

	T hm;
};

#endif