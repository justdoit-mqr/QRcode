#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T21:26:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRcode
TEMPLATE = app

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

#linux 平台
unix{
    LIBS += -L./libs/lib_linux/ -lqrencodeLib #qrencode 生成二维码的开源c语言静态库
    LIBS += -L./libs/lib_linux/ -lQZXing #QZXing静态库
    #LIBS += -L./libs/lib_linux/ -lQZXing2 #QZXing动态库
}
#windows 平台
win32{
    LIBS += -L./libs/lib_windows/ -lqrencodeLib #qrencode 生成二维码的开源c语言静态库
    LIBS += -L./libs/lib_windows/ -lQZXing #QZXing静态库
    #LIBS += -L./libs/lib_windows/ -lQZXing2 #QZXing动态库
}
