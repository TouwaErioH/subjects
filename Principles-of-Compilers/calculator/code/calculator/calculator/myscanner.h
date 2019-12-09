#pragma once
#include "defs.h"
#include "errorout.h"
#include "Fmystack.h"
#include "clac.h"

using namespace std;

// iden array of defined identifiers; idennum: number of defined identifiers; errl: line number of prog
void scanner(string prog,Token* iden,int* idennum,int errl)// prog: line being processed
{
	int p = 0;
	char ch = prog[p];
	Token token;
	token.key = "";

	int n = prog.find('=');

	//没有'=',那么只能是声明如int a; 或输出 write(a)  假定不存在其他情况如a； int；题目要求先声明后赋值
	if (n == -1) {
		if (prog[0] == 'w')   //write
		{
			int i = prog.find('(');
			i += 1;
			char c = prog[i];
			token.key = "";
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token.key += c;
					i++;
					c = prog[i];
				}
				int j = 0;
				for (j = 0; j < *idennum; j++)
				{
					if (token.key == iden[j].key)
					{
						break;
					}


				}
				if (j == *idennum)
				{
					error("undefined identifier in write",errl);   //write了一个合语法但不存在的标识符，或write的对象不和语法，如 write(a*b)
					//system("pause");                               //暂不考虑write(a b)且a是已声明并初始化的情况
					//exit(1);
				}

				/*
				else if ((iden[j].tag == INT)&& iden[j].flag==1)//flag==1，已赋值
				{
					printf("%s = %d\n",iden[j].key, iden[j].value.i_val);
				}
				else if ((iden[j].tag == FLOAT )&& iden[j].flag == 1) {
					printf("%s = %.2f\n", iden[j].key, iden[j].value.f_val);
				}
				*/
				else if ((iden[j].tag == FLOAT) && iden[j].flag == 1)
				{
					cout << iden[j].key << " = " << iden[j].value.f_val << endl;
				}
				else if ((iden[j].tag == INT) && iden[j].flag == 1)
				{
					cout << iden[j].key << " = " << iden[j].value.i_val << endl;
				}
				else {
					error("Uninitialized identifier in write",errl);  //标识符已声明但未初始化
				}

			}

			else
			{
				error("undefined and illegal identifier in write",errl);   //write了一个不合法的标识符
			}

		}

		else if (prog[0] == 'i') { //initial int
			//int i = prog.find(' ');
			int i = 4;            // 假定文件中声明int 变量时从0列开始，那么变量名即从4列开始
			char c = prog[i];
			token.key = "";
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token.key += c;
					i++;
					c = prog[i];
				}
				if (c != ';')
				{
					error("illegally defined int identifier",errl); //不规则的命名，中间有其他字符
				}

			}
			else
			{
				error("illegally defined int identifier",errl);  //不规则的命名，开始不是字母
			}
			token.tag = INT;  //注意此时该标识符仍未赋值，只是声明
			token.flag = 0; //undefined
			iden[*idennum] = token;
			*idennum = *idennum + 1;
		}
		//initial float  suppose like: int a;
		else if (prog[0] == 'f') 
		{
			//int i = prog.find(' ');
			int i = 6;
			char c = prog[i];
			token.key = "";
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token.key += c;
					i++;
					c = prog[i];
				}
				if (c != ';')
				{
					error("illegally defined float identifier", errl); //不规则的命名，中间有其他字符  
				}

			}
			else
			{
				error("illegally defined float identifier", errl);  //不规则的命名，开始不是字母
			}
			token.tag = FLOAT;  //注意此时该标识符仍未赋值，只是声明
			token.flag = 0; //undefined
			iden[*idennum] = token;
			*idennum = *idennum + 1;
		}

	}

	//有'='，赋值；即a= expr;的形式
	else {
		//确定被赋值的标识符
		int i = 0;
		char c = prog[i];
		token.key = "";
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				token.key += c;
				i++;
				c = prog[i];
			}
			if (c != ' ')  //认为标准格式为 a = expr,=号前有' '
			{
				error("assign to illegal identifier", errl); //被赋值的标识符有问题。比如a+b = expr; 未考虑 a b = expr 的问题
				return;
			}

			int j = 0;
			for (j = 0; j < *idennum; j++)
			{
				if (token.key == iden[j].key)
				{
					break;
				}


			}
			if (j == *idennum)
			{
				error("assign to undefined identifier",errl);   //待后续处理。标识符语法合法但是undefined
				return;
			}

			//正确找到了被赋值的标识符
			 //调用calc计算expr的值，赋给iden[j];
			//思路：先替换掉expr中的标识符;比如b=a*2，a=2；先把a换成2，在字符串中很容易这样操作；
			//若此时发现a未赋值，报错即可
			
			//替换标识符部分 这里我们已经假定'='后的expr以;或.结束 并且输入格式为 a = exp，即=号后还有一个空格

			string raw = prog.substr(n+2);
			
			//计算并赋值
				string expr = "";  //expr存储转换后的表达式
				int k = 0;
				int start, end; //start-end 区间需要被替换
				start = 0;
				int p = 0;
				//while ((raw[k] != ':') && (raw[k] != '.')&&(k<raw.length())  // out of range
				while (k < raw.length()) 
				{
					char c = raw[k];
					string cp = "";          //cp 待替换的标识符
					if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
						end = k;
						while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
							cp += c;
							k++;
							c = raw[k];
						}
						expr = expr + raw.substr(start, end - start);
						start = k;
						for (p = 0; p < *idennum; p++)
						{
							if (cp == iden[p].key)
							{
								break;
							}
						}
						if (p == *idennum)
						{
							error("undefined identifier in expression",errl);   //expr中存在合语法但未定义的标识符 
												//我们现在只考虑expr中只出了这个问题，其他的表达式不规范比如1++2暂不考虑
												 //而 b_d+3 这种情况，b是合法字符但整体不合法的情况也暂时不考虑
							return;
						}

						if (iden[p].flag == 0)
						{
							error("uninitialized identifier in expression", errl);
							return;
						}
						if(iden[p].tag==FLOAT)
							expr = expr + to_string(iden[p].value.f_val);
						else
							expr = expr + to_string(iden[p].value.i_val);
						/*if ((iden[p].tag == INT) && (iden[j].tag == FLOAT))
						{
							error("cast error: int to float",errl);  //类型转换int to float 在要求中不允许
						}*/
					}
					else
					{
						k++;
					}

				}
				expr = expr + raw.substr(start, raw.length() - start); //去掉最后的;
				//cout <<"final" <<expr<< endl;
				Token* rst = &iden[j]; //目标标识符的token
				calce(expr,rst,errl);      //计算并赋值
		}
		else
		{
			error("assign to illegal identifier", errl);//被赋值的标识符开头不是字母，同时undefined
		}
	}

}


