采用pyinstaller将python源代码打包为.exe可执行文件。
但是因为调用的库较多所以出现了一定的错误，还在解决中。
需要测试，可以在安装了wx，cv2，pyaudio，ImageGrab，wave，os，socket，struct，sys库的python3.7环境直接运行server.py，client.py测试。