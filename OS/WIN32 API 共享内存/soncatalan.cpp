#include<windows.h>
#include<cstdio>
#include<vector>
using namespace std;
#include<string>
#include<fstream>
#include<iostream>
#define MAX 20
#include<sstream>
int main(int argc,char* argv[]){

HANDLE hMapFile;
LPVOID lpMapAddress;

//文件映射
hMapFile=OpenFileMapping(FILE_MAP_ALL_ACCESS,false,TEXT("SharedObject"));

lpMapAddress=MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,1024*1024);

/*ifstream data((char*)lpMapAddress);   //想多了直接试一下%s输出就挺好的。
        vector<int> res;
        string line;
        while(getline(data,line))
       {
        stringstream ss; //输入流
        ss << line; //向流中传值
        if (!ss.eof()) {
            int temp;
            while (ss >> temp) //提取int数据
                res.push_back(temp); //保存到vector
          }
       }
for(int i=0;i<res.size();i++)
{
    cout<<res[i]<<endl;
}*/
printf("%s",lpMapAddress);
cout<<"son finished!"<<endl;
Sleep(1000000);

UnmapViewOfFile(lpMapAddress);
CloseHandle(hMapFile);
return 0;
}
