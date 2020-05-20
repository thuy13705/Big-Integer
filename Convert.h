#pragma once
#pragma once
#include<iostream>
#include<string>
#define ARRBITS_SIZE 4
using namespace std;


class QInt
{
public:
	unsigned int _arrBits[4];
public:
	QInt();
	QInt(string numStr, unsigned int base);


public:
	string DecToBin() const;
	void BinToDec(string bit);
	string DecToHex();
	void HexToDec(string hex);



public:
	QInt operator+(const QInt& x);
	QInt operator-(const QInt& x);
	QInt operator*(const QInt& x);
	QInt operator / (QInt& x);
public:
	QInt operator&(const QInt& x);
	QInt operator|(const QInt& x);
	QInt operator^(const QInt& x);
	QInt operator~();
public:
	QInt operator>>(unsigned i);
	QInt operator<<(unsigned i);
	QInt rol();
	QInt ror();
public:
	bool operator ==(const QInt& x);
public:
	void printQInt();
	void test();


};
string HexToBin(string hex);
string BinToHex(string bit);
string DiveBy2(string numStr);
string MultBy2(string numStr);



