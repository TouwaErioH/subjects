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

int readcount;       
pthread_t id1,id0;  
sem_t w,mutexreadcount,que;


void* reader(void* arg)          
{
   int i=*(int*)arg;
   printf("process %d want to read\n",i);

   sem_wait(&que);                        //见writer。增加que
   sem_wait(&mutexreadcount);             //先来的writer写完才释放que，这样后面先申请的无论是写者还是读者，都是按申请顺序来。
   readcount++;
   if(readcount==1) sem_wait(&w);                //若有写者，等待。没有则阻塞后续写者
   
   sem_post(&mutexreadcount);
   sem_post(&que);
   //reading;                           //读者无需互斥
   printf("process %d is reading\n",i);
   sleep(1);
      printf("process %d complete reading\n",i);
   sem_wait(&mutexreadcount);
   readcount--;                        //直到没有读者才会释放write。后来的读者会增加readcount，故不会等待写者
   if(readcount==0) sem_post(&w);
   sem_post(&mutexreadcount);
}

void* writer(void* arg)          
{
    int i=*(int*)arg;
    printf("process %d want to write\n",i);
    sem_wait(&que);                  //  增加que信号量。后面先来的写者先申请que。
    sem_wait(&w);                
    //write
    printf("process %d is writing\n",i);
    sleep(4);
    printf("process %d complete writing\n",i);
    sem_post(&w);
    sem_post(&que);                       //写完再释放que。这样防止readcount一直增加导致读者不释放write。
                                          //同时写者互斥
}


int main(int argc, char*argv[])                                
{
int ret;    
sem_init(&w,0,1); sem_init(&mutexreadcount,0,1); sem_init(&que,0,1);
readcount=0;

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