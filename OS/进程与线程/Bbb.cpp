#include<iostream>
#include <sys/types.h>
#include <unistd.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<pthread.h>             //在编译时注意加上-lpthread参数，以调用静态链接库。因为pthread并非Linux系统的默认库。
//#include<bits/stdc++.h>
int cal,recent;
using namespace std;

struct argu         //向线程传递多个参数时用结构体
{
  int re;
  int func;
};


pthread_mutex_t mutex_pause = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t cond_pause = PTHREAD_COND_INITIALIZER;  
bool pthread_pause = false; 

void pthread_suspend(void)  
{  
	if (pthread_pause == false)  
	{  
		pthread_mutex_lock( &mutex_pause );  
		pthread_pause = true;  
		printf("\n------transport pause------\n");  
		pthread_mutex_unlock( &mutex_pause );      
	}  
	else  
	{  
		printf("the transport suspend already/n");  
	}  
}  

void pthread_resume(void)  
{   
	if (pthread_pause == true)  
	{  
		pthread_mutex_lock(&mutex_pause);                  //这就是书上学过的，先加锁，防止同时写pthread_pause
		pthread_pause = false;  
		pthread_cond_broadcast(&cond_pause);               
		printf("------transport resume------/n");  
		pthread_mutex_unlock(&mutex_pause);   
	}  
	else  
	{  
		printf("transport resume already/n");  
	}  
} 

void* thread1(void* arg)           //线程1
{
  while（true）
  {
  sleep(5);                             //睡5秒，便于观察暂停操作
   if(pthread_pause ＝＝ false)
   {
    argu arg1;
    arg1=*(argu*)arg;
    int sum;
    printf("%d\n",arg1.func);

    if(arg1.func==4)
    printf("input wrong!\n");

    else if(arg1.func==2)
    printf("A and B exited!");

    else{

       for(int i=1;i<=arg1.re;i++)         //这里用int，暂时不考虑溢出
       {
     sum+=i;
       }
         printf("%d %d\n",sum,arg1.re);

       }
   }

   else
   {
     pthread_cond_wait(&cond_pause,&mutex_pause);
   }
  }
}

void* thread2(void* arg)        //线程2
{
 int func=0;
    cal=0;
string input=*(string*)arg;
int len;
bool num=true;

len=input.size();
for(int i=0;i<len;i++)
{
    int k=input[i]-'0';
    if((k>=0)&&(k<=9))
    {
         cal=cal*10+k;                             //cal是全局变量
    }
    else
    {
        num =false;
        break;
    }
}

if(input[0]=='p'&&len==1)                                  //根据输入x的不同，返回不同的func，便于调整线程1的动作
 func=1;
else if(input[0]=='e'&&len==1)     //输入e，退出
  func=2;
else if(num)                      //是正整数
  func=3;
else                                //非法输入
  func=4;
  //pthread_exit((void*)func);
  return (void*)(long)func;
}

int main()                                 //主线程
{
string input;
bool flag=true;//第一次输入
bool flag_pth1=true;//是否需要重新计算
argu arg1;//参数

while(true){

cin >> input;
pthread_t id1, id2;
int ret;
int func=0;

ret = pthread_create(&id2,NULL,thread2,(void*)&input);


if (ret != 0)                                     //安全考虑
	{
		printf("Create thread2 error!\n");
		exit(1);
	}

pthread_join(id2,(void **)&func);                   //线程2监控输入的x，返回func    //等待线程2结束
//printf("%d\n",func);                            //debug时使用，判断线程2有没有问题

arg1.re=cal;
arg1.func=func;

if(flag)
{
flag=false;            //第一次输入参数，没有“上一次”
recent=cal;
}
else
{
    if(cal==recent)                  //这次输入等于上一次，不需要重新计算
      flag_pth1=false;
    else
      flag_pth1=true;
recent=cal;
}

if(func==1)
{
pthread_suspend();
printf("pause!\n");
}

if(func==2)                 //线程1打印退出信息
{
pthread_resume();
ret = pthread_create(&id1,NULL,thread1,(void*)&arg1);
if (ret != 0)
	{
		printf("Create thread1 error!\n");
		exit(1);
	}
pthread_join(id1,NULL);  //等待线程1结束,打印退出信息。防止主线程先退出

return 0;       //exit;
}

if(func==3&&(!flag_pth1))
{
printf("输入相同，不需要重新计算\n");
}

if(func==3&&flag_pth1)  //重新计算 ， 第一次也可以看做另类的重新计算
{
ret = pthread_create(&id1,NULL,thread1,(void*)&arg1);
if (ret != 0)
	{
		printf("Create thread1 error!\n");
		exit(1);
	}
}

if(func==4)                 //线程1打印错误提示
{
  pthread_resume();
ret = pthread_create(&id1,NULL,thread1,(void*)&arg1);
if (ret != 0)
	{
		printf("Create thread1 error!\n");
		exit(1);
	}
}

}
return 0;
}


/*
pthread_mutex_t mutex_pause = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t cond_pause = PTHREAD_COND_INITIALIZER;  
   
bool pthread_pause = false;  

void pthread_suspend(void)  
{  
	if (pthread_pause == false)  
	{  
		pthread_mutex_lock( &mutex_pause );  
		pthread_pause = true;  
		printf("\n------transport pause------\n");  
		pthread_mutex_unlock( &mutex_pause );      
	}  
	else  
	{  
		printf("the transport suspend already/n");  
	}  
}  

void pthread_resume(void)  
{   
	if (pthread_pause == true)  
	{  
		pthread_mutex_lock(&mutex_pause);                  //这就是书上学过的，先加锁，防止同时写pthread_pause
		pthread_pause = false;  
		pthread_cond_broadcast(&cond_pause);  
		printf("------transport resume------/n");  
		pthread_mutex_unlock(&mutex_pause);   
	}  
	else  
	{  
		printf("transport resume already/n");  
	}  
} 

void* threadFunc(void* args)  //工作线程函数 
{    
    while(1)    
    {       
        if(pthread_pause ＝＝ false)       
        {     work();       
        }       
        else      
        {             
           pthread_cond_wait(&cond_pause,&mutex_pause);
        }    
     }    
     return ((void*)0); 
}   
*/