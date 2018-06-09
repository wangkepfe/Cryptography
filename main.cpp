//#include "ECDSA.hpp"
//
//int64_t ECC<int64_t>::a;
//int64_t ECC<int64_t>::b;
//int64_t ECC<int64_t>::p;
//
//int main()
//{
	//ECDSA<int64_t> ecdsa((int64_t)1 << 15, (int64_t)1 << 16);
	//ecdsa.keygen();
	//ecdsa.signatureGen(12345);
	//ecdsa.verification(12345);
	//cout << endl;

	///*ecdsa.signatureGenKreuse(23456);
	//cout << endl;
	//cout << endl;*/

	//ECDSA<int64_t> ecdsa2((int64_t)1 << 15, (int64_t)1 << 16);
	//ecdsa2.keygen();
	//ecdsa2.signatureGen(12345);
	//ecdsa2.verification(12345);
	//cout << endl;

	///*ecdsa2.signatureGenKreuse(23456);
	//cout << endl;
	//cout << endl;*/

	//ECDSA<int64_t> ecdsa3((int64_t)1 << 15, (int64_t)1 << 16);
	//ecdsa3.keygen();
	//ecdsa3.signatureGen(12345);
	//ecdsa3.verification(12345);
	//cout << endl;

	//ecdsa3.signatureGenKreuse(23456);

//	ECDSA<int64_t> ecdsa(49363, 8633, 15941, 49697, 38138, 47385);
//	ecdsa.retrieveKey(1839, 32917, 36018, 49741, 25608);//40478
//
//	getchar();
//	return 0;
//}

//#include "DSA.hpp"
//
//int main()
//{
//	DSA<uint64_t> dsa;
//
//	dsa.key_generation();
//	dsa.signing(12345);
//	dsa.print("dsa1.txt");
//
//	dsa.key_generation();
//	dsa.signing(54321);
//	dsa.print("dsa2.txt");
//
//	dsa.key_generation();
//	dsa.signing(12321);
//	dsa.print("dsa3.txt");
//
//	//dsa.reuse_k(32123);
//	//dsa.print("dsa4.txt");
//
//	//std::cout << dsa.inverse_mod( 1234 ,61757 );
//
//	//dsa.checking(509, 30, 224, 9, 3333, 1, 22);
//	//dsa.calculate_x(1438, 3212, 857, 9174, 1040, 10177);
//
//	getchar();
//
//	return 0;
//}

#include "Elgamal.hpp"

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost;
using namespace multiprecision;

int main() {
	srand(time(NULL));

	ElgamalClient<uint512_t> a("10000000000000000000000000000000000000000000000000000000000000000005707", 5);
	ElgamalClient<uint512_t> b("10000000000000000000000000000000000000000000000000000000000000000005707", 5);
	
	a.generatePrivateKey();
	b.generatePrivateKey();

	cout << "private key\na.x = " << a.x << "\nb.x = " << b.x << endl;
	
	a.calculatepublicKey();
	b.calculatepublicKey();

	cout << "public key\na.X = " << a.X << "\nb.X = " << b.X << endl;
	
	a.calculateSharedKey(b.X);
	b.calculateSharedKey(a.X);

	cout << "shared key\na.k = " << a.k << "\nb.k = " << b.k << endl;

	const string msg = "98.248.119.127 Ke Wang";

	cout << "message from a is \nc = " << msg << endl;

	uint512_t c = a.encryptMessage(msg);
	string m = b.decryptMessage(c);

	cout << "encrypted message\nc = " << c << endl;
	cout << "decrypted message at b is \nm = " << m << endl;
	
	/*
	ElgamalClient<uint512_t> a("10000000000000000000000000000000000000000000000000000000000000000005707", 5);
	a.generatePrivateKey();
	a.calculatepublicKey();
	cout << "{" << a.X << "}" << endl;
	a.calculateSharedKey(lexical_cast<uint512_t>("1219196886851042680574552998311030593093353237836921210295465809859737"));
	uint512_t c = a.encryptMessage("98.248.119.127 Ke Wang");
	cout << "{" << c << "}" << endl;
	*/
	getchar();
	return 0;
}