# -*- coding: utf-8 -*-
"""
Created on Sun Jun 23 19:33:37 2019

@author: SmallBillows
"""

import wx
import os
import numpy as np
from PIL import ImageGrab 
import socket
import sys
import struct
import cv2
import pyaudio
import wave

HOST = '127.0.0.1'
PORT = 51327
input_filenamescr = "screen.jpg"                         
input_filepath = "E:"          
in_pathscr = input_filepath + input_filenamescr  
input_filename = "record.wav"                          # 麦克风采集的语音输入
input_filepath = "E:"              # 输入文件的path
in_pathrec = input_filepath + input_filename  
input_filenamecam = "camera.jpg"                         
input_filepath = "E:"          
in_pathcam = input_filepath + input_filenamecam   



def get_screen(filepatha):    #摄像头截图
       img = ImageGrab.grab()
       img.save(filepatha)


def get_audio(filepath):  #麦克风录音
        CHUNK = 256                 #定义数据流块
        FORMAT = pyaudio.paInt16    #量化位数（音量级划分）
        CHANNELS = 1               # 声道数;声道数：可以是单声道或者是双声道
        RATE = 8000                # 采样率;采样率：一秒内对声音信号的采集次数，常用的有8kHz, 16kHz, 32kHz, 48kHz, 11.025kHz, 22.05kHz, 44.1kHz
        RECORD_SECONDS = 10          #录音秒数
        WAVE_OUTPUT_FILENAME = filepath     #wav文件路径
        p = pyaudio.PyAudio()               #实例化

        stream = p.open(format=FORMAT,
                        channels=CHANNELS,
                        rate=RATE,
                        input=True,
                        frames_per_buffer=CHUNK)
       # print("*"*10, "开始录音：请在5秒内输入语音")
        frames = []                                                 #定义一个列表
        for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):      #循环，采样率11025 / 256 * 5
            data = stream.read(CHUNK)                               #读取chunk个字节 保存到data中
            frames.append(data)                                     #向列表frames中添加数据data
      #  print(frames)
      #  print("*" * 10, "录音结束\n")

        stream.stop_stream()
        stream.close()          #关闭
        p.terminate()           #终结

        wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')                  #打开wav文件创建一个音频对象wf，开始写WAV文件
        wf.setnchannels(CHANNELS)                                   #配置声道数
        wf.setsampwidth(p.get_sample_size(FORMAT))                  #配置量化位数
        wf.setframerate(RATE)                                       #配置采样率
        wf.writeframes(b''.join(frames))                            #转换为二进制数据写入文件
        wf.close()              #关闭
        
#拍照
 
def get_camera(filepath):
   cap=cv2.VideoCapture(0)
   ret,frame = cap.read()
   i=0;
   cv2.imwrite(filepath,frame)
   cap.release()
   cv2.destroyAllWindows()        


class MainWindow(wx.Frame):
    def __init__(self,parent,title):
        self.dirname = ''
        wx.Frame.__init__(self,parent,title=title,size=(200,-1))
        self.contrl = wx.Frame() #暂时不用这个控件
        self.CreateStatusBar()# 创建位于窗口的底部的状态栏

        #设置菜单
        filemenu = wx.Menu()

        menuOpen = filemenu.Append(wx.ID_OPEN,"&Open","Open a file")
        menuAbout = filemenu.Append(wx.ID_ABOUT,"&About","Information about this program") #(ID, 项目名称, 状态栏信息)
        filemenu.AppendSeparator()  #分割线
        menuExit = filemenu.Append(wx.ID_EXIT,"&Exit","Terminate the program") # (ID, 项目名称, 状态栏信息)


        #创建菜单栏
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu, "&File") #在菜单栏中添加filemenu菜单
        self.SetMenuBar(menuBar) #在frame中添加菜单栏

        #设置 events
        self.Bind(wx.EVT_MENU, self.OnOpen, menuOpen)
        self.Bind(wx.EVT_MENU, self.OnAbout,menuAbout)
        self.Bind(wx.EVT_MENU, self.OnExit,menuExit)

        #设置 sizer
        self.sizer2 = wx.BoxSizer(wx.HORIZONTAL)
        self.buttons = []
        self.buttons.append(wx.Button(self, -1, "开启服务器"))
        self.sizer2.Add(self.buttons[0], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "quit"))
        self.sizer2.Add(self.buttons[1], 1, wx.SHAPED)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.contrl,1,wx.EXPAND)
        self.sizer.Add(self.sizer2,0,wx.GROW)
        
        self.Bind(wx.EVT_BUTTON, self.OpenServer, self.buttons[0])
        self.Bind(wx.EVT_BUTTON, self.Quit, self.buttons[1])

        #激活sizer
        self.SetSizer(self.sizer)
        self.SetAutoLayout(True)
        self.sizer.Fit(self)
        self.Show(True)
        
        
    def OpenServer(self,e):
            s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            print('waiting ------->')
            s.connect((HOST,PORT))       
            dlg = wx.MessageDialog(None, "connected with:"+HOST+"对方将控制你的机器", "Check", wx.YES_NO)
            if dlg.ShowModal() == wx.ID_NO:
                    trigger = '0'
                    s.sendall(trigger.encode())
                    dlg = wx.MessageDialog(self,'同意被控制',"Connection",wx.OK)
                    dlg.ShowModal() #显示对话框
                    dlg.Destroy()   #当结束之后关闭对话框
                    self.Close(True)
            else:
                 dlg = wx.MessageDialog(self,'同意被控制',"Connection",wx.OK)
                 dlg.ShowModal() #显示对话框
                 dlg.Destroy()   #当结束之后关闭对话框
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
                   fileinfo_size = struct.calcsize('128sl')
                   fhead = struct.pack('128sl', bytes(os.path.basename(filepath).encode()),os.stat(filepath).st_size)  #encode很重要
                   s.send(fhead)
                   print ('client filepath: {0}'.format(filepath))  #
                   fp = open(filepath, 'rb')  # rb 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
                   while 1:
                       data = fp.read(1024)
                       if not data:
                           print ('{0} file send over...'.format(filepath))
                           break
                       s.send(data)
             elif data.lower() == '2':
                   print('对方将获取你的录音')
                   get_audio(in_pathrec)
                   filepath = in_pathrec
                   if os.path.isfile(filepath):
                       fileinfo_size = struct.calcsize('128sl')
                       fhead = struct.pack('128sl', bytes(os.path.basename(filepath).encode()),os.stat(filepath).st_size)  #encode很重要
                       s.send(fhead)
                       print ('client filepath: {0}'.format(filepath))  #
                       fp = open(filepath, 'rb')  # rb 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
                       while 1:
                           data = fp.read(1024)
                           if not data:
                               print ('{0} file send over...'.format(filepath))
                               break
                           s.send(data)    
             elif data.lower() == '3':
                   print('对方将获取你的摄像头拍照')
                   get_camera(in_pathcam)
                   filepath = in_pathcam
                   if os.path.isfile(filepath):
                       fileinfo_size = struct.calcsize('128sl')
                       fhead = struct.pack('128sl', bytes(os.path.basename(filepath).encode()),os.stat(filepath).st_size)  #encode很重要
                       s.send(fhead)
                       print ('client filepath: {0}'.format(filepath))  #
                       fp = open(filepath, 'rb')  # rb 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
                       while 1:
                           data = fp.read(1024)
                           if not data:
                               print ('{0} file send over...'.format(filepath))
                               break
                           s.send(data) 
             elif data.lower() == '4':
                 fileinfo_size=struct.calcsize('128sl')
                 buf = s.recv(fileinfo_size)
                 if buf:
                     filename, filesize = struct.unpack('128sl', buf)
                     fn = filename.decode().strip('\00')                             #Python strip() 方法用于删除字符串头部和尾部指定的字符，默认字符为所有空字符，包括空格、换行(\n)、制表符(\t)等。
                     new_filename = os.path.join('./', 'new_' + fn)
                     print ('file new name is {0}, filesize is {1}'.format(new_filename,
                                                                     filesize))
                     
                     recvd_size = 0  # 定义已接收文件的大小
                     dlg = wx.MessageDialog(self,'在server路径下收到文件'+new_filename,"Connection",wx.OK)
                     dlg.ShowModal() #显示对话框
                     dlg.Destroy()   #当结束之后关闭对话框
                     fp = open(new_filename, 'wb')
                     print ('start receiving...')
                     
                     while not recvd_size == filesize:
                         if filesize - recvd_size > 1024:
                             data = s.recv(1024)
                             recvd_size += len(data)
                         else:
                                 data = s.recv(filesize - recvd_size)
                                 recvd_size = filesize
                         fp.write(data)
                     fp.close()
                     print ('end receive...')
             elif data.lower() == '5':
                   print('对方将获取你的E:jojo.txt') #这里应该先接收路径，处理后将该文件发给client
                   filepath = 'E:jojo.txt'   #简单点，发送E:jojo.txt
                   if os.path.isfile(filepath):
                       fileinfo_size = struct.calcsize('128sl')
                       fhead = struct.pack('128sl', bytes(os.path.basename(filepath).encode()),os.stat(filepath).st_size)  #encode很重要
                       s.send(fhead)
                       print ('client filepath: {0}'.format(filepath))  #
                       fp = open(filepath, 'rb')  # rb 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
                       while 1:
                           data = fp.read(1024)
                           if not data:
                               print ('{0} file send over...'.format(filepath))
                               break
                           s.send(data) 
             elif data.lower() == '-1':
                  s.close()
                  self.Close(True)
                   
            

    def Quit(self,e):
        self.Close(True)
        
    def OnAbout(self,e):
        #创建一个带"OK"按钮的对话框。wx.OK是wxWidgets提供的标准ID
        dlg = wx.MessageDialog(self,"小组成员：刘Q 郭WT","About Editors",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()   #当结束之后关闭对话框

    def OnExit(self,e):
        self.Close(True)  #关闭整个frame

    def OnOpen(self,e):
        dlg = wx.FileDialog(self,"Choose a file",self.dirname,"","*·*",wx.FD_OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname,self.filename),'r')#暂时只读
            self.contrl.SetValue(f.read())
            f.colse()
        dlg.Destroy()
        
if __name__ == '__main__':
    
    app = wx.App(False)
    frame = MainWindow(None,title="Server")
    app.MainLoop()
    
            
            