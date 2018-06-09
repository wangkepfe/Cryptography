#ifndef  __ELGAMAL_HPP__
#define __ELGAMAL_HPP__

#include <string>

#include <boost/lexical_cast.hpp>

#include "MathUtils.hpp"

using namespace MathUtil;
using namespace boost;
using namespace std;


template<class T>
struct ElgamalClient {
	ElgamalClient() {}
	ElgamalClient(T p, T g) : p{ p }, g{ g } {}
	ElgamalClient(const string& ps, T g) : p{ lexical_cast<T>(ps) }, g{ g } {}

	T p;
	T g;

	T x;
	T X;
	T k;

	void generatePrivateKey() {
		x = findRandomPrime(p / 2, p - 1, 100);
	}

	void calculatepublicKey() {
		X = power_mod(g, x, p);
	}

	void calculateSharedKey(T Y) {
		k = power_mod(Y, x, p);
	}

	T wordsToInt(const string& ms) {
		T ret = 0;
		for (char c : ms) {
			ret += c;
			ret = ret << 7;
		}
		return ret;
	}

	string intToWords(T m) {
		string ret;
		for (int i = 0; i < 512 / 7; i++) {
			char c = char(m & 0x7F);
			if (c != 0)
				ret = c + ret;
			m = m >> 7;
		}
		return ret;
	}

	T encryptMessage(T m) {
		T c = mod(m * k, p);
		return c;
	}

	T encryptMessage(const string& ms) {
		T m = wordsToInt(ms);
		T c = mod(m * k, p);
		return c;
	}

	string decryptMessage(T c) {
		T m = mod(c * inverse_mod(k, p), p);
		return intToWords(m);
	}

	string decryptMessage(const string& cs) {
		T c = lexical_cast<T>(cs);
		T m = mod(c * inverse_mod(k, p), p);
		return intToWords(m);
	}
};

#endif // ! __ELGAMAL_HPP__

