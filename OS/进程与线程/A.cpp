#include<iostream>
#include <unistd.h>
#include<cstdio>
#include<sys/types.h>   //pid
#include<sys/wait.h> 
using namespace std;

int main()
{
    pid_t fpid;                                      
    fpid=fork();                   //fork()子进程
    if(fpid<0)
    {
        printf("error");
    }
    else if (fpid==0)
    {
        execlp("/home/erio/Desktop/B","B",NULL);   //子进程使用另一个执行体 “B”
    }
    else
    {
       printf("Hello,world!\n");          //主进程打印 Hello,world!
       wait(NULL);                        //等待子进程，防止僵尸进程
    } 
    return 0;
}