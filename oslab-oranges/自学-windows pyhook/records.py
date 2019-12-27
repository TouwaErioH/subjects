# -*- coding: utf-8 -*-
"""
Created on Wed Dec 25 11:33:41 2019
@author: erio
"""
#USB 接口的键盘鼠标
import pythoncom
import PyHook3 as pyHook
import win32api
import time

path = 'F://records.txt'

def onMouseEvent(event):
    with open(path, 'a+') as f:
        f.write("-----Mouse Event Start-----\n")
        # 监听鼠标事件
        print ("MessageName:", event.MessageName)
        print ("WindowName:", event.WindowName)
        print ("Position:", event.Position)
        print ("Wheel:", event.Wheel)
        print ("---")
        # 返回 True 以便将事件传给其它处理程序
        # 注意，这儿如果返回 False ，则鼠标事件将被全部拦截
        # 也就是说你的鼠标看起来会僵在那儿，似乎失去响应了
        t = time.localtime()
        t = time.asctime(t)
        result = "Time : " + t + "\n" + "MessageName: "+ str(event.MessageName)+'\n'+"WindowName: " + str(event.WindowName) + \
                "\n" +"Position: "+str(event.Position)+'\n'+"Wheel: " +str(event.Wheel)+'\n'
        f.write(result)
        f.write("-----Mouse Event End-----\n\n\n")
    f.close()
    return True

def onKeyboardEvent(event):
    with open(path, 'a+') as f:
        # 监听键盘事件
        f.write("-----KeyBoard Event Start-----\n")
        print ("MessageName:", event.MessageName)
        print ("WindowName:", event.WindowName)
        print ("Ascii:", event.Ascii, chr(event.Ascii))
        print ("Key:", event.Key)
        print ("ScanCode:", event.ScanCode)
        print ("Alt", event.Alt)
        print ("---")
        # 同鼠标事件监听函数的返回值
        t=time.localtime()
        t=time.asctime(t)
        result ="Time : " + t + "\n" +"MessageName: "+ str(event.MessageName)+'\n'+"WindowName: " + str(event.WindowName) + " \n" +\
                "Ascii: "+str(event.Ascii)+ ' '+chr(event.Ascii)+'\n'+"Key: " + str( event.Key) + "\n" +"ScanCode: "+str(event.ScanCode)+"\n"+\
                "Alt: "+str(event.Alt)+'\n'
        f.write(result)
        f.write("-----KeyBoard Event End-----\n\n\n")
        f.close()
        if event.Key== 'Q':  # 按下F12后终止adsw
            win32api.PostQuitMessage()
    return True

def main():
    # 创建一个“钩子”管理对象aaavv  q
    hm = pyHook.HookManager()
    # 监听所有键盘事件
    hm.KeyDown = onKeyboardEvent
    # 设置键盘“钩子”
    hm.HookKeyboard()
    # 监听所有鼠标事件
    hm.MouseAll = onMouseEvent
    # 设置鼠标“钩子”
    hm.HookMouse()
    # 进入循环，如不手动关闭，程序将一直处于监听状态
    pythoncom.PumpMessages()


if __name__ == "__main__":
    main()