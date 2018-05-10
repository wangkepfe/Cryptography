#include "ECC.hpp"

int64_t ECC::a = 1123;
int64_t ECC::b = 1002;
int64_t ECC::p = 7879;

//int64_t ECC::a = 4192;
//int64_t ECC::b = 20814;
//int64_t ECC::p = 73529;

//int64_t ECC::a = 2;
//int64_t ECC::b = 2;
//int64_t ECC::p = 17;

//int64_t ECC::a = 12;
//int64_t ECC::b = 3;
//int64_t ECC::p = 13;

//int64_t ECC::a = 4;
//int64_t ECC::b = 20;
//int64_t ECC::p = 29;

int main()
{
	ECC ecc;

	ecc.setStartPoint({ 236, 125 });
	//ecc.setStartPoint({ 59745, 8956 });
	//ecc.setStartPoint({ 5, 1 });
	//ecc.setStartPoint({ 1, 4 });
	//ecc.setStartPoint({ 8, 10 });

	//ecc.getKStepPoint(9).print();
	//ecc.getKStepPoint(20).print();

	ecc.createCyclicGroup();

	getchar();

	return 0;
}