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
        if os.path.isfile(filepath):
            # 定义定义文件信息。128s表示文件名为128bytes长，l表示一个int或log文件类型，在此为文件大小
            fileinfo_size = struct.calcsize('128sl')
            # 定义文件头信息，包含文件名和文件大小
            fhead = struct.pack('128sl', bytes(os.path.basename(filepath).encode()),os.stat(filepath).st_size)  #encode很重要
			#   先将报头转换成字符串      os.path.basename返回path最后的文件名  st_size: 普通文件以字节为单位的大小
        #   发送报头长度,发送报头内容,最后放真是内容
        #   报头内容包括文件名,文件信息,报头
        #   接收时:先接收4个字节的报头长度,
        #   将报头长度解压,得到头部信息的大小,在接收头部信息, 反序列化(json.loads)
        #   最后接收真实文件
            s.send(fhead)
            print ('client filepath: {0}'.format(filepath))  #

            fp = open(filepath, 'rb')  # rb 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
            while 1:
                data = fp.read(1024)
                if not data:
                    print ('{0} file send over...'.format(filepath))
                    break
                s.send(data)
				
 s.close()


socket_client()

