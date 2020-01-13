#include<cstring>
#include<pthread.h>
#include<cstdio>
#include<unistd.h>
#include<stdlib.h>//atoi
#include <sys/ipc.h> //shm
#include <sys/shm.h> //shm
#include<sys/types.h>
#include<sys/stat.h> //S_IRUSR | S_IWUSR
#include<sys/wait.h>
#include<semaphore.h>
using namespace std;

int readcount,writecount;         
pthread_t id1,id0;  
sem_t r,w,mutexreadcount,mutexwritecount,mutexpriority;     


void* reader(void* arg)          
{
   int i=*(int*)arg;
   printf("process %d want to read\n",i);
   sem_wait(&mutexpriority);
   sem_wait(&r);                               //先P(r)才能增加readcount，这样后来的读者不能P(r)，不会阻碍write的释放。
   sem_wait(&mutexreadcount);                  //之前的读者运行完释放write   读写互斥
   readcount++;
   if(readcount==1) sem_wait(&w);
   sem_post(&mutexreadcount);
   sem_post(&r);
   sem_post(&mutexpriority);             //在优先锁释放之前，若来了写者，读者，写者会P(r)而读者因为优先锁没有释放不能P(r)。同时写者P(r)，阻碍后续的读者增加readcount
   //reading;                           //读者无需互斥
   printf("process %d is reading\n",i);
   sleep(1);
      printf("process %d complete reading\n",i);
   sem_wait(&mutexreadcount);
   readcount--;
   if(readcount==0) sem_post(&w); //之前的读者运行完释放write   读写互斥
   sem_post(&mutexreadcount);
}

void* writer(void* arg)          
{
    int i=*(int*)arg;
    printf("process %d want to write\n",i);
    sem_wait(&mutexwritecount);
    writecount++;
    if(writecount==1) sem_wait(&r);
    sem_post(&mutexwritecount);
    sem_wait(&w);                    //写者互斥
    //write
    printf("process %d is writing\n",i);
    sleep(4);
    printf("process %d complete writing\n",i);
    sem_post(&w);
    sem_wait(&mutexwritecount);
    writecount--;
    if(writecount==0) sem_post(&r);               //所有的写者写完才能让后续P(r)了的读者增加readcount
    sem_post(&mutexwritecount);
}


int main(int argc, char*argv[])                                
{
int ret;    
sem_init(&r,0,1);  sem_init(&w,0,1); sem_init(&mutexpriority,0,1); sem_init(&mutexreadcount,0,1); sem_init(&mutexwritecount,0,1);
readcount=0;writecount=0;

if(argc!=2)
{
    fprintf(stderr,"argu wrong!\n");
    return -1;
}
int size=atoi(argv[1]);                           //从命令行输入先后共几个进程
printf("假设总共%d个进程先后来到\n",size);
int num[size+1];

for(int i=1;i<=size;i++)
{
  int slp=rand()%2+1;
  sleep(slp);
  num[i]=i;

  int flag=rand()%2;   //flag==0,write; flag==1,read;
  if(flag)
  {
    ret = pthread_create(&id0,NULL,reader,&num[i]);    
    if (ret != 0)                                    
	{
		printf("Create reader error!\n");
		exit(1);
	}
  }

  else{
       ret = pthread_create(&id1,NULL,writer,&num[i]);   
   if (ret != 0)                                    
	{
		printf("Create writer error!\n");
		exit(1);
	}   
  }
}
pthread_join(id1,NULL);
pthread_join(id0,NULL);
sleep(20);
return 0;  
}