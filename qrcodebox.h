/*
 *@file:   qrcodebox.h
 *@author: 缪庆瑞
 *@date:   2016.11.3
 *@brief:  生成绘制二维码
 */
#ifndef QRCODEBOX_H
#define QRCODEBOX_H

#include "qrencode.h"
#include <QGroupBox>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QMessageBox>


class QRcodeBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit QRcodeBox(QWidget *parent = 0);
    ~QRcodeBox();
    void generateString(QString text, int version=1, int failoverLevel=0, double scale=2 ,QColor color=QColor(Qt::black),QString iconAddr="");
    void drawQRcode(QPainter &painter, int x, int y);
    bool saveImage(QString fileName,int size);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    QRcode *qr;//qrencode开源代码中的结构体，存放二维码版本，宽度，和包含编码数据（0，1）char*串
    int failoverLevel;//容错级别
    double scale;//二维码的缩放比例
    QColor color;//二维码的颜色
    QString iconAddr;//图标地址

    QImage iconImage;//存放加载的图标，默认为NULL

};

#endif // QRCODEBOX_H
