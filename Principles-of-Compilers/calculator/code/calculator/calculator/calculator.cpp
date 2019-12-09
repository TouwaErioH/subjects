// calculator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
calculator.cpp包含main程序，调用analyse.h中的analyse函数
myanalyse.h进行文件读取和判断有无syntax error和直接的divide by zero，并将文件按行调用myscanner.h处理
myscanner处理一行字符串，包含write，标识符声明，以及计算表达式并赋值功能。其中计算表达式会调用calc.h进行运算。
同时进行undefined标识符，标识符illegally defined等等错误类型判断
calc.h将中缀表达式转换为后缀并计算。同时进行int to float的cast error以及divide by zero检查
errorout.h包含格式化输出错误信息的函数
Fmystack.h包含所需栈结构以及栈相关函数
defs.h包含一些用到的标准库函数以及自定义的token信息

*/

#include <iostream>
#include "defs.h"
#include "myanalyse.h"
#include "clac.h"
using namespace std;

int main(int argc, char** argv)
{

	
	if (argc == 0) {
		cout << "lack of file！" << endl;
		system("pause");
	}
	else {
		analyse(argv[1]);
	}
	system("pause");
	
	//analyse("E:\\calculator\\calculator\\Debug\\test10.txt");
	return 0;
}

