#include<iostream>
#include <sys/types.h>
#include <unistd.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<pthread.h>             //在gcc编译时注意加上-lpthread参数，以调用静态链接库。因为pthread并非Linux系统的默认库。
using namespace std;

int cal,recent,target;
int func;      //全局变量，减少线程间通信的麻烦
pthread_t id1, id2;

pthread_mutex_t mutex_pause = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t cond_pause = PTHREAD_COND_INITIALIZER;  
bool pthread_pause = false; 

void pthread_suspend(void)                         //暂停函数
{  
	if (pthread_pause == false)  
	{
		pthread_mutex_lock( &mutex_pause );
		pthread_pause = true;  
		printf("pause\n");                               //打印暂停信息
		pthread_mutex_unlock( &mutex_pause );
			  
	}  
	else  
	{  
		printf("suspend already\n");  
	}  
} 

void pthread_resume(void)                            //恢复函数
{   
	if (pthread_pause == true)  
	{  
		pthread_mutex_lock(&mutex_pause);                  //这就是书上学过的，先加锁，防止同时写pthread_pause
		pthread_pause = false;  
		pthread_cond_broadcast(&cond_pause);               
		printf("resume\n");                             //打印恢复信息
		pthread_mutex_unlock(&mutex_pause); 
	}  
	else  
	{  
		printf("resume already\n");  
	}  
}

void* thread1(void* arg)           //线程1
{
  while(true)
  {
  sleep(5);                             //睡5秒，便于观察暂停时,输入相同时，输入p，e时，输入错误字符时线程1的动作
   if(pthread_pause ==false)              // 这里的最外层if 和 else实现了线程的暂停            若没有暂停，进行累加，输出错误信息等操作
	  {
    int sum=0;
    //printf("%d\n",func);

    if(func==4)                          
    printf("input wrong!\n");         //根据线程2的监控，输入错误，输出错误信息

    else if(func==2)                  //根据线程2的监控，要求退出，输出退出信息
		{
    printf("A and B exited!\n");
		return (void*)0;
		}
    else{                             //进行累加操作

       for(int i=1;i<=target;i++)         //这里用int，暂时不考虑溢出
       {
           sum+=i;
       }
         printf("%d %d\n",sum,target);               //输出运算结果和输入x，便于检查

       }
   }

   else                                            //暂停的实现。利用pthread_cond_wait
   {
		 //printf("暂停中\n");
     pthread_cond_wait(&cond_pause,&mutex_pause);
   }
  }
}

void* thread2(void* arg)        //线程2，监控输入
{
bool flag=true;//第一次输入
bool flag_pth1=true;//是否需要重新计算
string input;                            //因为没有限制输入长度等信息，故采用string
    cal=0;
while(true)
{
	cal=0;          //初始化             cal是全局变量
cin>>input;
int len;
bool num=true;

len=input.size();      
for(int i=0;i<len;i++)                         //处理输入
{
    int k=input[i]-'0';
    if((k>=0)&&(k<=9))                            //若为非负整数
    {
         cal=cal*10+k;                             
    }
    else                                      //不满足if((k>=0)&&(k<=9)) ，说明是字符或负数等
    {
        num =false;
        break;
    }
}                        

//根据输入x的不同，有不同的func，便于调整线程1的动作

if(input[0]=='p'&&len==1)                                  //输入p，暂停
	 func=1;
else if(input[0]=='e'&&len==1)     //输入e，退出
		func=2;
else if(num)                      //是正整数
  func=3;
else                                //非法输入
  func=4;

if(flag)
{
flag=false;            //第一次输入参数，没有“上一次”
recent=cal;                                   //recent记录最近输入的数字,便于和这次比较，若相同，无需重新计算
target=cal;                                   //target为线程1累加的目标
}
else
{
    if(cal==recent)                  //这次输入等于上一次，不需要重新计算
      {
				flag_pth1=false;
			  target=recent;
			}
    else                           //不同，重新计算
      {
				flag_pth1=true;
				target=cal;
			}
recent=cal;
}


if(func==1)                                 //func==1，暂停线程1
{
	if (pthread_pause == false)  
	pthread_suspend();                                       
}
if(func==2)                                  //func==2，说明监控到输入e，准备退出
{
	if(pthread_pause ==true)     
	pthread_resume();

  pthread_join(id1,NULL);   //等待线程1结束           先要线程1打印退出信息，线程2再退出 
	return (void*)0;
}

if(func==3&&(!flag_pth1))                     //func==3，说明输入非负整数；(!flag_pth1)，说明输入和上次相同，不需要重新计算
printf("输入相同，不需要重新计算\n");

if(func==3&&flag_pth1)                           //重新计算 ， 第一次也可以看做另类的重新计算
 {
	if(pthread_pause ==true)
  pthread_resume();
 }

if(func==4)                                  //监控到输入错误的字符。线程1打印错误提示
{
  if(pthread_pause ==true)
  pthread_resume();
}
}
}

int main()                                 //主线程
{
int ret;

ret = pthread_create(&id2,NULL,thread2,NULL);      //创建线程2
if (ret != 0)                                     //安全考虑，创建失败则退出
	{
		printf("Create thread2 error!\n");
		exit(1);
	}

ret = pthread_create(&id1,NULL,thread1,NULL);       //创建线程1
if (ret != 0)                                      //安全考虑，创建失败则退出
	{
		printf("Create thread1 error!\n");
		exit(1);
	}

pthread_join(id2,NULL);                           //等待线程2结束,防止主线程先退出
return 0;  
}
