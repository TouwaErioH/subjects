# -*- coding: utf-8 -*-
"""
Created on Mon Jun 24 14:47:35 2019

@author: erio
"""
from PIL import Image,ImageGrab
import socket
import sys
import struct
import os
import time
import json

IP = '192.168.43.190'  # 填写服务器端的IP地址
port = 80  # 端口号必须一致
input_filenamescr = "screen.jpg"                         
input_filepath = "E:"          
in_pathscr = input_filepath + input_filenamescr    

  
def get_screen(filepatha):
   img = ImageGrab.grab()
   img.save(filepatha)


def socket_client():
 s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  #创建socket对象
 '''
 try:
    s.connect((IP, port))
 except Exception as e:
    print('server not find or not open')
    sys.exit()
 finally:
     '''
 s.connect((IP, port))
 
 print('与ip127.0.0.1建立连接,对方将远程控制你的机器')# inconsistent use of tabs and spaces in indentation
 name = input('输入y同意对方控制,输入n拒绝对方控制并退出:')
 if name== 'n':
    print('拒绝控制')
    trigger = '0'
    s.sendall(trigger.encode())
    sys.exit()
 else:
    print('同意控制')
    trigger = '1'
    s.sendall(trigger.encode())
 while True:
    data = s.recv(1024)
    data = data.decode()
    if data.lower() == '1':
        print('对方将获取你的屏幕截图')
        get_screen(in_pathscr)
        filepath = in_pathscr
        #filemesg = filepath.strip()
        filemesg = filepath.strip()
        filesize_bytes = os.path.getsize(filemesg) # 得到文件的大小,字节
        filename = 'new' + filemesg
        dirc = {
            'filename': filename,
            'filesize_bytes': filesize_bytes,
        }
        head_info = json.dumps(dirc)  # 将字典转换成字符串
        head_info_len = struct.pack('i', len(head_info)) #  将字符串的长度打包
        #   先将报头转换成字符串(json.dumps), 再将字符串的长度打包
        #   发送报头长度,发送报头内容,最后放真是内容
        #   报头内容包括文件名,文件信息,报头
        #   接收时:先接收4个字节的报头长度,
        #   将报头长度解压,得到头部信息的大小,在接收头部信息, 反序列化(json.loads)
        #   最后接收真实文件
        s.send(head_info_len)  # 发送head_info的长度
        s.send(head_info.encode('utf-8'))

        #   发送真是信息
        with open(filemesg, 'rb') as f:
            data = f.read()
            s.sendall(data)
        print('发送成功')
				
 s.close()


socket_client()

