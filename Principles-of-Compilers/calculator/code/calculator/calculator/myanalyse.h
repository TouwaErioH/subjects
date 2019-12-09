#pragma once
#include "defs.h"
#include "Fmystack.h"
#include "myscanner.h"
#include <fstream>
void analyse(string fPath) {
	ifstream sho(fPath, ios::in);//展示原代码
	{	string rline;
		int ln = 0;
		while (!sho.eof()) {
			ln++;
			getline(sho, rline);
			cout << "line " << ln << "  " << rline << endl;
		}

	}
	cout << endl<<endl;
	Token identis[100];   //假定标识符不超过100个   要改进的话可以使用vector
	int idennum = 0;
	string path = fPath;
	ifstream in(path, ios::in);   //cpp standard  in方式表示要读取文件,文件不存在的话,不建立文件,而是得到一个空的ifstream
	string line = "";
	int errl = 0, errPsn = 0;//错误所在的行和位置
	//按行读取文件
	while (!in.eof()) {
		getline(in, line);  //按行解析
		errl++;
		if (line != "") {//判断结尾是否是";"   syntax error
			if (line[line.length() - 1] != ';') {  // in case of write().
				if ((line[line.length() - 2] != ')') && (line[line.length() - 1] != '.'))
				{
					errPsn = line.length();
					cout << "Error(line " << errl << ",position " << errPsn << "): syntax error, lack of ';'" << endl;
					//system("pause");
				}
			}
			if (line.find("/0") != -1) {//判断除数是否为零 divide by zero,针对直接除0如 a/0 //在中缀求值过程中也有检测，针对除0变量
				errPsn = line.find("/0") + 1;
				cout << "Error(line " << errl << ",position " << errPsn << "): Divide by zero!" << endl;
				//system("pause");
			}
			
		}
		if (line!="")
		scanner(line, identis, &idennum, errl);
	}
	
	if (line[line.length() - 1] != '.')
		error("syntax error: ended without '.'!",errl);
	in.close();
}