/*
 *@file:   qrcodebox.cpp
 *@author: 缪庆瑞
 *@date:   2016.11.3
 *@brief:  生成绘制二维码
 */
#include "qrcodebox.h"
#include <QtMath>
#include <QImage>
#include <QDebug>

QRcodeBox::QRcodeBox(QWidget *parent) :
    QGroupBox(parent)
{
    qr = NULL;//初始化
    failoverLevel = 0;
    scale = 2;//默认缩放为2倍
    color = QColor(Qt::black);//初始颜色黑色
    iconAddr = "";
}

QRcodeBox::~QRcodeBox()
{
    if(qr != NULL)
    {
        QRcode_free(qr);//释放结构体变量空间
    }
}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.3
 *@brief:   将输入的文本内容生成二维码的二维数组
 *@param:   text:要编码的内容
 *@param:   version:二维码的版本1-40 1(21*21模块) 2(25*25模块) 每加一个版本模块加4
 *@param:   failoverLevel:容错级别
 *@param:   scale:缩放比例
 *@param:   color:二维码的颜色
 *@param:   iconAddr:图标地址
 */
void QRcodeBox::generateString(QString text, int version, int failoverLevel, double scale, QColor color, QString iconAddr)
{
    if(qr != NULL)
    {
        QRcode_free(qr);
    }
    this->failoverLevel = failoverLevel;
    this->scale = scale;
    this->color = color;
    this->iconAddr = iconAddr;
    QRecLevel level;//定义枚举变量
    //根据传来容错级别的值，确定要使用的枚举值。其实这里的参数和枚举值的数字是对应的，可以直接使用参数
    switch(failoverLevel)
    {
    case 0:
        level = QR_ECLEVEL_L;
        break;
    case 1:
        level = QR_ECLEVEL_M;
        break;
    case 2:
        level = QR_ECLEVEL_Q;
        break;
    case 3:
        level = QR_ECLEVEL_H;
        break;
    default:
        level = QR_ECLEVEL_L;//若传来的容错级别有误，默认用最低级别
        break;
    }

    /* toStdString 是将QString的内容经过toUtf8()的中间转换变成c++ std命名空间的string，
     * 然后在转成const char*。
     * version:二维码的版本,如果编码的内容太多导致所选版本存放不下时,会自动使用更高的版本。
     * 这里使用QR_MODE_8编码模式,对于非字母数字都使用8bit 编码utf8字符 最后一个是敏感事件(?)
     */
    qr = QRcode_encodeString(text.toStdString().c_str(),version,level,QR_MODE_8,1);
    /* 判断图片是否成功，因为drawQRcode()函数一旦重绘就会执行，而结束提示对话框，
     * 界面就会重绘一次，这样一旦图片加载失败就会不停的弹出提示框，所以把提示框放在
     * 绘制函数的外边，每执行一次生成只会执行一次
     */
    if(!iconAddr.isEmpty())
    {
        if(!iconImage.load(iconAddr))
        {
            QMessageBox::information(0,tr("QR_code 1.0.0"),tr("加载图片失败\n请查看图片格式、路径\n是否正确!!!"));
        }
    }
    update();//更新重绘
}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.3
 *@brief:   绘制二维码
 *@param:   painter:用来绘图
 *@param:   x:二维码左顶点的x坐标
 *@param:   y:二维码左顶点的y坐标
 */
void QRcodeBox::drawQRcode(QPainter &painter, int x, int y)
{
    painter.setBrush(QBrush(color));//设置二维码颜色
    int qrWidth = qr->width;
    //绘制二维码
    for(int i=0;i<qrWidth;i++)//纵坐标
    {
        for(int j=0;j<qrWidth;j++)//横坐标
        {
            //data变量类型为unsigned char*,以一维的形式存放二维码的数据0/1（按行存储）
            uchar a = qr->data[i*qrWidth + j];
            if(a&0x01)
            {
                /*if(coloursFlag)
                {
                    //给三个定位图形一样的颜色，否则不易识别 定位图形模块数7*7
                    if((i<7 && (j<7||j>qrWidth-8)) || ((i>qrWidth-8)&&j<7))
                    {
                        painter.setBrush(QBrush(Qt::black));
                    }
                    else
                    {
                        //这里使用HSL色彩模式随机产生颜色，三个参数分别表示色相(颜色)、饱和度(鲜艳)、(明度)
                        //H色相范围0-360，色相环以角度指定颜色，L饱和度0-255(用灰度的多少表示鲜艳程度)
                        //S明度0-255，表示黑和白之间的明亮度 这里防止颜色过浅 使用0-200
                        color = QColor::fromHsl(qrand()%360,qrand()%255,qrand()%100);
                        painter.setBrush(QBrush(color));
                    }
                }*/
                //二维码模块以矩形框填充
                QRectF rect(x+j*scale,y+i*scale,scale,scale);
                painter.drawRect(rect);
            }
        }
    }
    //如果有图标且成功被加载，则绘制图标
    if(!iconAddr.isEmpty()&& !iconImage.isNull())
    {
        int module = ((qr->version-1)*4)+21;//二维码的单行或单列模块数
        double imageWidth;//图标宽度
        switch(failoverLevel)
        {
        case 0:
            imageWidth = qSqrt(module * module * 0.05);//7% 根据容错级别确定可以用图标遮挡的模块宽度
            break;
        case 1:
            imageWidth = qSqrt(module * module * 0.09);//15%  //容错要比标准的小，否则不易识别
            break;
        case 2:
            if(qr->version ==1)
            {
                imageWidth = qSqrt(module * module * 0.11);//版本一较小，容错大会遮挡定位图
                break;
            }
            imageWidth = qSqrt(module * module * 0.14);//25%
            break;
        case 3:
            if(qr->version ==1)
            {
                imageWidth = qSqrt(module * module * 0.11);
                break;
            }
            imageWidth = qSqrt(module * module * 0.14);//30%
            break;
        default:
            imageWidth = qSqrt(module * module * 0.04);
            break;
        }
        int tmpDistance = (qrWidth-imageWidth)/2*scale;
        QRectF rect(x+tmpDistance,y+tmpDistance,imageWidth*scale,imageWidth*scale);
        painter.drawImage(rect,iconImage);
    }
}
/*
 *@author:  缪庆瑞
 *@date:    2016.11.5
 *@brief:   保存图片，其实就是再绘制一遍二维码，只是载体是图片而不是widget，还有一个方法是
 *          使用render()将widget上画好的二维码渲染到图片上，但考虑到二维码可能会超出widget范围
 *          不好渲染，就使用了再绘制一次的方法
 *@param:   fileName:文件路径
 *@param:   size:保存图片的长宽
 *@return   bool类型 true：成功 false：失败
 */
bool QRcodeBox::saveImage(QString fileName, int size)
{
    //保存图片为了更清晰，将二维码以10倍的缩放比例定义image大小
    int actualSize = qr->width*10+40;
    QImage image(actualSize,actualSize,QImage::Format_ARGB32);//image需要指定大小，否则painter无法在其上面绘制
    //这里定义的painter作用范围是该函数，在函数结束时会自动析构，因为这里出现错误SIGSEGV花了不少时间
    QPainter painter(&image);//在image上绘图，QImage和QPixmap都是绘图设备
    painter.setPen(Qt::NoPen);//因为要使用drawRect()只填充颜色，不画边缘线，也就不需要画笔
    painter.setBrush(QBrush(Qt::white));//二维码背景区域为白色
    painter.drawRect(0,0,actualSize,actualSize);//画一个比二维码略大的区域

    int tmpScale = scale;
    scale = 10.0;//暂时将缩放比例设成10
    drawQRcode(painter,20,20);//留宽20的白色边缘
    scale = tmpScale;//将缩放比例恢复
    /*罪魁祸首是下面这句话，scaled的返回值是一个新的QImage对象，而不是指针，即执行过下面这句话后
     *image的内存地址发生了变化，而在函数结束后，painter会进行析构，而painter的参数image不再是绘图时
     *的image，会产生异常，即野指针的现象
     */
    //image = image.scaled(400,400);
    QImage saveImage = image.scaled(size,size);//将图片缩放到指定大小
    //第一个参数指定保存的文件名，第二个图片格式，第三个是图片质量0-100
    if(saveImage.save(fileName,0,90))
    {
        return true;
    }
    else
    {
        return false;
    }

}

/*
 *@author:  缪庆瑞
 *@date:    2016.11.3
 *@brief:   重写重绘事件处理函数，根据qrencode库生成的二维数组绘制二维码
 *@param:   event:绘制事件
 */
void QRcodeBox::paintEvent(QPaintEvent *event)
{
    QGroupBox::paintEvent(event);//先调用父类的重绘事件处理函数，不然默认的groupbox的边框和标题都不再显示
    QPainter painter(this);
    painter.setPen(Qt::NoPen);//因为要使用drawRect()只填充颜色，不画边缘线，也就不需要画笔
    painter.setBrush(QBrush(Qt::white));//二维码背景区域为白色
    if(qr != NULL)
    {
        int qrWidth = qr->width*scale;//用scale个像素宽表示二维码一个0\1模块
        int x = (this->width()-qrWidth)/2;//二维码左顶点在box中的x位置
        int y = (this->height()-qrWidth)/2;//二维码左顶点在box中的y位置
        painter.drawRect(x-5,y-5,qrWidth+10,qrWidth+10);//画一个比二维码略大的区域

        drawQRcode(painter,x,y);//绘制二维码模块
    }
}
