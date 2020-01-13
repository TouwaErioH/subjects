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

#define chairs 5   //等待的座椅数
int count;              //count与cuts值相同。因为cuts值不便于访问故设置count
pthread_t id1,id0;  
sem_t custs,bars;      //cuts为等待理发的顾客数，不包括正在理发者，初值为0； bars为正在等候理发师的顾客数。初值为0.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   //mutex用于互斥访问count.（初值为1）
//bool pthread_pause = false; 
//pthread_mutex_lock( &mutex_pause );                        
//pthread_mutex_unlock( &mutex_pause );

void* barber(void* arg)          
{
    while(true){
        //等待顾客，无顾客则睡眠
      sem_wait(&custs);     
      pthread_mutex_lock(&mutex);
      count = count -1;     
         //理发师通知顾客已准备好理发                 
      sem_post(&bars);
      pthread_mutex_unlock(&mutex);
      printf("Barber:理发中\n");
      sleep(3);//理发时间
      printf("Barber:理发完成.客人离开\n");
    }
}

void* customer(void* arg)          
{
    int i=*(int*)arg;
    printf("第%d个客人来到\n",i);
  pthread_mutex_lock(&mutex);
   if(count<chairs)
   {
       printf("第%d个客人入等待座\n",i);
       count++;
       sem_post(&custs);
       pthread_mutex_unlock(&mutex);
       sem_wait(&bars);
       printf("第%d个客人正在理发\n",i);
       //gethaircut
   }
   else
   {
      printf("Chairs are all occupied,第i=%d个客人离开\n",i);
      pthread_mutex_unlock(&mutex);
   }
}


int main(int argc, char*argv[])                                
{
pid_t ret;    
sem_init(&bars,0,0);  sem_init(&custs,0,0);

ret = pthread_create(&id0,NULL,barber,NULL);    
if (ret != 0)                                    
	{
		printf("Create barber error!\n");
		exit(1);
	}

if(argc!=2)
{
    fprintf(stderr,"argu wrong!\n");
    return -1;
}
int size=atoi(argv[1]);                           //从命令行输入先后共几个客人

printf("假设总共%d个客人先后来到\n",size);

int num[size+1];
for(int i=1;i<=size;i++) 
{
    //直接传i不行，因为线程创建后，用的是&i，这时i++了，那么之前创建的线程参数也++了
    num[i]=i;
    int slp=rand();
    slp=slp%2+1;                                     //这里决定了来的间隔最长时间
    sleep(slp);    
 ret = pthread_create(&id1,NULL,customer,&num[i]);    //这里不能直接传i。
   if (ret != 0)                                    
	{
		printf("Create customer error!\n");
		exit(1);
	}   
}
pthread_join(id1,NULL); //等待最后一个客人结束
sleep(15);  //如果最后一个客人理发，那么他结束后3秒理发完成。  不理发最多15秒（5个椅子）
printf("理发师下班了!\n");
return 0;  
}