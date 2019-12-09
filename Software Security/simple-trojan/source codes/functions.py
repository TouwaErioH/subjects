# -*- coding: utf-8 -*-
"""
Created on Mon Jun 24 14:47:35 2019

@author: erio
"""

from PIL import Image,ImageGrab
import cv2
import pyaudio
import wave



'''
#录音
input_filename = "record.wav"                          # 麦克风采集的语音输入
input_filepath = "E:"              # 输入文件的path
in_pathrec = input_filepath + input_filename         #通俗解释就是wav文件路径

def get_audio(filepath):
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
        
get_audio(in_pathrec)
'''



#截屏
input_filenamescr = "screen.jpg"                         
input_filepath = "E:"          
in_pathscr = input_filepath + input_filenamescr      
def get_screen(filepath):
   img = ImageGrab.grab()
   img.save(filepath)

get_screen(in_pathscr)





'''
#拍照
input_filenamecam = "camera.jpg"                         
input_filepath = "E:"          
in_pathcam = input_filepath + input_filenamecam    
def get_camera(filepath):
   cap=cv2.VideoCapture(0)
   ret,frame = cap.read()
   i=0;
   cv2.imwrite(filepath,frame)
   cap.release()
   cv2.destroyAllWindows()
   
get_camera(in_pathcam)
'''



'''
#录制视频
cap = cv2.VideoCapture(0)#创建一个 VideoCapture 对象

flag = 1 #设置一个标志，用来输出视频信息
num = 1 #递增，用来保存文件名
while(cap.isOpened()):#循环读取每一帧
    ret_flag, Vshow = cap.read() #返回两个参数，第一个是bool是否正常打开，第二个是照片数组，如果只设置一个则变成一个tumple包含bool和图片
    cv2.imshow("Capture_Test",Vshow)  #窗口显示，显示名为 Capture_Test
    k = cv2.waitKey(1) & 0xFF #每帧数据延时 1ms，延时不能为 0，否则读取的结果会是静态帧
    if k == ord('s'):  #若检测到按键 ‘s’，打印字符串
        cv2.imwrite("D:/pycharmthings/IMF/getpics/"+ str(num) + ".jpg", Vshow)
        print(cap.get(3)); #得到长宽
        print(cap.get(4));
        print("success to save"+str(num)+".jpg")
        print("-------------------------")
        num += 1
    elif k == ord('q'): #若检测到按键 ‘q’，退出
        break
cap.release() #释放摄像头
cv2.destroyAllWindows()#删除建立的全部窗口
'''