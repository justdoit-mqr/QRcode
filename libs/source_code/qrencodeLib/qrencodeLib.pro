#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T13:03:20
#
#-------------------------------------------------

QT       -= gui

TARGET = qrencodeLib
TEMPLATE = lib
CONFIG += staticlib
DEFINES += HAVE_CONFIG_H

SOURCES += \
    qrencode-3.4.2/bitstream.c \
    qrencode-3.4.2/mask.c \
    qrencode-3.4.2/mmask.c \
    qrencode-3.4.2/mqrspec.c \
    qrencode-3.4.2/qrencode.c \
    qrencode-3.4.2/qrinput.c \
    qrencode-3.4.2/qrspec.c \
    qrencode-3.4.2/rscode.c \
    qrencode-3.4.2/split.c

HEADERS += \
    qrencode-3.4.2/bitstream.h \
    qrencode-3.4.2/config.h \
    qrencode-3.4.2/mask.h \
    qrencode-3.4.2/mmask.h \
    qrencode-3.4.2/mqrspec.h \
    qrencode-3.4.2/qrencode.h \
    qrencode-3.4.2/qrencode_inner.h \
    qrencode-3.4.2/qrinput.h \
    qrencode-3.4.2/qrspec.h \
    qrencode-3.4.2/rscode.h \
    qrencode-3.4.2/split.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
