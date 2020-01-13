#include<windows.h>
#include<cstdio>
#include<vector>
#include<string>
#include<cstdlib>
#include<iostream>
using namespace std;
#define MAX 20

int main(int argc,char*argv[]){

int sizek; vector<int>catalan; //这里用int，所以数据不能很大，不然会爆掉
catalan.push_back(1);     //第一个Catalan数为1
int temp=0;

if(argc!=2)
{
    fprintf(stderr,"argu wrong!\n");
    return -1;
}
sizek=atoi(argv[1]);
if(sizek>MAX)
{
    fprintf(stderr,"size should be less than %d\n",MAX);
    return -1;
}
if(sizek<1)
{
    fprintf(stderr,"size should be more than %d\n",0);
    return -1;
}

for(int i=0;i<sizek-1;i++)
{
  int j=i+3;
  temp=catalan[i]*(4*j-6)/j;
  catalan.push_back(temp);
}

HANDLE hFile,hMapFile;
LPVOID lpMapAddress;

//handle
hFile=CreateFile("C:\\Users\\lenovo\\Desktop\\a.txt",
GENERIC_READ | GENERIC_WRITE,
0, //	要求独占对设备的访问。如果设备已经打开，CreateFile 调用会失败；如果成功地打开了设备，后续的 CreateFile 调用会失败
NULL, //指向一个 SECURITY_ATTRIBUTES 结构的指针，定义了文件的安全特性（如果操作系统支持的话）
OPEN_ALWAYS,  //告诉CreateFile打开一个已有的文件，如果文件存在，那么CreateFile会直接打开文件，如果不存在，则会创建一个新文件
FILE_ATTRIBUTE_NORMAL, //文件没有其他属性,比如隐藏
NULL); //用于复制文件句柄


if(hMapFile == NULL){
		cout << "wrong in CreateFile" << endl;
		return 0;
	}

//文件映射
hMapFile=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,1024*1024,TEXT("SharedObject"));  //0,0会报错;   书上0,0是因为如果已经存在文件，那么会映射真实大小
                                                                         //两个数字分别为文件高位大小，低位大小
int rst = GetLastError();                                                
    if (hMapFile == NULL || rst == ERROR_ALREADY_EXISTS)
    {
        cout<<rst<<endl;   //0,0 rst为1006 我tou
        printf("CreateFileMapping error\n");
        return 0;
    }

    //视图。视图可以是映射文件的一部分，也可以是全部
lpMapAddress=MapViewOfFile(hMapFile,
FILE_MAP_ALL_ACCESS, // 等价于CreateFileMapping的 FILE_MAP_WRITE|FILE_MAP_READ)
0,0,1024*1024);         //0,0表示文件映射起始偏移的高32位.低32位

if(lpMapAddress == NULL){
		cout << "wrong in MapViewOfFile" << endl;
		return 0;
	}

string s;
char a[20];
int j=0;
for(int i=0;i<sizek;i++)
{
int t=sprintf((char*)lpMapAddress+j,"%d\r\n",catalan[i]);  //要转换  sprintf返回写入buffer 的字符数
j+=t;                                                      //注意\r\n  在MFC界面显示中要用“\r\n”。
}
//printf("%s",lpMapAddress);

STARTUPINFO si;
PROCESS_INFORMATION pi;
ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));

TCHAR szCommandLine[] = TEXT("C:\\Users\\lenovo\\Desktop\\soncatalan.exe");

CreateProcess(NULL,           // No module name (use command line)
                  szCommandLine,  // Command line
                  NULL,           // Process handle not inheritable
                  NULL,           // Thread handle not inheritable
                  FALSE,          // Set handle inheritance to FALSE
                  0,              // No creation flags
                  NULL,           // Use parent's environment block
                  NULL,           // Use parent's starting directory
                  &si,            // Pointer to STARTUPINFO structure
                  &pi);            // Pointer to PROCESS_INFORMATION structure



Sleep(1000000);                       //S大写
UnmapViewOfFile(lpMapAddress);
CloseHandle(hFile);
CloseHandle(hMapFile);
return 0;
}
