测试文件test.txt中要求identifier与'='，expression间隔一空格，形如 a = 123;

同时要求每行最后没有多余字符，如空格;，即在 ' ; '或 ' . ' 后换行

同时+-*/符号要求为英文符号

float a;
int b;
a = (10.44*356+1.28) / 2 + 1024 * 1.6
c = 6;
b = a * 2 - a/2;
float e;
int f;
f = 0;
e = f;
e = k*5;
int 9g;
int p;
int h;
h = 1/0;
p = 1/f.
write(j);
write(b);
write(a);

第三行 没有用  ;  结束 
第四行c=6，c是未声明的标识符
第九行e = f; e为float型，f为int型，不可以将int转换为float
第十行 e=k*5 ，k为未声明的标识符
第十一行声明 int 9g ，9g不符合标识符规则
第14行 除0
第15行 1/f，f=0，出现除0
第16行write(j)，j未声明
正确输出b，a的值并换行
最后以 ; 结束没有以  .  结束

报错与输出为
Error(line 3,position 37): syntax error, lack of ';'
Error(line  4 ):  assign to undefined identifier
Error(line  9 ):  cast error:int to float
Error(line  10 ):  undefined identifier in expression
Error(line  11 ):  illegally defined int identifier
Error(line 14,position 6): Divide by zero!
Error(line  14 ):  divide by zero
Error(line  15 ):  divide by zero
Error(line  16 ):  undefined identifier in write
b = 4324
a = 2882.96
Error(line  18 ):  syntax error: ended without '.'!
