/*
 *@file:   qrcodewidget.h
 *@author: 缪庆瑞
 *@date:   2016.11.2
 *@brief:  布置生成二维码的整体界面
 */
#ifndef QRCODEWIDGET_H
#define QRCODEWIDGET_H

#include "qrcodebox.h"
#include "QZXing.h"
#include <QWidget>
#include <QTextEdit>
#include <QTextCursor>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QColor>
#include <QLabel>
#include <QLineEdit>

class QRcodeWidget : public QWidget
{
    Q_OBJECT

public:
    QRcodeWidget(QWidget *parent = 0);
    ~QRcodeWidget();

public slots:
    void setQRcodeColorSlot();//设置二维码颜色
    void produceQRcodeSlot();//点击生成二维码
    void setColorRandSlot();//启停随机颜色
    void setIconSlot();//设置图标
    void saveImageSlot();//保存图片

    void parseImageSlot();//解析二维码图片

private:
    QZXing *zxing;//解析二维码的接口类，具体内容在库中
    QColor color;//二维码的颜色
    bool colorRandFlag;//随机颜色标记
    //bool QRcodeExist;//标记是否生成了二维码
    QTextEdit *textEdit;//文本框，输入二维码存放的内容,或显示解析二维码内容
    QString iconAddr;//图标地址
    QRcodeBox *qrcodeBox;//存放绘制的二维码
    QGroupBox *qrcodeBox2;//存放被解析的二维码图片，也可能只是张图片
    QLabel *imageLabel;//用来展示打开的图片

    //容错级别的单选按钮 允许的误码率
    QButtonGroup *failoverRadioBox;//按钮组件容器，方便判断单选按钮的选中状态
    QRadioButton *failoverL;//7%
    QRadioButton *failoverM;//15%
    QRadioButton *failoverQ;//25%
    QRadioButton *failoverH;//30%

    //版本及缩放
    QComboBox *versionBox;//版本号
    QDoubleSpinBox *scaleNumber;//缩放比例

    //生成QRCodebox中的部件
    QPushButton *setColorBtn;//设置颜色
    QPushButton *colorBtn;//显示当前颜色
    QPushButton *iconBtn;//添加图标
    QLineEdit *iconAddrDisplay;//图标地址显示
    QPushButton *produceQRcodeBtn;
    QPushButton *saveImageBtn;//保存成图片

    //解析QRcodebox中的部件  2017.2.25 add
    QPushButton *selectImageBtn;//选择图片
    QLineEdit *imageAddrDisplay;//图片地址显示
};

#endif // QRCODEWIDGET_H
