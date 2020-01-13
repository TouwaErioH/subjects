//环境
sudo apt-get update  //更新系统源码 
sudo apt-get install vim //安装vim
sudo apt-get install libncurses5-dev libssl-dev  //下载依赖包
sudo apt-get install libelf-dev
sudo apt-get install build-essential openssl  
sudo apt-get install zlibc minizip  
sudo apt-get install libidn11-dev libidn11
sudo apt-get install flex bison 

//解压
       sudo su                                 //获取root
       sudo mv  压缩包位置    /usr/src         //移动文件
       cd  /usr/src                           //切换到该目录下进行操作
       sudo tar -xvflinux-4.18.14.tar.gz      //解压

//添加调用
sudo vim  kernel/sys.c  //添加自己的函数

asmlinkage void sys_helloworld(void)
{
    printk("hello world，this is erio greeting！");
}

sudo vim arch/x86/include/asm/syscalls.h //添加声明

asmlinkage sys_helloworld(void);

sudo vimarch/x86/entry/syscalls/syscall_64.tbl  //添加调用号

336   64   helloworld   sys_helloworld()

//删除无用的文件
sudo make mrproper
sudo make clean

//配置 
sudo make menuconfig

//编译    
sudo make -j2  

//安装
sudo make modules_install  
sudo make install

//重启 
reboot

//测试程序
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<sys/user.h>
#include <sys/reg.h> //modified
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main()
   {
        int a;
        scanf("%d",&a);
        syscall(336);
        printf("%d\n",a);
        return 0;
   }

//编译程序
gcc -o -g test2 test2.cpp

//运行

./test2

//查看信息
dmesg


//strace
strace ./test2
strace -c ./test2


//gdb
gdb test2
l
r
b 14
b 15
r c
clear
bt
