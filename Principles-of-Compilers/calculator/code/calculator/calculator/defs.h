#pragma once

#include<string.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
 //不加的话，string 会报 未知重写说明符
using namespace std;
/*
	SIGN: 计算符
*/
// 0-4
enum Tag {
	START, END, SIGN, INT, FLOAT   
};

/**
 * 加减乘除
 * LP: 左括号（Left parenthesis）
 * RP: 右括号（Right parenthesis）
 */
enum Sign {
	PLUS = '+', MINUS = '-', MUL = '*', DIV = '/', LP = '(', RP = ')'
};

/**
 * token相关数据结构和函数
 * tag表示token类型
 * value表示具体值
 */
typedef struct _Token
{
	int tag;
	string key;
	int flag;
	union Value
	{
		int sign;
		int i_val;
		float f_val;
	} value;

}Token;