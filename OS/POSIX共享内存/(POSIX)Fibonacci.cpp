#include<cstdio>
#include<unistd.h>
#include<stdlib.h>//atoi
#include <sys/ipc.h> //shm
#include <sys/shm.h> //shm
#include<sys/types.h>
#include<sys/stat.h> //S_IRUSR | S_IWUSR
#include<sys/wait.h>

using namespace std;

#define MAX 10

typedef struct shares{
long fib[MAX];
int fib_size;
//shares(long fib[0]=0,long fib[1]=1):fib[0](fib[0]),fib[1](fib[1]){}
}share;

int main(int argc, char*argv[])
{
    int i,size;  int segment_id; shares* shared;
pid_t id;
if(argc!=2)
{
    fprintf(stderr,"argu wrong!\n");
    return -1;
}
size=atoi(argv[1]);
if(size>MAX)
{
    fprintf(stderr,"size should be less than %d\n",MAX);
    return -1;
}

if((segment_id=shmget(IPC_PRIVATE,sizeof(share),S_IRUSR | S_IWUSR))==-1)         //IPC-PRIVATE会创建新的共享内存段  S_IRUSR | S_IWUSR既读又写
{
    fprintf(stderr,"Create shared memory segment failed!\n");
    return -1;
}

printf("segment_id=%d\n",segment_id);

if((shared=(share*)shmat(segment_id,0,0))==(share*)-1)
{
    fprintf(stderr,"Unable to attach to segment %d\n",id);
    return -1;
}

shared->fib_size=size;
id=fork();

if(id==-1)
{
     fprintf(stderr,"Create child failed!\n");
    return -1;
}

if(id==0)   //child
{
    printf("Child: shared memory attached at:%p \n",shared);
    shared->fib[0]=0;
    shared->fib[1]=1;
    for(i=2;i<shared->fib_size;i++)
    {
        shared->fib[i]=shared->fib[i-1]+shared->fib[i-2];
    }
    shmdt((void*)shared);           //断开链接。禁止本进程访问此片共享内存 成功返回0
}

else
{
    struct shmid_ds shmbuffer;
    wait(NULL);
    for(i=0;i<shared->fib_size;i++)
    printf("%ld\n",shared->fib[i]);
    shmdt((void*)shared);
    if(shmctl(segment_id,IPC_STAT,&shmbuffer)==-1)   //IPC_RMID：删除这片共享内存  	IPC_STAT：得到共享内存的状态，把共享内存的shmid_ds结构复制到buf中)
    {
        fprintf(stderr,"Unable to access segment %d\n",segment_id);
        return -1;
    }
    printf("ID\t\tKEY\tMODE\t\tOWNER\tSIZE\tATTACHES\n");
    printf("--\t\t---\t----\t\t-----\t----\t-------\n");
    printf("%d\t\t%d\t",segment_id,shmbuffer.shm_perm.__key);   //Linux __t,Mac.key
    unsigned short mode;
    mode=shmbuffer.shm_perm.mode;  //shmbuffer.shm_perm.mode is unsignd short
    
    // OWNER 拥有者可读..

    if(mode & 0400)
    printf("r");
    else printf("-");
    if(mode & 0200)
    printf("w");
    else printf("-");
    if(mode & 0100)
    printf("a");
    else printf("-");
    
    //GROUP 组可读..
    if(mode & 0040)
    printf("r");
    else printf("-");
    if(mode & 0020)
    printf("w");
    else printf("-");
    if(mode & 0010)
    printf("a");
    else printf("-");
    
    //WORLD  全局可读..
    if(mode & 0004)
    printf("r");
    else printf("-");
    if(mode & 0002)
    printf("w");
    else printf("-");
    if(mode & 0001)
    printf("a");
    else printf("-");

    printf("\t%d\t",shmbuffer.shm_perm.uid);
    printf("%lu\t",shmbuffer.shm_segsz);
    printf("%lu\t\n",shmbuffer.shm_nattch);
    printf("time of last attach %ld\n",shmbuffer.shm_atime);                        
}        //icps -m -i id

return 0;
}