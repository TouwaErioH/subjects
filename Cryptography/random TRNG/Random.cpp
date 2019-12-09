#include <conio.h>
#include <wtypes.h>
#include <iostream>
#include <fstream>
#include<string>
#include <cstdio>
#include<cstdlib>

using namespace std;

ofstream rfile;
int cnt0 = 0, cnt1 = 0; //统计最终生成的0,1个数

class SHA1 {  //对message散列得160位，取5位（32,64,96,128,160）随机数，送入最终结果的文件。
public:
	void creat_w(unsigned char input[64], unsigned long w[80])
	{
		int i, j; unsigned long temp, temp1;
		for (i = 0; i < 16; i++)
		{
			j = 4 * i;
			w[i] = ((long)input[j]) << 24 | ((long)input[1 + j]) << 16 | ((long)input[2 + j]) << 8 | ((long)input[3 + j]) << 0;

		}
		for (i = 16; i < 80; i++)
		{
			w[i] = w[i - 16] ^ w[i - 14] ^ w[i - 8] ^ w[i - 3];
			temp = w[i] << 1;
			temp1 = w[i] >> 31;
			w[i] = temp | temp1;

		}
	}
	char ms_len(long a, char intput[64])
	{
		unsigned long temp3, p1;  int i, j;
		temp3 = 0;
		p1 = ~(~temp3 << 8);
		for (i = 0; i < 4; i++)
		{
			j = 8 * i;
			intput[63 - i] = (char)((a & (p1 << j)) >> j);

		}
		return '0';
	}
	void begin(unsigned long in)
	{
		unsigned long H0 = 0x67452301, H1 = 0xefcdab89, H2 = 0x98badcfe, H3 = 0x10325476, H4 = 0xc3d2e1f0;
		unsigned long A, B, C, D, E, temp, temp1, temp2, temp3, k, f; int i, flag; unsigned long w[80];
		unsigned char input[64]; long x; int n;
		input[0] = in >> 24;
		input[1] = in >> 16;
		input[2] = in >> 8;
		input[3] = in;
		n = strlen((LPSTR)input);
		if (n < 57)
		{
			x = n * 8;
			ms_len(x, (char*)input);
			if (n == 56)
				for (i = n; i < 60; i++)
					input[i] = 0;
			else
			{
				input[n] = 128;
				for (i = n + 1; i < 60; i++)
					input[i] = 0;
			}

		}

		creat_w(input, w);
		A = H0; B = H1; C = H2; D = H3; E = H4;
		for (i = 0; i < 80; i++)
		{
			flag = i / 20;
			switch (flag)
			{
			case 0: k = 0x5a827999; f = (B & C) | (~B & D); break;
			case 1: k = 0x6ed9eba1; f = B ^ C ^ D; break;
			case 2: k = 0x8f1bbcdc; f = (B & C) | (B & D) | (C & D); break;
			case 3: k = 0xca62c1d6; f = B ^ C ^ D; break;
			}
			temp1 = A << 5;
			temp2 = A >> 27;
			temp3 = temp1 | temp2;
			temp = temp3 + f + E + w[i] + k;
			E = D;
			D = C;

			temp1 = B << 30;
			temp2 = B >> 2;
			C = temp1 | temp2;
			B = A;
			A = temp;
		}
		H0 = H0 + A;
		H1 = H1 + B;
		H2 = H2 + C;
		H3 = H3 + D;
		H4 = H4 + E;
		writ(H0);
		writ(H1);
		writ(H2);
		writ(H3);
		writ(H4);
	}

	void writ(unsigned long tp) {
		int tmp = 0;
		string a;
		tmp = tp % 2;
		if (tmp == 0)
		{
			a = "0"; cnt0++;
		}
		else
		{
			a = "1"; cnt1++;
		}
		rfile << a;
	}
};



class MT {  //梅森旋转，得32位无符号整型，然后送入SHA-1。
public:
	int index;
	unsigned long MT[624];  //624 * 32 - 31 = 19937
	void initial(unsigned long seed)
	{
		//printf("seed:%d\n", seed);
		index = 0;
		MT[0] = seed;
		//对数组的其它元素进行初始化
		for (int i = 1; i < 624; i++)
		{
			unsigned long t = 1812433253 * (MT[i - 1] ^ (MT[i - 1] >> 30)) + i;
			MT[i] = t & 0xffffffff;   //取最后的32位赋给MT[i]
		}
	}

	void twist() //遍历旋转链，进行旋转算法
	{
		for (int i = 0; i < 624; i++)
		{
			// 2^31 = 0x80000000
			// 2^31-1 = 0x7fffffff
			unsigned long y = (MT[i] & 0x80000000) + (MT[(i + 1) % 624] & 0x7fffffff);
			MT[i] = MT[(i + 397) % 624] ^ (y >> 1);
			if (y & 1)
				MT[i] ^= 2567483615;
		}
	}

	unsigned long Rand()
	{
		if (index == 0)
			twist();
		unsigned long y = MT[index];
		y = y ^ (y >> 11);                 //y右移11个bit
		y = y ^ ((y << 7) & 2636928640);   //y左移7个bit与2636928640相与，再与y进行异或
		y = y ^ ((y << 15) & 4022730752);  //y左移15个bit与4022730752相与，再与y进行异或
		y = y ^ (y >> 18);                 //y右移18个bit再与y进行异或
		index = (index + 1) % 624;
		return y;
	}

	void begin(unsigned long seed) 
	{

		//rfile.open("C:\\Users\\lenovo\\Desktop\\rands.txt", ios::out | ios::app);
		//if (!rfile.is_open()) return ;

		initial(seed);  //初始化梅森旋转链
		int cnt = 0;
		unsigned long ret = 0;
		string a;
		ret = Rand();
		ret = Rand();
		SHA1 sha;
		sha.begin(ret);   //使用SHA-1签名，160位取5位，得5位随机数
	}
};

unsigned long xorshf96(unsigned long x, unsigned long y, unsigned long z) { //period 2^96-1 。得32位无符号整型
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;
	return z;
}


int main()
{
	string dir = "C:\\Users\\lenovo\\Desktop\\random.txt";
	rfile.open(dir, ios::out | ios::app | ios::binary);
	bool flag = 0;
	unsigned long result;
	unsigned long result1 = 0, result2 = 0, result3 = 0;
	MT mt1;
	int aa = 0, bb = 0;
	long long count = 0;
	int count1 = 0;  
	for (int i = 0; i <2000000 ; i++) {   //每次生成5个二进制随机数。10 000 000 /5 = 2 000 000次循环。

		// xorshift
		__asm {
			rdseed eax
			mov result, eax
		}
		/*if (result % 2 == 0)  //测试得0,1接近1:1
			aa++;
		else
			bb++;*/
		result1 = result;
		__asm {
			rdseed eax
			mov result, eax
		}
		result2 = result;
		__asm {
			rdseed eax
			mov result, eax
		}
		result3 = result;
		result = xorshf96(result1, result2, result3);
		/*if (result % 2 == 0)  //测试得0,1接近1:1
			aa++;
		else
			bb++;*/
		mt1.begin(result);

	}
	cout << cnt1 << "  " << cnt0 << endl;
	system("pause");
	return 0;
}



/*
	__asm {
		mov eax, 1
		cpuid
		and eax, 0x20000000 //30th bit
		test eax, 0
		jnz L
		mov flag, 1
		L:
	}
	if (!flag) {
		cout << "Not support this CPU!";
		return 0;
	}
	*/





