书籍配套的代码osfromscratch2010.iso  见 https://github.com/yyu/osfs00

实验中遇到了一些问题，差不多都在https://www.cnblogs.com/lqerio/category/1494269.html。


书籍：
高清版为手打版，可能有部分错误/符号丢失。另有扫描版，网络中有很多资源。


code：
在linux makefile 的过程中可能会报错undefined reference to `__stack_chk_fail'
只需要在Makefile中的$(CFLAGS)后面加上-fno-stack-protector，即不需要栈保护

lib/klib.o:lib/klib.c
        $(CC) $(CFLAGS) -fno-stack-protector -o $@ $<

然后编译就可以了正常执行了

另外bochs的安装见https://www.cnblogs.com/lqerio/p/11654617.html

bochsrc需要做修改
修改bochsrc中部分内容：
修改vgaromimage对应的文件位置，以你的实际安装位置为准
注释掉keyboard_mapping一行
增加display_library: sdl
