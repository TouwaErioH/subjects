# -*- coding: utf-8 -*-
"""
Created on Sun Jun 23 19:33:37 2019

@author: frame by SmallBillows(GWT)  function by erio(LQ)
"""

import wx
import os
import numpy as np
import time
from PIL import ImageGrab 
import socket
import sys
import struct

HOST = '127.0.0.1'
PORT = 51327
func='-1'

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
        self.buttons.append(wx.Button(self, -1, "文件浏览"))
        self.sizer2.Add(self.buttons[0], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "文件上传"))
        self.sizer2.Add(self.buttons[1], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "文件下载"))
        self.sizer2.Add(self.buttons[2], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "文件执行"))
        self.sizer2.Add(self.buttons[3], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "开启摄像头"))
        self.sizer2.Add(self.buttons[4], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "开启麦克风"))
        self.sizer2.Add(self.buttons[5], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "OpenClient"))
        self.sizer2.Add(self.buttons[6], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "屏幕截屏"))
        self.sizer2.Add(self.buttons[7], 1, wx.SHAPED)
        self.buttons.append(wx.Button(self, -1, "quit"))
        self.sizer2.Add(self.buttons[8], 1, wx.SHAPED)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.contrl,1,wx.EXPAND)
        self.sizer.Add(self.sizer2,0,wx.GROW)
        
        self.Bind(wx.EVT_BUTTON, self.Fileview, self.buttons[0])
        self.Bind(wx.EVT_BUTTON, self.FileUpload, self.buttons[1])
        self.Bind(wx.EVT_BUTTON, self.FileDownload, self.buttons[2])
        self.Bind(wx.EVT_BUTTON, self.FileExecute, self.buttons[3])
        self.Bind(wx.EVT_BUTTON, self.OpenCamera, self.buttons[4])
        self.Bind(wx.EVT_BUTTON, self.OpenMicrophone, self.buttons[5])
        self.Bind(wx.EVT_BUTTON, self.OpenClient, self.buttons[6])
        self.Bind(wx.EVT_BUTTON, self.ScreenView, self.buttons[7])
        self.Bind(wx.EVT_BUTTON, self.Quit, self.buttons[8])

        #激活sizer
        self.SetSizer(self.sizer)
        self.SetAutoLayout(True)
        self.sizer.Fit(self)
        self.Show(True)
        
        
        
        
        
    def Fileview(self,e):
        
        dlg = wx.MessageDialog(self,"使用简单的遍历就可以获取路径，思路简单。但是我想要做成图形化的显示，时间所限暂时未完成","Fileview",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
        
    def FileUpload(self,e):
        func='4'
        dlg = wx.MessageDialog(self,"将上传指定文件到server路径下","FileUpload",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
        #print('将上传指定文件到server路径下')
        send = func
        conn.sendall(send.encode())
        '''
        path_text = wx.TextCtrl(frame, pos=(5, 5), size=(350, 24))
        print(path_text)
        这里可以用textctrl获取想要上传的文件的路径。
        方便起见，我们先上传固定路径的某个文件
        '''
        filepath = 'E:test.txt'
        if os.path.isfile(filepath):
                   fileinfo_size = struct.calcsize('128sl')
                   fhead = struct.pack('128sl', bytes(os.path.basename(filepath).encode()),os.stat(filepath).st_size)  #encode很重要
                   conn.send(fhead)
                   print ('client filepath: {0}'.format(filepath))  #
                   fp = open(filepath, 'rb')  # rb 以二进制格式打开一个文件用于读写。文件指针将会放在文件的开头。
                   while 1:
                       data = fp.read(1024)
                       if not data:
                           print ('{0} file send over...'.format(filepath))
                           break
                       conn.send(data)
        dlg = wx.MessageDialog(self,"Upload E:test.txt","FileUpload",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
        
    def FileDownload(self,e):
        func='5'
        dlg = wx.MessageDialog(self,"将下载指定文件到client路径下","FileDownload",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
       # print('将下载指定文件到client路径下')
        send = func                            #这里应该输入fileview获取的server某个文件的路径，传送到server，server将该文件传送过来
        conn.sendall(send.encode())
        fileinfo_size=struct.calcsize('128sl')   #简单点，只下载server E:jojo.txt
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
        
        dlg = wx.MessageDialog(self,"Download E:jojo.txt from server to client's path!","FileDownload",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
        
    def FileExecute(self,e):
        
        dlg = wx.MessageDialog(self,"使用os.system转换路径，然后执行文件。时间所限暂时未完成","FileExecute",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
    
    def OpenCamera(self,e):
        func='3'
        print('将获取被控制端的摄像头拍照')
        send = func
        conn.sendall(send.encode())
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
        dlg = wx.MessageDialog(self,"已经收到被控端摄像头拍照文件","Camera",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
       # frame2 = wx.Frame(None,-1,title='OpenCamera',size=(400,-1))
       # frame2.Show()
        
    def OpenMicrophone(self,e):
        func='2'
        print('将获取被控制端的麦克风录音')
        send = func
        conn.sendall(send.encode())
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
        dlg = wx.MessageDialog(self,"已经收到被控端录音文件","Camera",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
       # frame3 = wx.Frame(None,-1,title='OpenMicrophone',size=(400,-1))
       # frame3.Show()
        
    def OpenClient(self,e):
        global line
        line = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        line.bind((HOST,PORT))
        line.listen(3)
        global conn,addr
        conn,addr=line.accept()
        '''
        dlg = wx.MessageDialog(self,addr,"Connected",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()   #当结束之后关闭对话框
        '''
        data = conn.recv(1024)
        data = data.decode() 
        if data=='0':
         dlg = wx.MessageDialog(self,'对方拒绝被控制，断开连接',"Connection",wx.OK)
         dlg.ShowModal() #显示对话框
         dlg.Destroy()   #当结束之后关闭对话框
        else:
         dlg = wx.MessageDialog(self,'对方同意被控制',"Connection",wx.OK)
         dlg.ShowModal() #显示对话框
         dlg.Destroy()   #当结束之后关闭对话框
        
        
    def ScreenView(self,e):
        func='1'
        print('将获取被控制端的屏幕截图')
        send = func
        conn.sendall(send.encode())
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
        dlg = wx.MessageDialog(self,"已经收到被控端截屏文件","Camera",wx.OK)
        dlg.ShowModal() #显示对话框
        dlg.Destroy()
        #frame4 = wx.Frame(None,-1,title='ScreenView',size=(400,-1))
        #frame4.Show()
    
    def Quit(self,e):
        func='-1'
        conn.sendall(func.encode())
        line.close()                 #initial 否则可能造成下次打开，用到的端口还在被占用
        self.Close(True)
        
    def OnAbout(self,e):
        #创建一个带"OK"按钮的对话框。wx.OK是wxWidgets提供的标准ID
        dlg = wx.MessageDialog(self,"小组成员：刘强 郭文涛","About Editors",wx.OK)
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
    frame = MainWindow(None,title="Client")
    app.MainLoop()
