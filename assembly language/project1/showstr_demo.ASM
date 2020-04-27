assume cs:codesg, ds:datasg

datasg segment
   db 'Welcome to asm!',0
datasg ends

codesg segment
start: mov ax,datasg
       mov ds,ax
       mov dh,12
       mov dl,30
       mov cl,1
       mov si,0
       call show_str

       mov ax,4c00h
       int 21h

show_str:
       push ax
       push cx
       push dx
       push si

       mov ax,0B800h
       mov es,ax
       mov si,0

       ;compute location  dh-row dl-col
       mov al,160
       mul dh
       mov di,ax
       mov al,2
       mul dl
       add di,ax

       ;judge colour cl=3,2,1 R G B
       mov bh,0
judge: add bh,1
       loop judge

       ;display loop
      
display:
       mov bl,ds:[si]
       mov es:[di],bx
       add si,1
       add di,2
       mov cl,ds:[si]
       jcxz endshow
       jmp display

endshow:
       pop si
       pop dx
       pop cx
       pop ax
       ret
           
codesg ends
end start


