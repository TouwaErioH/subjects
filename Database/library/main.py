import sys
from PyQt5.QtWidgets import QApplication
from model import main_widget, database


def main():
    database.init_database()  #初始化数据库
    app = QApplication(sys.argv)
    ex = main_widget.MainWindow()
    ex.show()
    sys.exit(app.exec_())

# 1.app.exec_()的作用是运行主循环，必须调用此函数才能开始事件处理，调用该方法进入程序的主循环直到调用exit（）结束。
# 主事件循环从窗口系统接收事件，并将其分派给应用程序小部件。如果没有该方法，那么在运行的时候还没有进入程序的主循环就直接结束了
# ，所以运行的时候窗口会闪退。
# app.exec_()在退出时会返回状态代码
# 2.不用sys.exit(app.exec_()),只使用app.exec_()，程序也可以正常运行，但是关闭窗口后进程却不会退出。


if __name__ == '__main__':
    main()
