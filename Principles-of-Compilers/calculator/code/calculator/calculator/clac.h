#pragma once
#include "defs.h"
#include "Fmystack.h"



/**
 * scan: 扫描字符串，获取下一个token
 * cal: 计算给定表达式的结果
 * cal_token: 计算两个token二元运算的结果，sign为运算符，结果输出到result参数里
 * get_op_level:获取操作符运算优先级
 */

int scan(const char* expr, int start, Token* current);
void cal(const char* expr,Token* aim,int errl);
void cal_token(Token* t1, Token* t2, int sign, Token* result,int errl,int*flag);//当出现divide by 0时flag=1，直接结束表达式计算
int get_op_level(int op,int errl);
void calce(string expr, Token* rst, int errl);

void calce(string expr,Token* rst,int errl) {


	//string to char*

	//给出的参考代码是char*的中缀表达式，暂时这么用，有时间调一下
	string tmp = expr;
	tmp = tmp.substr(0, tmp.length() - 1);
	//char* ccc = tmp.c_str(); //c_str()返回的是一个临时指针，不能对其进行操作。
	char c[100];
	strcpy_s(c, tmp.c_str());
	cal(c, rst,errl);

}



int get_op_level(int op,int errl) {
	switch (op)
	{
	case '(':
		return 1;
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	case ')':
		return 4;
	default:
		error("bad operator",errl);
		return 0;
	}
}

void cal(const char* expr,Token * aim,int errl) {
	int len = strlen(expr);
	int location = 0; // 下标
	Token current;
	current.flag = 0;
	current.key = "";
	Stack post;
	Stack ops;

	current.tag = START;

	init_stack(&post);
	init_stack(&ops);
	while (current.tag != END)
	{
		location = scan(expr, location, &current);
		switch (current.tag)
		{
		case START:
			//unexpected case
			error("scan error: find no token",errl);
			break;
		case END:
			break;
		case INT:
		case FLOAT:
			push(&post, current);
			break;
		case SIGN: {
			int sign = current.value.sign;
			switch (sign)
			{
			case LP:
				push(&ops,current);
				break;
			case RP:
				do
				{
					Token* op;
					if (ops.size == 0)
					{
						error("parentheses mismatch",errl);
					}
					op = pop(&ops);
					push(&post, *op);

				} while (top(&ops)->value.sign != LP);
				pop(&ops);
				break;
			case PLUS:
			case MINUS:
			case MUL:
			case DIV: {
				if (ops.size == 0)
				{
					push(&ops, current);
					break;
				}
				int top_op = top(&ops)->value.sign;
				if (get_op_level(sign,errl) > get_op_level(top_op,errl))
				{
					push(&ops, current);
					break;
				}
				else {
					while (ops.size != 0)
					{
						int top_op = top(&ops)->value.sign;
						Token* op;
						if (get_op_level(sign,errl) <= get_op_level(top_op,errl))
						{
							op = pop(&ops);
							push(&post, *op);
						}
						else {
							break;
						}
					}
					push(&ops, current);
					break;
				}
			}
			default:
				break;
			}
			break;
		}
		default:
			//unexpected case
			error("scan error: find unexpected token",errl);
			break;
		}
	}

	
	while (ops.size != 0)
	{
		Token* op = pop(&ops);
		push(&post, *op);
	}


	free_stack(&ops);
	// 转换的后缀里的元素是相反的，所以需要反转栈中数据
	inverse(&post);
	// 计算后缀表达式
	Stack tmp;
	init_stack(&tmp);

	while (post.size != 0) {
		int tag = top(&post)->tag;
		switch (tag)
		{
		case INT:
		case FLOAT:
			push(&tmp, *pop(&post));
			break;
		case SIGN: {
			Token t;
			int sign = pop(&post)->value.sign;
			switch (sign) {
			case '+':
			case '-':
			case '*':
			case '/': {
				Token* value2 = pop(&tmp);
				Token* value1 = pop(&tmp);
				int flag = 0;
				cal_token(value1, value2, sign, &t,errl,&flag);
				if (flag == 1)
				{
					return; //直接结束表达式计算
				}
				push(&tmp, t);
				break;
			}
			}
			break;
		}
		default:
			error("wrong in postfix expression",errl);
			break;
		}
	}
	if (tmp.size != 1) {
		printf("%d\n", tmp.size);
		error("bad expression",errl);
	}

	// 计算的结果
	Token* cal_result = pop(&tmp);
	if (cal_result->tag == INT)
	{
		if (aim->tag == FLOAT)
			error("cast error:int to float",errl);  //类型转换错误
		else
		{
			aim->value.i_val = cal_result->value.i_val;
			aim->flag = 1;						//flag=1,已赋值
		}
	}
	else{
		if (aim->tag == FLOAT) {
			aim->value.f_val = cal_result->value.f_val;
			aim->flag = 1;
		}
		else
		{
			aim->value.i_val = cal_result->value.f_val;
			aim->flag = 1;						//flag=1,已赋值
		}
	}

}

int scan(const char* expr, int start, Token* current) {
	int len = strlen(expr);
	int offset = start;
	char curr_char = expr[offset];
	while (offset < len)
	{
		// 跳过空格
		if (curr_char == ' ')
		{
			curr_char = expr[++offset];
			continue;
		}

		if (curr_char == '\0')
		{
			current->tag = END;
			return offset + 1;
		}

		// 解析计算符
		if (curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/'
			|| curr_char == '(' || curr_char == ')')
		{
			current->tag = SIGN;
			current->value.sign = curr_char;
			return offset + 1;
		}

		// 解析数字
		if (curr_char >= '0' && curr_char <= '9')
		{
			int is_float = 0;
			int int_val = 0;
			float float_val;

			do
			{
				int_val = int_val * 10 + (curr_char - '0');
				curr_char = expr[++offset];
			} while (curr_char >= '0' && curr_char <= '9');

			// 如果有小数点，则为float
			if (curr_char == '.')
			{
				float_val = int_val;
				curr_char = expr[++offset];
				float times = 10.0f;

				int overflow = 0;
				while (curr_char >= '0' && curr_char <= '9') {
					if (overflow)
					{
						curr_char = expr[++offset];
						continue;
					}

					float_val += (curr_char - '0') / times;
					curr_char = expr[++offset];
					times *= 10;
					// 防止溢出
					if (times > 0xffffffffu / 10)
					{
						overflow = 1;
					}
				}

				current->tag = FLOAT;
				current->value.f_val = float_val;
				return offset;
			}

			current->tag = INT;
			current->value.i_val = int_val;
			return offset;
		}

	}
	if (curr_char == '\0')
	{
		current->tag = END;
		return offset;
	}
}


void cal_token(Token* t1, Token* t2, int sign, Token* result,int errl,int*flag) {
	if (t1->tag == FLOAT || t2->tag == FLOAT)
	{
		result->tag = FLOAT;
		float v1, v2;
		v1 = t1->tag == FLOAT ? t1->value.f_val : t1->value.i_val;
		v2 = t2->tag == FLOAT ? t2->value.f_val : t2->value.i_val;

		switch (sign)
		{
		case '+':
			result->value.f_val = v1 + v2;
			return;
		case '-':
			result->value.f_val = v1 - v2;
			return;
		case '*':
			result->value.f_val = v1 * v2;
			return;
		case '/':
			if (v2 == 0.0f)
			{
				error("divide by zero",errl);
				*flag = 1;    //直接返回，结束表达式计算
				return;
			}
			result->value.f_val = v1 / v2;
			return;
		default:
			return;
		}
	}
	else {
		result->tag = INT;
		int v1, v2;
		v1 = t1->value.i_val;
		v2 = t2->value.i_val;
		switch (sign)
		{
		case '+':
			result->value.i_val = v1 + v2;
			return;
		case '-':
			result->value.i_val = v1 - v2;
			return;
		case '*':
			result->value.i_val = v1 * v2;
			return;
		case '/':
			if (v2 == 0)
			{
				error("divide by zero",errl);
				*flag = 1;
				return;
			}
			result->value.i_val = v1 / v2;
			return;
		default:
			return;
		}
	}
}

