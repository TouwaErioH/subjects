import sys
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QGridLayout, QLabel, QLineEdit, QToolButton, QPushButton)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont, QPalette, QBrush, QPixmap


class Login(QWidget):
    def __init__(self):
        super().__init__()
        self.bodyLayout = QGridLayout()

        # 欢迎登陆图书馆系统标题
        self.titleText = QLabel(self)
        self.titleText.setText('欢迎使用图书馆管理系统')
        self.titleText.setAlignment(Qt.AlignCenter)
        self.titleText.setFixedSize(800, 100)
        ft1 = QFont("宋体", 20)
        self.titleText.setFont(ft1)

        # 账号标题
        ft = QFont("宋体", 20)
        account = QLabel()
        account.setText('账号')
        account.setFont(ft)

        # 密码标题
        password = QLabel()
        password.setText('密码')
        password.setFont(ft)

        # 学号输入框
        self.accountInput = QLineEdit()
        self.accountInput.setFixedSize(600, 50)
        self.accountInput.setText('学号')
        self.accountInput.setTextMargins(5, 5, 5, 5)
        self.accountInput.mousePressEvent = lambda x: self.inputClick(self.accountInput)
        # self.accountInput.setClearButtonEnabled(True)

        # 密码输入框
        self.passwordInput = QLineEdit()
        self.passwordInput.setFixedSize(600, 50)
        self.passwordInput.setText("输入你的密码")
        self.passwordInput.setTextMargins(5, 5, 5, 5)
        self.passwordInput.mousePressEvent = lambda x: self.inputClick(self.passwordInput)
        self.passwordInput.setEchoMode(QLineEdit.Password)
        # self.passwordInput.setClearButtonEnabled(True)

        # 注册按钮
        self.signup = QToolButton()
        self.signup.setText('注  册')
        self.signup.setFixedSize(150, 60)

        # 登录按钮
        self.loginButton = QToolButton()
        self.loginButton.setText('登  录')
        self.loginButton.setFixedSize(150, 60)

        # 把上面定义的元素加入大框
        self.inputBoxLayout = QVBoxLayout()
        self.inputBoxLayout.addWidget(account)
        self.inputBoxLayout.addWidget(self.accountInput)
        self.inputBoxLayout.addWidget(password)
        self.inputBoxLayout.addWidget(self.passwordInput)
        self.inputBoxLayout.addWidget(self.signup)
        self.inputBoxLayout.addWidget(self.loginButton)

        # 下面一个大框
        self.inputBox = QWidget()
        self.inputBox.setObjectName('inputBox')
        self.inputBox.setContentsMargins(100, 20, 100, 100)  #到四边的距离
        self.inputBox.setFixedSize(800, 500)          #宽，高
        self.inputBox.setLayout(self.inputBoxLayout)

        # 把大标题和下面输入框加入self
        self.bodyLayout.addWidget(self.titleText, 0, 0)
        self.bodyLayout.addWidget(self.inputBox, 1, 0)
        self.setLayout(self.bodyLayout)
        self.setFixedSize(1000, 700)    #矩形大框所能占有的长和宽
        self.setMyStyle()



    def inputClick(self, e):
        if e.text() == '学号' or e.text() == '******':
            e.setText('')

    def setMyStyle(self):
        self.setStyleSheet('''
            QWidget{
                background-color:white;
            }
        ''')
        self.titleText.setStyleSheet('''
            *{
                color: rgba(250, 255, 240);
                width: 200px;
                background-color: rgba(128, 128, 105, 1);
                border: 1px solid rgba(220, 243, 249, 1);
                border-radius: 10px;
            }
        ''')
        self.inputBox.setStyleSheet('''
        QWidget#inputBox{
            border-radius: 5px;
            border: 1px solid rgba(229, 229, 229, 1);
        }
        QLineEdit{
            color: grey;
            border-radius: 5px;
            border: 3px solid rgba(229, 229, 229, 1);
        }
        QToolButton{
            border-radius: 10px;
            background-color:rgba(135, 206, 235, 1);
            color: white;
            font-size: 25px;
            font-family: 微软雅黑;
        }
        QPushButton{
            color:blue;
            font-weight:300;
            border:0;
            background-color:white;
        }
        ''')


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Login()
    ex.show()
    sys.exit(app.exec_())
