#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T21:26:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRcode
TEMPLATE = app
LIBS += -L./libs/ -lqrencodeLib #qrencode 生成二维码的开源c语言静态库
#LIBS += -L./libs/ -lQZXing    #QZXing  解析二维码的开源应用于Qt的静态库
LIBS += -L./libs/ -lQZXing2   #动态库的辅助文件(.a)，存放库中的接口，供编译通过，运行则需要dll文件

SOURCES += main.cpp\
        qrcodewidget.cpp \
    qrcodebox.cpp

HEADERS  += qrcodewidget.h \
    qrcodebox.h \
    qrencode.h \
    QZXing.h \
    QZXing_global.h

RC_FILE += QRcode.rc

RESOURCES += \
    icon.qrc
