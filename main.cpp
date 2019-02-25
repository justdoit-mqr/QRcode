#include "qrcodewidget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QRcodeWidget w;
    w.setWindowTitle("二维码生成工具");
    w.resize(520,400);
    w.show();

    return a.exec();
}
