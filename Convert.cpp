#define  _CRT_SECURE_NO_WARNINGS
#include "Convert.h"


QInt::QInt()
{
	memset(_arrBits, 0, 16);
}
QInt::QInt(string numStr, unsigned int base)
{
	memset(_arrBits, 0, 16);

	switch (base)
	{
	case 2:
	{
		while (numStr.length() > 1 && numStr[0] == '0')
			numStr.erase(0, 1);
		if (numStr == "0")
			return;

		// convert positive number string into QInt
		unsigned int i = 0;
		unsigned int size = ARRBITS_SIZE * 32;
		string tmp = numStr;

		for (int i = 0; i < numStr.length(); i++)
		{
			unsigned int arrIndex = (size - i - 1) / 32;
			unsigned int bitIndex = i % 32;
			if (numStr[numStr.length() - i - 1] == '1')
				_arrBits[arrIndex] ^= (1 << bitIndex);
		}
		break;
	}
	case 10:
	{

		bool isNegative = 0;
		if (numStr[0] == '-')
		{
			isNegative = 1;
			numStr.erase(0, 1);
		}

		//0012, -00
		while (numStr.length() > 1 && numStr[0] == '0')
			numStr.erase(0, 1);
		if (numStr == "0")
			return;

		unsigned int i = 0;
		unsigned int size = ARRBITS_SIZE * 32;
		string tmp = numStr;

		while (tmp != "0")
		{
			if ((tmp[tmp.length() - 1] - 48) % 2 == 1)
			{
				unsigned int arrIndex = (size - i - 1) / 32;
				unsigned int bitIndex = i % 32;
				_arrBits[arrIndex] ^= (1 << bitIndex);
			}
			tmp = DiveBy2(tmp);
			i++;
		}
		//if negative number, present complement 2
		if (isNegative)
		{
			for (int i = 0; i < ARRBITS_SIZE; i++)
				_arrBits[i] = ~_arrBits[i]; // bitwise

			i = ARRBITS_SIZE - 1;
			while (_arrBits[i] == 0xFFFFFFFF)
			{
				_arrBits[i] = 0;
				i--;
			}
			_arrBits[i] = _arrBits[i] + 1;
		}

		break;

	}
	case 16:
	{
		while (numStr.length() > 1 && numStr[0] == '0')
			numStr.erase(0, 1);
		if (numStr == "0")
			return;
		numStr = HexToBin(numStr);
		// convert positive number string into QInt
		unsigned int i = 0;
		unsigned int size = ARRBITS_SIZE * 32;
		string tmp = numStr;

		for (int i = 0; i < numStr.length(); i++)
		{
			unsigned int arrIndex = (size - i - 1) / 32;
			unsigned int bitIndex = i % 32;
			if (numStr[numStr.length() - i - 1] == '1')
				_arrBits[arrIndex] ^= (1 << bitIndex);
		}
		break;
		break;
	}

	default:
		break;
	}
}


string QInt::DecToBin() const
{
	string bin = "";
	unsigned len = ARRBITS_SIZE * 32;


	//convert to binary string 
	for (int i = 0; i < len; i++)
	{
		char c = _arrBits[i / 32] >> (32 - i % 32 - 1) & 1;
		bin += (c + 48);
	}
	//Remove excess zeros and convert binary to bool*bin
	unsigned zeros = 0;
	while (bin[zeros] == '0')
	{
		zeros++;
	}
	bin.erase(0, zeros);

	return bin;
}
void QInt::BinToDec(string bit)
{
	memset(_arrBits, 0, 16);
	unsigned len = ARRBITS_SIZE * 32;
	unsigned n = bit.length();
	unsigned arrIndex;
	unsigned bitIndex;


	for (int i = 0; i < n; i++)
	{
		arrIndex = (len - i - 1) / 32;
		bitIndex = i % 32;
		if (bit[n - i - 1] == 1)
			_arrBits[arrIndex] ^= (1 << bitIndex);

	}
}
string QInt::DecToHex()
{
	string hex = "";

	string bit = DecToBin();
	hex = BinToHex(bit);
	return hex;
}
void QInt::HexToDec(string hex)
{
	string bin=HexToBin(hex);
	BinToDec(hex);
}
string HexToBin(string hex) {
	string bit = "";
	string _arrhex[16]={ "0000","0001","0010","0011",
						"0100","0101","0110","0111",
						"1000","1001","1010","1011",
						"1100","1101","1110","1111" };
	int temp;
	for (int i = hex.length() - 1; i >= 0; i--)
	{
		switch (hex[i])
		{
		case 'A':
			temp = 10;
			break;
		case 'B':
			temp = 11;
			break;
		case 'C':
			temp = 12;
			break;
		case 'D':
			temp = 13;
			break;
		case 'E':
			temp = 14;
			break;
		case 'F':
			temp = 15;
			break;
		default:
			temp = hex[i] - '0';
		}
		bit = _arrhex[temp] + bit;
	}
	return bit;
}
string BinToHex(string bit)
{
	string hex = "";
	unsigned valueOf_4Bits = 0;
	unsigned n = bit.length();

	//conver bit array to hex string
	unsigned j = 0;
	unsigned i = 0;

	while (i < n)
	{
		while (i < n && j < 4)
		{
			if (bit[n - i - 1] == 1)
				valueOf_4Bits ^= (1 << j);
			i++;
			j++;
		}
		j = 0;
		switch (valueOf_4Bits)
		{
		case 10:
			hex += 'A';
			break;
		case 11:
			hex += 'B';
			break;
		case 12:
			hex += 'C';
			break;
		case 13:
			hex += 'D';
			break;
		case 14:
			hex += 'E';
			break;
		case 15:
			hex += 'F';
			break;

		default:
			hex = char(valueOf_4Bits + 48) + hex;
			break;
		}
		valueOf_4Bits = 0;
	}

	return hex;

}


QInt  QInt::operator+(const QInt& x)
{
	long long maxUnsignes = 0xFFFFFFFF;
	QInt result;
	bool mis = 0;

	for (int i = ARRBITS_SIZE - 1; i >= 0; i--)
	{
		if (maxUnsignes - _arrBits[i] - x._arrBits[i] - mis >= 0)
		{
			result._arrBits[i] = _arrBits[i] + x._arrBits[i] + mis;
			mis = 0;
		}
		else
		{
			result._arrBits[i] = -maxUnsignes - 1 + _arrBits[i] + x._arrBits[i] + mis;
			mis = 1;
		}
	}

	return result;
}
QInt  QInt::operator-(const QInt& x)
{
	// a-b = a+(-b)
	//tim so doi cua x
	QInt cloneX = x;
	for (int i = 0; i < ARRBITS_SIZE; i++)
		cloneX._arrBits[i] = ~cloneX._arrBits[i];

	unsigned i = ARRBITS_SIZE - 1;
	while (i > 0 && cloneX._arrBits[i] == 0xFFFFFFFF)
	{
		cloneX._arrBits[i] = 0;
		i--;
	}
	if (i >= 0)
		cloneX._arrBits[i] += 1;

	return *this + cloneX;
}
QInt  QInt::operator*(const QInt& x)
{
	QInt result;
	QInt tmp = *this;
	string bin = x.DecToBin();
	unsigned n = bin.length();

	for (int i = n - 1; i >= 0; i--)
	{
		if (i != n - 1)
			tmp = tmp + tmp;
		if (bin[i] == 1)
		{
			result = result + tmp;
		}

	}

	return result;
}
QInt  QInt::operator/(QInt& x)
{

	if (*this == QInt() || *this == QInt("-0", 10))
		return *this;
	if (x == QInt("0", 10) || x == QInt("-0", 10))
	{
		throw new exception("Error divise for 0.\n");
		return x;
	}
	bool isNeGative = 0;//FALSE
	QInt Q(*this);
	QInt M(x);
	QInt A;

	if (_arrBits[0] & (1 << 31))
	{
		isNeGative = 1 - isNeGative;
		Q = ~Q;
	}
	if (x._arrBits[0] & (1 << 31))
	{
		isNeGative = 1 - isNeGative;
		M = ~M;
	}


	for (int i = 0; i < 128; i++)
	{
		A = A + A;
		if (Q._arrBits[3] & (1 << 31))
		{
			A._arrBits[0] |= 1;
		}
		Q = Q + Q;
		A = A - M;
		if (A._arrBits[3] & (1 << 31))
		{
			A = A + M;

		}
		else
			Q._arrBits[0] |= 1;

	}
	if (isNeGative)
	{
		Q = ~Q;
	}
	return Q;

}


QInt  QInt::operator&(const QInt& x)
{
	QInt result;
	for (int i = 0; i < 4; i++)
		result._arrBits[i] = _arrBits[i] & x._arrBits[i];
	return result;
}
QInt  QInt::operator|(const QInt& x)
{
	QInt result;
	for (int i = 0; i < 4; i++)
		result._arrBits[i] = _arrBits[i] | x._arrBits[i];
	return result;
}
QInt  QInt::operator^(const QInt& x)
{
	QInt result;
	for (int i = 0; i < 4; i++)
		result._arrBits[i] = _arrBits[i] ^ x._arrBits[i];
	return result;
}
QInt  QInt::operator~()
{
	QInt result;
	for (int i = 0; i < 4; i++)
		result._arrBits[i] = ~_arrBits[i];
	return result;
}

bool QInt::operator==(const QInt& x)
{
	for (int i = 0; i < 4; i++)
		if (_arrBits[i] != x._arrBits[i])
			return 0;

	return 1;

}

QInt  QInt::operator>>(unsigned i)
{
	QInt result;
	string numbin = this->DecToBin();
	//cout << numbin << endl;
	int len = numbin.length();
	if (len > i)
	{
		for (int index = 0; index < i; index++)
			numbin = '0' + numbin;
		numbin.erase(len, i);
		//cout << numbin << endl;
		result.BinToDec(numbin);
	}
	else
		numbin = "0";
	return result;
}
QInt  QInt::operator<<(unsigned i)
{
	QInt result;
	string numbin = this->DecToBin();
	//cout << numbin << endl;
	int len = numbin.length();
	for (int index = 0; index < i; index++)
		numbin = numbin+'0';
	while (numbin.length() > 128)
		numbin.erase(0, 1);
	//cout << numbin << endl;
	result.BinToDec(numbin);
	return result;
}
QInt QInt::rol()
{
	QInt result;
	string numbin = this->DecToBin();
	int len = numbin.length();
	if (len < 128)
	{
		numbin = '0' + numbin;
		len++;
	}
	numbin = numbin + numbin[0];
	if (len>=128)
		numbin.erase(0, 1);
	else
	{
		while (numbin[0] == '0')
			numbin.erase(0, 1);
	}
	cout << numbin << endl;
	result.BinToDec(numbin);
	return result;
}
QInt QInt::ror()
{
	QInt result;
	string numbin = this->DecToBin();
	int len = numbin.length();
	for (int i = 0; i < 128 - len; i++)
		numbin = '0' + numbin;
	numbin = numbin[numbin.length()-1] + numbin;
	numbin.erase(numbin.length()-1, 1);
	while (numbin[0] == '0')
		numbin.erase(0, 1);
	cout << numbin << endl;
	result.BinToDec(numbin);
	return result;
}


void QInt::test()
{
	for (int i = 0; i < 4; i++)
		cout << _arrBits[i] << " ";
}
void QInt::printQInt()
{
	string result = "0";
	unsigned tmp;
	unsigned len = 0;
	unsigned j;
	bool isNegative = 0;
	bool flag = 0;
	bool* binArr;

	//check negative number and convert positive number
	if (_arrBits[0] & 0x80000000) //10..0,127 số 0
	{

		isNegative = 1;
		int i = ARRBITS_SIZE;
		//-1
		while (i > 0 && _arrBits[i - 1] == 0)
		{
			_arrBits[i - 1] = 0xFFFFFFFF;
			i--;
		}
		if (i > 0)
			_arrBits[i - 1] = _arrBits[i - 1] - 1;

		//bitwise
		for (i = 0; i < ARRBITS_SIZE; i++)
			_arrBits[i] = ~_arrBits[i];

	}


	for (int i = 0; i < ARRBITS_SIZE; i++)
	{
		if (_arrBits[i] == 0)
			continue;

		//convert arrBits[i] to binary
		tmp = _arrBits[i];
		j = ARRBITS_SIZE * 8;
		binArr = new bool[j] { 0 };
		while (tmp)
		{
			binArr[j - 1] = tmp % 2;
			tmp = tmp / 2;
			j--;
		}

		//convert arrBits[i] to number string
		for (int i = 0; i < ARRBITS_SIZE * 8; i++)
		{
			if (binArr[i] == 1 && flag == 0)
			{
				flag = 1;
				result = "1";
			}
			else if (flag == 1)
			{
				result = MultBy2(result);
				if (binArr[i] == 1)
				{
					unsigned len = result.length();
					while (len > 0 && result[len - 1] == '9')
					{
						result[len - 1] = '0';
						--len;
					}
					if (len == 0)
						result = '1' + result;
					else
						result[len - 1] += 1;
				}

			}
		}

		delete[]binArr;
	}

	if (isNegative)
		result = '-' + result;
	cout << result << endl;

}

string DiveBy2(string numStr)
{
	if (numStr == "0" || numStr == "1")
		return "0";

	string result = "";
	int i, j;
	i = 0;
	j = 0;
	if (numStr[0] == '1')
	{
		i = 1;
		j = 1;
	}
	while (j < numStr.length())
	{
		unsigned int tmp = i * 10 + numStr[j] - 48;
		i = tmp % 2 ? 1 : 0;
		result = result + char(tmp / 2 + 48);
		j++;
	}

	return result;
}
string MultBy2(string numStr)
{
	if (numStr == "0")
		return "0";

	string result = "";
	unsigned mis = 0;
	unsigned i;

	unsigned len = numStr.length();
	while (len)
	{
		i = (numStr[len - 1] - 48) * 2;
		result = char((i + mis) % 10 + 48) + result;
		mis = i / 10;
		len--;
	}
	if (mis)
	{
		result = '1' + result;
	}

	return result;
}