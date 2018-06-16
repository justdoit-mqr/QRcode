/*
 *@file:   qrcodewidget.cpp
 *@author: 缪庆瑞
 *@date:   2016.11.2
 *@brief:  布置生成二维码的整体界面
 */
#include "qrcodewidget.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QStringList>
#include <QColorDialog>
#include <QFileDialog>
#include <time.h>
#include <QDebug>
/*
 *@author:  缪庆瑞
 *@date:    2016.11.2
 *@brief:   构造整体界面
 */
QRcodeWidget::QRcodeWidget(QWidget *parent)
    : QWidget(parent)
{
    qsrand(time(0));//产生随机数种子，执行一次即可
    color = QColor(Qt::black);//初始化为黑色
    colorRandFlag = false;
    iconAddr = "";
    //初始化解析二维码的接口类，第一个参数表示解码的格式，可以通过逻辑或实现多种格式解码
    zxing = new QZXing(QZXing::DecoderFormat_QR_CODE,this);

    //整体创建一个栅格布局
    QGridLayout *gridLayout = new QGridLayout(this);
    //左侧栅格布局
    QGridLayout *leftGridLayout = new QGridLayout();
    //右侧栅格布局
    QGridLayout *rightGridLayout = new QGridLayout();

    //六个大部件
    textEdit = new QTextEdit(this);//输入内容
    textEdit->zoomIn(2);//将默认字体大小放大两倍
    textEdit->setPlaceholderText("By：为何而来 2016.11.6");//设置默认提示
    qrcodeBox = new QRcodeBox(this);//生成二维码区域
    qrcodeBox->setTitle("二维码区域");
    /*qrcodeBox2区域与qrcodeBox完全一致，只是为了存放解析时打开的图片，因为qrcodeBox重写了paintEvent
    如果直接在上面显示打开的图片，会导致重绘，显示生成的二维码，引起混乱,于是使用一个新的区域显示，通过添加
    在同一个布局里，同时只有一个显示来实现区域的变换*/
    qrcodeBox2 = new QGroupBox("解析二维码图片",this);
    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);//根据label大小放缩图片
    //设置label的大小策略为忽视，避免默认情况根据图片大小改变布局大小的sizehint
    imageLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    QHBoxLayout *qrImagehBoxLayout = new QHBoxLayout();
    qrImagehBoxLayout->setContentsMargins(35,25,35,25);
    qrImagehBoxLayout->addWidget(imageLabel);
    qrcodeBox2->setLayout(qrImagehBoxLayout);
    qrcodeBox2->setVisible(false);//初始化时不显示该区域
    QGroupBox *failoverBox = new QGroupBox("容错级别",this);//存放容错级别选项
    QGroupBox *versionAndScaleBox = new QGroupBox("版本及缩放比例",this);//存放版本及伸缩比例
    QGroupBox *produceBox = new QGroupBox("生成QR码",this);//存放生成按钮等
    QGroupBox *parseBox = new QGroupBox("解析QR码",this);//存放选择图片按钮等

    //容错box的按钮容器及单选部件
    failoverRadioBox = new QButtonGroup(this);//按钮组容器直接继承QObject，不是可显示的widget
    failoverL = new QRadioButton("L(7%)");
    failoverM = new QRadioButton("M(15%)");
    failoverQ = new QRadioButton("Q(25%)");
    failoverH = new QRadioButton("H(30%)");
    failoverM->setChecked(true);//默认选中M容错级别
    failoverRadioBox->addButton(failoverL,0);//添加按钮到按钮组，并为按钮设置Id
    failoverRadioBox->addButton(failoverM,1);
    failoverRadioBox->addButton(failoverQ,2);
    failoverRadioBox->addButton(failoverH,3);
    QGridLayout *failoverGridLayout = new QGridLayout();
    failoverGridLayout->addWidget(failoverL,0,0);//添加到按钮组的部件同样可以添加到布局中
    failoverGridLayout->addWidget(failoverM,0,1);//按钮组是一个不可显示的容器类，它在逻辑上将按钮归组
    failoverGridLayout->addWidget(failoverQ,1,0);//但按钮的实际位置可以任意
    failoverGridLayout->addWidget(failoverH,1,1);
    failoverBox->setLayout(failoverGridLayout);

    //版本及缩放比例box的部件
    versionBox = new QComboBox(this);
    QStringList versions;//定义可选版本号序列
    versions<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"
            <<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20"
            <<"21"<<"22"<<"23"<<"24"<<"25"<<"26"<<"27"<<"28"<<"29"<<"30"
            <<"31"<<"32"<<"33"<<"34"<<"35"<<"36"<<"37"<<"38"<<"39"<<"40";
    versionBox->addItems(versions);//默认显示第一个
    versionBox->setCurrentIndex(3);//初始版本号设为4
    scaleNumber = new QDoubleSpinBox(this);
    scaleNumber->setMinimum(1.0);//设置最小值和最大值
    scaleNumber->setMaximum(10.0);
    //步长由0.1改为0.2，因为缩放比例小数点后为奇数时如5.7，画图可能有偏差，出现干扰线，
    scaleNumber->setSingleStep(0.2);//经过测试感觉可能是qt绘图自身的问题
    scaleNumber->setDecimals(1);//设置精度 小数点位数
    scaleNumber->setValue(5.0);//设置默认缩放比例
    QFormLayout *versionAndScaleFormLayout = new QFormLayout();
    versionAndScaleFormLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    versionAndScaleFormLayout->addRow("版本号：",versionBox);
    versionAndScaleFormLayout->addRow("缩放比例：",scaleNumber);
    versionAndScaleBox->setLayout(versionAndScaleFormLayout);

    //生成QRcodeBox中的部件
    QGridLayout *produceGridLayout = new QGridLayout();
    setColorBtn = new QPushButton("颜色",this);
    colorBtn = new QPushButton("",this);
    //设置显示颜色的按钮的大小策略，水平忽视，否则有文本和无文本，在布局中大小不同
    colorBtn->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    colorBtn->setStyleSheet("background-color:"+color.name());
    iconBtn = new QPushButton("图标",this);
    iconAddrDisplay = new QLineEdit(this);
    //iconAddrDisplay->setEnabled(false);
    produceQRcodeBtn = new QPushButton("生成",this);
    saveImageBtn = new QPushButton("保存",this);
    produceGridLayout->addWidget(setColorBtn,0,0);
    produceGridLayout->addWidget(colorBtn,0,1);
    produceGridLayout->addWidget(iconBtn,1,0);
    produceGridLayout->addWidget(iconAddrDisplay,1,1);
    produceGridLayout->addWidget(produceQRcodeBtn,2,0);
    produceGridLayout->addWidget(saveImageBtn,2,1);
    produceBox->setLayout(produceGridLayout);

    //解析QRcodeBox中的部件
    QGridLayout *parseGridLayout = new QGridLayout();
    selectImageBtn = new QPushButton("选择图片",this);
    imageAddrDisplay = new QLineEdit(this);
    parseGridLayout->addWidget(selectImageBtn,0,0);
    parseGridLayout->addWidget(imageAddrDisplay,0,1);
    parseBox->setLayout(parseGridLayout);

    //左侧栅格布局添加部件
    leftGridLayout->setRowStretch(0,3);//设置第0行伸缩比例 （是针对其他行的比例关系）
    leftGridLayout->setRowStretch(1,1);//设置第1行伸缩比例 如上部件高度比3：1
    leftGridLayout->addWidget(qrcodeBox,0,0);//两个区域部件添加在布局同一位置，同时显示一个
    leftGridLayout->addWidget(qrcodeBox2,0,0);
    leftGridLayout->addWidget(textEdit,1,0);

    //右侧栅格布局添加部件
    rightGridLayout->setRowStretch(0,1);
    rightGridLayout->setRowStretch(1,1);
    rightGridLayout->setRowStretch(2,2);
    rightGridLayout->setRowStretch(3,1);//部件高度比1:1:2:1
    rightGridLayout->addWidget(failoverBox,0,0);
    rightGridLayout->addWidget(versionAndScaleBox,1,0);
    rightGridLayout->addWidget(produceBox,2,0);
    rightGridLayout->addWidget(parseBox,3,0);

    //整体栅格布局添加布局
    gridLayout->setColumnStretch(0,2);//设置第0列伸缩比例
    gridLayout->setColumnStretch(1,1);//设置第1列伸缩比例 如上部件宽度比2:1
    gridLayout->addLayout(leftGridLayout,0,0);
    gridLayout->addLayout(rightGridLayout,0,1);

    //连接按钮的信号与槽
    connect(produceQRcodeBtn,SIGNAL(clicked()),this,SLOT(produceQRcodeSlot()));
    connect(setColorBtn,SIGNAL(clicked()),this,SLOT(setQRcodeColorSlot()));
    connect(colorBtn,SIGNAL(clicked()),this,SLOT(setColorRandSlot()));
    connect(iconBtn,SIGNAL(clicked()),this,SLOT(setIconSlot()));
    connect(saveImageBtn,SIGNAL(clicked()),this,SLOT(saveImageSlot()));
    connect(selectImageBtn,SIGNAL(clicked()),this,SLOT(parseImageSlot()));

    //初始化一个二维码，本人新浪微博主页，图标采用资源文件嵌入到程序中,避免程序发布时还需要附件图片文件
    qrcodeBox->generateString("http://m.weibo.cn/u/1832088940",4,1,5.0,Qt::black,":/icon/icon1.png");
}

QRcodeWidget::~QRcodeWidget()
{

}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.4
 *@brief:   设置二维码颜色
 */
void QRcodeWidget::setQRcodeColorSlot()
{
    //colorBtn->setText("");
    color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(!colorRandFlag)
    {
        colorBtn->setStyleSheet("background-color:"+color.name());
    }
}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.3
 *@brief:   生成二维码的响应槽
 */
void QRcodeWidget::produceQRcodeSlot()
{
    QString text = textEdit->toPlainText();//获取输入的纯文本,无输入则返回空的字符串
    if(text.isEmpty())
    {
        QMessageBox::information(this,"QR_code 1.0.0","请输入要编码的内容！");
        return;
    }
    if(colorRandFlag)//如果启动了颜色随机
    {
        //这里使用HSL色彩模式随机产生颜色，三个参数分别表示色相(颜色)、饱和度(鲜艳)、(明度)
        //H色相范围0-360，色相环以角度指定颜色，L饱和度0-255(用灰度的多少表示鲜艳程度)
        //S明度0-255，表示黑和白之间的明亮度 这里防止颜色过浅 使用0-220
        color = QColor::fromHsl(qrand()%360,qrand()%255,qrand()%220);
    }
    int version = versionBox->currentText().toInt();//获取版本号
    double scale = scaleNumber->value();//获取缩放比例
    int id = failoverRadioBox->checkedId();//获取按钮组中选中的按钮id,对应容错级别
    qrcodeBox->generateString(text,version,id,scale,color,iconAddr);//生成编码数据，并绘制二维码
    qrcodeBox->setVisible(true);
    qrcodeBox2->setVisible(false);
    saveImageBtn->setEnabled(true);
    //QRcodeExist = true;
}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.5
 *@brief:   启停随机颜色
 */
void QRcodeWidget::setColorRandSlot()
{
    if(colorRandFlag)
    {
        colorRandFlag = false;
        colorBtn->setText("");
        colorBtn->setStyleSheet("background-color:"+QString(color.name()));
    }
    else
    {
        colorRandFlag = true;
        colorBtn->setText("Auto");//渐变色
        colorBtn->setStyleSheet("background-color: qlineargradient"
                                "(spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                "stop:0 rgba(255, 0, 0, 255), stop:0.166 "
                                "rgba(255, 255, 0, 255), stop:0.333 "
                                "rgba(0, 255, 0, 255), stop:0.5 "
                                "rgba(0, 255, 255, 255), stop:0.666 "
                                "rgba(0, 0, 255, 255), stop:0.833 "
                                "rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));");
    }

}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.5
 *@brief:   设置图标
 */
void QRcodeWidget::setIconSlot()
{
    //执行打开文件对话框会打印输出“onecoreuap\shell\ext\thumbnailcache\lib\thumbcacheapi.cpp(285)\thumbcache.dll
    //"!59E1FEFF: (caller: 59E0EF51) ReturnHr(21) tid(2fe0) 80004005 未指定的错误,可能是win10库的缘故
    iconAddr = QFileDialog::getOpenFileName(this,"选择图标","./icon","(*.png *.jpg *.ico *.bmp)");
    iconAddrDisplay->setText(iconAddr);
}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.5
 *@brief:   保存图片
 */
void QRcodeWidget::saveImageSlot()
{
    QString saveImagename = QFileDialog::getSaveFileName(this,"保存图片","./image/二维码.png","(*.png *.jpg *.bmp)");
    if(saveImagename.isEmpty())//如果取消保存对话框返回空的字符串
    {
        return;
    }
    if(qrcodeBox->saveImage(saveImagename,400))
    {
        QMessageBox::information(this,"QR_code 1.0.0","图片已成功保存！");
    }
    else
    {
        QMessageBox::information(this,"QR_code 1.0.0","图片保存失败！");
    }
}
/*
 *@author:  缪庆瑞
 *@date:    2017.2.25
 *@brief:   解析二维码图片，使用QZxing开源库
 */
void QRcodeWidget::parseImageSlot()
{
    QString imageAddr = QFileDialog::getOpenFileName(this,"选择二维码图片","./image","(*.png *.jpg *.jpeg *.bmp)");
    if(imageAddr.isEmpty())//如果取消打开对话框返回空的字符串
    {
        return;
    }
    imageAddrDisplay->setText(imageAddr);
    QImage image(imageAddr);
    if(image.isNull())//图片加载失败函数返回
    {
        QMessageBox::information(this,"QR_code 1.0.0","加载图片失败，请确认图片格式！");
        return;
    }
    else
    {
        imageLabel->setPixmap(QPixmap(imageAddr));//展示图片
        qrcodeBox->setVisible(false);
        qrcodeBox2->setVisible(true);
        saveImageBtn->setEnabled(false);//设置保存图片的按钮不可点，直到生成二维码后恢复
        //解析图片，经测试发现解析能力挺强的
        QString parseText = zxing->decodeImage(image);
        if(parseText.isEmpty())
        {
            QMessageBox::information(this,"QR_code 1.0.0","未从图片中扫描到二维码。");
        }
        else
        {
            textEdit->setTextColor(Qt::blue);
            textEdit->setText(parseText);
        }
    }

}
