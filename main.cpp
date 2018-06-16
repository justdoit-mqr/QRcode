#include "qrcodewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRcodeWidget w;
    w.setWindowTitle("二维码生成工具");
    w.resize(520,400);
    w.show();

    return a.exec();
}
