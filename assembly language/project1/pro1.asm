assume cs:code,ds:data

data segment
    db '1975','1976','1977','1978','1979','1980','1981','1982'
    db '1983','1984','1985','1986','1987','1988','1989','1990'
    db '1991','1992','1993','1994','1995' ;date   字符串 （byte）

    dd 16,22,382,1356,2390,8000,16000,24486
	dd 50065,97479,140417,197514,345980,590827,803530,1183000
    dd 1843000,2759000,3753000,4649000,5937000	;income   数据 dd 4 294 967 295 （dword）

    dw 3,7,9,13,28,38,130,220,476,778,1001,1442,2258,2793,4037,5635
    dw 8226,11542,14430,15257,17800 ;employee 数据  dw65535 （word）
data ends

stack segment
    dw 16 dup (0)
stack ends

; line 存储输出的字符串
line segment
	;输出格式 一行40 bytes 4+6+1+9+1+9+1+8=39.39+1=40 。最后的0标志一行字符串结束。中间字符'0'表面一段结束
	;		  ('YEAR      0 income  0 employee0 avg    ',0)
    db 21 dup ('year      x         x         x        ',0)
line ends


code segment
start:

initial:
	mov ax,data
    mov ds,ax ;data
	
    mov ax,stack
	mov ss,ax
    mov sp,20h;stack
	
	mov ax,line
    mov es,ax
	mov si,0; show lines

	;bx标志在data段的偏移
	;di 标志 员工数的偏移;因为dw,dd长度分别为2,4,故设置两个
    mov bx,0
    mov di,0

    mov cx,21 ;21 years
	call cls ;clear screen

;写字符串
data_processing:  
        push cx
		
		;将year 写入一行;
		;ax 2bytes。2*2=4='1975'
		;内存中 例如
		;    00 01 02 03
		;AA  1   9  7  5
        mov ax,[bx+0]
        mov es:[si],ax
        add si,2   ; e.g.,'19'   
		
        mov ax,[bx+2]
        mov es:[si],ax
        add si,2 ;e.g., '75'
		
		;写空格
        add si,6
		
		;写收入
		; 内存中 21年，年份占 21*4=84字节=54H 字节
		;然后开始 dd 格式的 收入。 4字节一组，小端。
		;高2字节存到dx，低2字节存到ax
        mov ax,[bx+54H]
        mov dx,[bx+54H+2]
		
		;收入转化为10进制字符串写入 line
		;长度不超过10，设定为10，美观
        call dtocdoubleword
        add si,10
		
		;写员工数
		;4*21+4*21=168=A8H
		;然后开始 dw格式的 员工数量
        mov ax,[di+0A8H]
        call dtocword
        add si,10
		
		;写人均
		;总收入/员工
        mov ax,[bx+54H+0]
        mov dx,[bx+54H+2]
        mov cx,[di+0A8H]
		;dx高16位，ax低16位,余数舍弃
        call divdw
		;人均转化为10进制串，写入line
        call dtocdoubleword
		;有一个0作为结束标记，占1字节，其余9个空格，初始化时定义
        add si,10
		
		;然后下一年;相对偏移4 bytes
        add bx,4
        add di,2

        pop cx
        loop data_processing
	
show_init:	
    mov dh,3   ;显示的初始行
    mov ax,line
    mov ds,ax
	
    mov si,0 ;字符串在line中的偏移（在show_str中会不断增加，遇到0停止，下面s1循环只inc si一次到下一年起始）
    mov cx,21
	
;显示字符串
show:
        push cx
		
        mov dl,15 ;起始列
        mov cl,00000010B ; 颜色  R G B 1 2 3
		;显示一行，以 0 结尾
        call show_str
		
        inc dh  ;换到显存下一行显示
        inc si  ;到字符串中下一行起始偏移
		
        pop cx
        loop show

done:
    mov ax,4C00H
    int 21H


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

;显示字符串。以 0 结尾
; 输入：dh 行号(0-24) dl(0-79) 列号 cl 颜色 ds:[si] 字符串首地址（line[si]）; 在程序中会改变
; 输出:在屏幕显示字符串
show_str:   
    push cx
    push ax
    push dx
    push es
	push bx
	;push si  si不用恢复
	
	calc_offset:
    mov al,160
    mul dh  ; mul ax=dh*al 。  显示器80*25.一行80*2=160字节偏移。dh为显示行，比如dh=1，跳过0行，跳过160字节
    ;sub ax,160
	
    add dl,dl;最多79+79=158=9EH，无溢出;列号*2 一列偏移2字节
    mov dh,0 ;dx=dh dl
    add ax,dx;dh行dl列的显存偏移地址
    mov bx,ax;这时es:[bx]指向了指定的显示器地址
	
	mov ax,0B800H
    mov es,ax ;显存  显存一列偏移2字节
    mov ah,cl;ah存放颜色格式
    
	continuewrite: 
        mov cl,[si]  ;[si]= line[si]
        mov ch,0
		
        jcxz show_str_recorvery;字符串以 0 结尾
		
        mov al,[si];得到数据放入al
        mov es:[bx],ax
		
        inc si  ;line中下一个字符  line[si+1]
		add bx,2;下一个字符到下一列
        jmp short continuewrite
	
    show_str_recorvery: 
		pop bx
        pop es
        pop dx
        pop ax
        pop cx
        ret

;  word 数据转化为字符串
;  输入：ax 16位 员工数
;  输出：转换为字符串写到line 对应的位置
;  es:[si]指向line当前写入位置（偏移）
dtocword:   
    push ax
    push bx
    push cx
    push dx
    push es
	
    mov bx,0 ;记录字符串位数
	
	;补全高16位，每次除10，余数就是 10进制的数字，从个位到十，百，...
	;转换为10进制ascii，压栈
    num2char:  
        mov dx,0
        mov cx,10
        div cx ; DX 高位，AX低位 (DX AX)/CX
		
        add dx,30H ;ascii '0'=30H
		push dx  ;当前10进制位，转换为字符串入栈
		
        inc bx;  bx 为 转换为10进制的数字的位数，也就是字符串长度
		
        mov cx,ax;商为0代表数字处理结束
		
        jcxz stack2string
        jmp short num2char
		
    stack2string:  
        mov cx,bx  ;写到line 的循环次数为10进制数的位数
		mov bx,0
	
	;si 已经偏移10，见主函数
	;写字符串
    writeline: 
        pop ax
        mov es:[bx+si],al;
        inc bx
        loop writeline

    recover: 
        pop es
        pop dx
        pop cx
        pop bx
        pop ax
        ret

; doubleword 转换为字符串
; 输入：dx高16位，ax低16位
; 输出: 写入到line中相应位置
;  es:[si]指向line当前写入位置（偏移）
dtocdoubleword: 
    push ax
    push bx
    push cx
    push dx
    push es
	
    mov bx,0
	
	;转换为10进制ascii，压栈
    num2chardoubleword:  
        mov cx,10
		
        call divdw;被除数在dx和ax中，余数在cx中;余数即为10进制当前位
		
        add cx,30H
        push cx
		
        inc bx; 32位数据转换为10进制数后的位数
		
        mov cx,dx
        or cx,ax;商为0代表数字处理结束
		
        jcxz stack2stringdoubleword
        jmp short num2chardoubleword
    
	stack2stringdoubleword:  
        mov cx,bx
		mov bx,0
    
	;写字符串
	writelinedw: 
        pop ax
        mov es:[bx+si],al
        inc bx
        loop writelinedw
    
	recoverydw: 
        pop es
        pop dx
        pop cx
        pop bx
        pop ax
        ret
		


;除法溢出分析 见实验十，问题二
;16位除法，ax存结果的商，dx存余数
;比如dx:0010H，ax:2343H,除数cx:0004H,dx>cx，会产生溢出。
;解决公式：  X/N=int(H/N)*65536+[rem(H/N)*65536+L]/N
;1.X/N：求x除以N的商
;2.int(H/N)：高位除以N的商
;3.65536：高位和低位的进制
;4.rem(H/N)：高位除以N的余数
;5.rem(H/N)*65536+L：高位和低位相加
;6.+[rem(H/N)*65536+L]/N：再次求商

; 输入：dx=被除数高16位，ax=被除数低16位，cx=除数
; 输出：dx=商高16位，ax=商低16位，cx=余数
divdw:  
    push bx
    push ax ;被除数低16位先入栈
	
    mov ax,dx;
    mov dx,0;
    div cx; 被除数高16位先除 除数
    mov bx,ax;int(H/N)存到bx ，rem(H/N)存到dx
	
    pop ax;
    div cx; int(H/N)+ax 除除数
    mov cx,dx; 余数存到cx
    mov dx,bx; 商高16位存到dx。低16位在ax
	
    pop bx;
    ret
	
code ends
end start
