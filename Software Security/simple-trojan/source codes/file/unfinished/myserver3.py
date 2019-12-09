# -*- coding: utf-8 -*-
"""
Created on Mon Jun 24 14:47:35 2019

@author: erio
"""
import socket
import sys
import os
import struct
import json
host = '192.168.43.190'  # 服务器端可以写"localhost"，可以为空字符串""，也为本机IP地址
port = 80  # 端口号
buffsize = 1024


def socket_server():
 s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   #创建socket对象 
 s.bind((host, port))                                    #socket.AF_INET,服务器之间网络通信 socket.SOCK_STREAM,流式socket , for TCP
 s.listen(3)                                             #backlog指定在拒绝连接之前，操作系统可以挂起的最大连接数量
 while True:
  print('xx')
  conn, addr = s.accept()                #接受TCP连接并返回（conn,address）,其中conn是新的套接字对象，可以用来接收和发送数据。 #address是连接客户端的地址。
  print('connected with', addr)
  data = conn.recv(1024)
  data = data.decode() 
  if data=='0':
   print('对方拒绝被控制，断开连接')
   sys.exit()
  else:
   print('对方同意被控制')
  while True:
    func=input('select a function:')
    if func=='1':
        print('将获取被控制端的屏幕截图')
        send = func
        conn.sendall(send.encode())
	    #head_struct = s.recv(4)  # 接收报头的长度,
        head_struct=conn.recv(4)
        if head_struct:
           print('已连接服务端,等待接收数据')
        head_len = struct.unpack('i', head_struct)[0]  # 解析出报头的字符串大小
        data = conn.recv(head_len)  # 接收长度为head_len的报头内容的信息 (包含文件大小,文件名的内容)
        head_dir = json.loads(data.decode('utf-8'))
        filesize_b = head_dir['filesize_bytes']
        filename = head_dir['filename']

           #   接受真的文件内容
        recv_len = 0
        recv_mesg = b''
        f = open(filename, 'wb')
        while recv_len < filesize_b:
            percent = recv_len / filesize_b
            if filesize_b - recv_len > buffsize:
              recv_mesg = conn.recv(buffsize)
              f.write(recv_mesg)
              recv_len += len(recv_mesg)
            else:
              recv_mesg = conn.recv(filesize_b - recv_len)
              recv_len += len(recv_mesg)
              f.write(recv_mesg)
        print(recv_len, filesize_b)
        f.close()
  conn.close()
  s.close()

socket_server()