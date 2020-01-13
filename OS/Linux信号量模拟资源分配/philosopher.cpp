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
pthread_t id1, id2,id3,id4,id0;   //5 philosophers
sem_t c0,c1,c2,c3,c4;   //5 chopsticks
//思路是前四个哲学家i都必须先拿编号为(i+1)mod5的筷子，而最后一个哲学家先拿c4。放筷子的时候先放先后到的筷子，这样一定不会有死锁。
//也就是资源分级解法
//pshared 的值为 0，那么信号量将被进程内的线程共享，并且应该放置在这个进程的所有线程都可见的地址上

/*pthread_mutex_t mutex_pause = PTHREAD_MUTEX_INITIALIZER;  
bool pthread_pause = false; 
		pthread_mutex_lock( &mutex_pause );                        
		pthread_mutex_unlock( &mutex_pause );*/


void* thread0(void* arg)          
{
    while(true){
  int rd=(rand()%6+1);
            //考虑到睡太长时间不好查看
  printf("哲学家0在思考\n");
          sleep(rd);
  printf("哲学家0想要进餐\n");
  sem_wait(&c1);
  sem_wait(&c0);
  printf("哲学家0正在进餐\n");
  sleep(1);
  sem_post(&c0);
  sem_post(&c1);
  printf("哲学家0进餐完毕，正在思考\n");
    }
}

void* thread1(void* arg)          
{
    while(true){
  int rd=(rand()%6+1);
  printf("哲学家1在思考\n");
          sleep(rd); 
  printf("哲学家1想要进餐\n");
  sem_wait(&c2);
  sem_wait(&c1);
  printf("哲学家1正在进餐\n");
  sleep(1);
  sem_post(&c1);
  sem_post(&c2);
  printf("哲学家1进餐完毕，正在思考\n");
    }
}

void* thread3(void* arg)          
{
    while(true){
  int rd=(rand()%6+1);
  printf("哲学家3在思考\n");
          sleep(rd);
  printf("哲学家3想要进餐\n");
  sem_wait(&c4);
  sem_wait(&c3);
  printf("哲学家3正在进餐\n");
  sleep(1);
  sem_post(&c3);
  sem_post(&c4);
  printf("哲学家3进餐完毕，正在思考\n");
    }
}

void* thread2(void* arg)          
{
    while(true){
int rd=(rand()%6+1);
  printf("哲学家2在思考\n");
          sleep(rd);
  printf("哲学家2想要进餐\n");
  sem_wait(&c3);
  sem_wait(&c2);
  printf("哲学家2正在进餐\n");
  sleep(1);
  sem_post(&c2);
  sem_post(&c3);
  printf("哲学家2进餐完毕，正在思考\n");
    }
}

void* thread4(void* arg)          
{
    while(true){
  int rd=(rand()%6+1);
  printf("哲学家4在思考\n");
          sleep(rd);
  printf("哲学家4想要进餐\n");
  sem_wait(&c4);
  sem_wait(&c0);
  printf("哲学家4正在进餐\n");
  sleep(1);
  sem_post(&c0);
  sem_post(&c4);
  printf("哲学家4进餐完毕，正在思考\n");
    }
}

int main(int argc, char*argv[])                                
{
pid_t ret;    
sem_init(&c0,0,1);  sem_init(&c1,0,1);  sem_init(&c2,0,1);  sem_init(&c3,0,1);  sem_init(&c4,0,1); 


ret = pthread_create(&id0,NULL,thread0,NULL);    
if (ret != 0)                                    
	{
		printf("Create thread0 error!\n");
		exit(1);
	}
ret = pthread_create(&id1,NULL,thread1,NULL);    
if (ret != 0)                                    
	{
		printf("Create thread1 error!\n");
		exit(1);
	}
ret = pthread_create(&id2,NULL,thread2,NULL);    
if (ret != 0)                                    
	{
		printf("Create thread2 error!\n");
		exit(1);
    }
ret = pthread_create(&id3,NULL,thread3,NULL);    
if (ret != 0)                                    
	{
		printf("Create thread3 error!\n");
		exit(1);
	}
ret = pthread_create(&id4,NULL,thread4,NULL);    
if (ret != 0)                                    
	{
		printf("Create thread4 error!\n");
		exit(1);
	}


sleep(10);   //用10秒看一看

pthread_cancel(id0);  pthread_join(id0,NULL);
pthread_cancel(id1);  pthread_join(id1,NULL); 
pthread_cancel(id2);  pthread_join(id2,NULL);
pthread_cancel(id3);  pthread_join(id3,NULL);
pthread_cancel(id4);  pthread_join(id4,NULL);
return 0;  
}