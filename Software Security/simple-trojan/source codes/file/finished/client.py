# -*- coding: utf-8 -*-
"""
Created on Mon Jun 24 14:47:35 2019

@author: erio
"""
import socket
import sys
import os
import struct
host = '192.168.43.190'  # 服务器端可以写"localhost"，可以为空字符串""，也为本机IP地址
port = 80  # 端口号


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
  while True:
    func=input('select a function:')
    if func=='1':
        print('将获取被控制端的屏幕截图')
        send = func
        conn.sendall(send.encode())
	    #fileinfo_size = struct.calcsize('128sl') 
        fileinfo_size=struct.calcsize('128sl')
        buf = conn.recv(fileinfo_size)
        if buf:
            filename, filesize = struct.unpack('128sl', buf)
            fn = filename.decode().strip('\00')                             #Python strip() 方法用于删除字符串头部和尾部指定的字符，默认字符为所有空字符，包括空格、换行(\n)、制表符(\t)等。
            new_filename = os.path.join('./', 'new_' + fn)
            print ('file new name is {0}, filesize is {1}'.format(new_filename,
                                                                 filesize))

            recvd_size = 0  # 定义已接收文件的大小
            fp = open(new_filename, 'wb')
            print ('start receiving...')

            while not recvd_size == filesize:
                if filesize - recvd_size > 1024:
                    data = conn.recv(1024)
                    recvd_size += len(data)
                else:
                    data = conn.recv(filesize - recvd_size)
                    recvd_size = filesize
                fp.write(data)
            fp.close()
            print ('end receive...')
  conn.close()
  s.close()

socket_server()