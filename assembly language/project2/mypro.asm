assume cs:writefloppy

;3.5英寸软盘，2面*80磁道*18扇区*512字节=1.44mb
;开机后BIOS int19h 将0面0道1扇区安装到0:7c00h，然后cs:IP指向0:7C00H，执行。
;若程序长度>512字节，该扇区应该将其他扇区的内容读入内存（也就是作为引导程序）
; 除了引导程序和被引导的程序，还要有程序将这两部分写入软盘

;写软盘
writefloppy segment
assume cs:writefloppy   ;;如果想在代码段直接使用数据标号访问数据，需要assume

write:
 ;引导程序 0面0道1扇区
 mov ax,bootloader
 mov es,ax
 mov bx,0    ;es:bx指向要写入的数据

 mov ch,0	;磁道号
 mov cl,1	;扇区号
 mov dh,0	;驱动器号，软驱从0开始，0为软驱A
 mov dl,0	;磁头（面）
 mov al,1  ;写一个扇区

 mov ah,3  ;写扇区 int13h 3
 int 13h

 ;被引导的程序  2-16   整体8kb=8192B=16扇区 这里试一下1+15 
 mov ax,system
 mov es,ax

 mov cl,2
 mov al,15

 mov ah,3
 int 13h

done:
 mov ax,4c00h
 int 21h

writefloppy ends

;;引导程序，读被引导程序的扇区写入内存。
bootloader segment
assume cs:bootloader

boot:
 mov ax,2000h  ;将被引导程序读入内存 2000:0 
 mov es,ax
 mov bx,0

 mov ch,0 
 mov cl,2 
 mov dh,0 
 mov dl,0  
 mov al,15 

 mov ah,2
 int 13h

setcsip:
 mov ax,2000h
 push ax
 mov ax,0
 push ax
 retf  ;CS IP 设置为2000:0

bootloader ends

;被引导程序，功能函数：菜单显示，重启，引导，显示时间，设置时间
; 考虑显示和用户输入 ，结构为
;显示菜单
;处理输入->跳转到对应函数
;函数处理，返回到处理输入

system segment
assume cs:system

;自己加的堆栈
dw 40 dup(0)

;主菜单
menu:
;设堆栈
mov ax,system
mov ss,ax
mov sp,20h

jmp near ptr showmenu ;段内转移，大于 127_-128

menudata:
    openstr0 db "------ welcome ------",0
    openstr1 db "1) reset pc",0 ;重启计算机
    openstr2 db "2) start system",0     ;引导当前系统
    openstr3 db "3) clock",0   ;显示时钟
    openstr4 db "4) set clock",0    ;设置时钟
    ;四个功能
    sys_func dw func_restart,func_diskboot,func_showtime,func_settime   
    ;菜单显示
    menustr dw openstr0,openstr1,openstr2,openstr3,openstr4

;参数：dh=行号，dl=列号，cl=颜色，ds:si指向字符串首地址
showmenu:
    call cls
    ;显示函数参数
    mov dh,5
    mov dl,30
    mov bp,0
    mov ax,cs
    mov ds,ax

    mov cx,5
showmenu_s:
    push cx
    mov si,menustr[bp]
    mov cl,02h
    call func_showstr0
 ;下一行
    add bp,2
    add dh,2
    pop cx
    loop showmenu_s



;处理用户输入
func_input:
    ;键盘读入送到AL，读ASCII码
    mov ah,0
    int 16h

    mov bx,0
    mov bl,al

    sub bl,31h ;ascii转换为功能序号 30h+1h 
    ;排除其他输入
    cmp bx,0
    jb waitinginput ;无符号小于跳转
    cmp bx,3
    ja waitinginput ;无符号大于跳转
    add bx,bx
    ;,只可以使用 BX, BP, SI, DI 
    call word ptr sys_func[bx]  ;调用菜单功能
waitinginput:
    jmp short func_input



 ;清屏
cls:
        push bx
        push cx
        push dx
        push si
		push es
	   
        mov bx,0b800h        ;显存地址 0xb800 　　80x25模式下，一屏的内容在显示缓冲区共4000个字符。
        mov es,bx

        xor bx,bx

        mov cx,4000           ;文本模式显示25行，80字   即80*25*2 = 4000字节
   clss:   
		mov dl,0            ;NULL字符=ASCII 0
        mov dh,0            ;黑色 
        mov es:[bx],dx        
        add bx,2
        loop clss
		
		pop es
        pop si
        pop dx
        pop cx
        pop bx
        ret


;不放在这里会一直闪，应该放在处理用户输入之后，这样会等待输入不闪烁
;显示以0结尾字符串
;dh=行号，dl=列号，cl=颜色，ds:si指向字符串首地址
func_showstr0:
    push ax
    push cx
    push dx
    push si
    push bp
    push es

    mov ax,0b800h
    mov es,ax
    mov al,80*2 ;80*2、  一行80*2字节
    mul dh  ;al*dh,得行起始地址。结果在AH
    mov dh,0
    add dx,dx ;列号*2
    add ax,dx ;ax存字符串在显存开始位置
    mov bp,ax

showstr_s:
    mov ch,ds:[si]   ;一个个的字符，当ds[si]=0说明到了最后
    cmp ch,0
    je showstr_return

    mov es:[bp],ch
    inc bp
    mov es:[bp],cl ;颜色
    inc bp
    inc si
    jmp short showstr_s

showstr_return:
    pop es
    pop bp
    pop si
    pop dx
    pop cx
    pop ax
    ret



;重启
; ffff:0 单元有跳转指令跳到BIOS重启
func_restart:
    mov ax,0ffffh
    push ax
    mov ax,0h
    push ax
    retf


;从硬盘 C，0道0面1扇区引导
func_diskboot:
    call cls
    mov ax,0h  ;硬盘数据读取到0:7c00
    mov es,ax
    mov bx,7c00h
    mov al,1  ;读取的扇区数
    mov dh,0  ;0面
    mov ch,0  ;0道
    mov cl,1  ;1扇区
    mov dl,80h  ;硬盘从80h开始，80h:C
    mov ah,2
    int 13h

    call func_restart


;显示时钟
func_showtime:
    call cls
    jmp short readtime
timedata:
    ;分别显示年月日时分秒，功能按键
    ;年月日时分秒偏移分别是0,
    timecolor db 02h
    timeform db '00/00/00 00:00:00',0
    tips1 db 'Press ESC and return to menu',0
    tips2 db 'Press F1 and change color',0
    timestr dw offset timeform,offset tips1,offset tips2
    ;CMOS 中 年月日时分秒在987420单元
    comstable db 9,8,7,4,2,0

readtime:
    mov si,0        ;时间字符串偏移
    mov di,0        ;cmos偏移
    mov cx,6
readtime_s:
    push cx
    ;写要读的地址
    mov al,comstable[di]
    out 70h,al 
    ;读到al       
    in al,71h
    ;16进制转10进制
    mov ah,al
    mov cl,4        
    shr ah,cl        ;10位数/16
    and al,00001111b    ;个位数保留
    ;转ascii
    add ah,30h
    add al,30h

    mov byte ptr timeform[si],ah    ;把读出的时间写入
    mov byte ptr timeform[si+1],al
    add si,3
    inc di
    pop cx
loop readtime_s

printtime:
    ;显示函数的参数   
     mov dh,6
     mov dl,30
     mov bp,0
     mov ax,cs
     mov ds,ax
     mov cx,3
printtime_s:
     push cx
     mov si,timestr[bp]
     mov cl,timecolor[0]  ;将颜色值赋值给cl
     call func_showstr0
     add bp,2
     add dh,2
     pop cx
     loop printtime_s
input_timefunc:
     mov ah,1 ;16h中断1号功能（非阻塞），可以一直显示时间变化
     int 16h
    ;ascii f1 112=70H  al若为0说明是不可打印字符，扫描码（不是ascii）放在ah
     cmp ah,3bh ;判断是否为F1
     je changecolor
    ;ascii esc 27
     cmp al,1bh
     je returntime
    ;否则继续显示时间
     jmp short readtime
changecolor:
    inc timecolor
    mov ah,0   
    int 16h
    jmp near ptr readtime

returntime:
     call cls
     mov ah,0   ;16h中断1号功能不会清除键盘缓冲区，清空避免干扰
     int 16h 
     jmp near ptr menu ;退出返回菜单



;模拟栈，字符入栈，出栈,显示
;  ds[si]栈底
;bx top，栈深度
;(ah)=0入栈，1出栈，2显示
;ah=0，(al)为入栈字符
;ah=1，(al)返回的字符
;ah=2，(dh)(dl)字符串在屏幕显示的行列位置
mystack:
    jmp short stack_pre
mystackdata:
    stackfunc dw mypush,mypop,showstack
    top  dw 0
stack_pre:
    push bx
    push dx
    push di
    push es

    ;大于说明出错
    cmp ah,2
    ja stackdone

    mov bl,ah
    mov bh,0
    add bx,bx
    jmp word ptr stackfunc[bx]
mypush:
    mov bx,top
    mov [si][bx],al
    inc top
    jmp stackdone
mypop:
    ;到底
    cmp top,0
    je stackdone

    dec top
    mov bx,top
    mov al,[si][bx]
    jmp stackdone
showstack:
    mov bx,0b800h
    mov es,bx
    mov al,160
    mov ah,0
    mul dh
    ;行*160+列*2=di=偏移
    mov di,ax
    add dl,dl
    mov dh,0
    add di,dx
    mov bx,0

showstack_s:
    cmp bx,top
    je stackdone
    mov al,[si][bx]
    mov es:[di],al
    mov byte ptr es:[di+2],' '
    mov byte ptr es:[di+1],02h
    inc bx
    add di,2
    jmp showstack_s

stackdone:
    pop es
    pop di
    pop dx
    pop bx
    ret



;设置时钟
func_settime:
    jmp short showtips
inputdata:
    settimedata db 'Inuput time as: "yy/mm/dd hh:mm:ss"',0
    setsuccess  db  'Success,Press any key to return',0
;提示信息
showtips:
    call cls
    mov ax,cs
    mov ds,ax
    mov si,offset settimedata
    mov dh,6
    mov dl,20
    mov cl,02h
    call func_showstr0

    ;读输入
    ;call gettime 

;子程序：接收字符串
;放到ds[si]
gettime:
    push ax
gettimes:
    mov ah,0
    int 16h
    ;小于32不是可显示字符，读扫描码
    cmp al,20h
    jb invisible
    ;否则入栈显示字符。ah=0，入栈
    mov ah,0
    call mystack
    ;入栈完显示
    mov ah,2
    mov dh,8
    mov dl,25
    call mystack
    jmp gettimes

invisible:
    ;退格键
    cmp ah,0eh
    je backspace
    ;enter键
    cmp ah,1ch
    je enter
    jmp gettimes

backspace:
    ;ah=1，出栈1个字符
    mov ah,1
    call mystack
    ;显示
    mov ah,2
    call mystack
    jmp gettimes
enter:
    ;enter结束输入，添加最后的0,显示，然后返回
    mov al,0
    mov ah,0
    call mystack
    mov ah,2
    call mystack
    pop ax
    ;设置时间
    ;call settime

;模拟栈读取输入，ds[si]指向时间字符
;读到dh 10位 dl个位
settime:
    jmp short setposition
cmosdata:
    setcmostable db 9,8,7,4,2,0
setposition:
    mov bx,0
    mov cx,6
settime_s:
    push cx
    mov dh,ds:[si]
    inc si
    mov dl,ds:[si]
    add si,2

    ;转ascii
    ;mov al,30h
    sub dl,30h
    sub dh,30h
    ;shl移位大于1要放到cl
    mov cl,4
    shl dh,cl

    or dl,dh
    ;写cmos
    mov al,setcmostable[bx]
    out 70h,al
    mov al,dl
    out 71h,al
    ;下一单位
    inc bx
    pop cx
    loop settime_s

;提示退出
showquitinfo:
    mov ax,cs
    mov ds,ax
    mov si,offset setsuccess
    mov dh,10
    mov dl,20
    mov cl,02h
    call func_showstr0

    ;按任意键退出
    mov ah,0
    int 16h
    call cls
    jmp near ptr menu

system ends
end write