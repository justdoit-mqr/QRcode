QRcode是一个生成与解析二维码的小程序，它生成和解析的是QR二维码（日本的一家公司研制的矩阵式二维码符号）。QR码也是当前使用最广泛的二维码之一，微信支付宝的扫码都是QR码。

QR码有着极其复杂的编码及纠错算法，本程序使用的是开源的linux C代码qrencode作为静态库，来生成编码后的01序列，由QT的二维画图QPainter绘制二维码。同时使用基于C++代码应用于QT的QZXing作为动态库，来解码图片，恢复原始信息。

#####################################################################
qrencode开源库地址：http://fukuchi.org/works/qrencode/
QZXing开源库地址：https://sourceforge.net/projects/qzxing/

qrencode生成库的方法：
将下载的压缩包解压，linux下可以直接执行./config  make  make install命令生成库文件。在windows下或者linux下只想要生成编码算法的库，而不需要libpng等图片库则可以自己编译
1) 将qrencode源码中的(*.h  *.c)加入到工程中（不要加qrenc.c,因为这里面使用了png库，本程序用QPainter自己画不需要该库）
2) 将源码中的config.h.in文件修改成config.h并加入工程（因为公司加密原因，这里遇到了问题，但一般情况直接改扩展名即可）
3) 在QT的pro文件中添加DEFINES += HAVE_CONFIG_H 定义全局宏定义
4) 重新定义 MAJOR_VERSION、MICRO_VERSION、MINOR_VERSION、VERSION
5）编译生成静态库（liaqrencodeLib.a,因为我使用的Qt是基于minGW的，静态库和linux一样是.a，而windows下的VS则是.lib）
注：因为使用qt编译c源码还不会生成动态库（.dll + .a/.lib(此处不是静态库，而是动态库的辅助文件，供编译时使用)），所以使用了静态库。使用该库时，只需在编译pro文件中加上  LIBS += -L./ -lqrencodeLib ，然后加上<qrencode.h>即可

QZXing生成库的方法：
QZXing源码默认直接构建能生成动态库，包括QZXing2.dll动态库和libQZXing2.a辅助文件。使用该库时，只需在编译pro文件中配置LIBS += -L./ -lQZXing2，然后加上“QZXing.h”和“QZXing_global.h”两个头文件即可。运行时目录下加上动态库文件。对于生成静态库，也比较容易，只需轻微改动即可，详见qt下静态库与静态库的创建与使用。（代码目录下的libQZXing.a便是静态库），本程序使用的QZXing动态库，因为静态库略大。

使用qrencode库中的方法编码只需要调用下面的接口函数
QRcode *qrcode;
//QR_ECLEVEL_Q  容错等级
qrcode = QRcode_encodeString("http://m.weibo.cn/u/1832088940", 2, QR_ECLEVEL_Q, QR_MODE_8, 0);
QRcode 是一个结构体
typedef struct {
	int version;         ///< version of the symbol
	int width;           ///< width of the symbol
	unsigned char *data; ///< symbol data
} QRcode;
data中按行存储着编码后的01字符串，1：黑块  0：白块

使用QZXing库中的方法解码只需调用下面的方法
QZXing *zxing;
zxing->decodeImage(image);//该方法返回QString串，标识图片二维码的内容

本程序代码结构：
qrcodebox.cpp:绘制二维码的部件
qrcodewidget.cpp:主界面，设置参数，生成保存图片，解析展示二维码图片
qrencode.h //qrencode库函数接口头文件
QZXing.h //QZXing库的接口函数、

#####################################################################
最后说一下QT程序在windows系统上的打包发布
一个程序完成后，使用QT生成release版本的可执行exe文件，但是仅有exe可执行文件是无法在别人的机器上运行的。因为QT默认是动态编译的，程序还需要各种各样的动态库文件，高版本的qt有一个windows下的部署工具windeployqt.exe将生成的release版本的*.exe文件，单独放到一个文件夹中，打开qt的命令行工具，或者将部署工具所在目录添加到系统path变量中，使用cmd命令行进入*.exe所在文件夹，执行命令windeployqt *.exe；(*表示你的程序名)部署工具会自动将依赖的动态库以及插件放到文件夹下。不使用部署工具也可以直接点击*.exe根据缺少的库自己添加，能在本机运行了，但往往还是添不全，在别的机器上还无法运行。因为还需要plugins下面的一些插件（一般是imageformats文件夹下的各种格式的图片库，和platform下的qwindows.dll），总之缺什么去qt库的bin目录和plugins下找就行。

作者联系方式：@为-何-而来(新浪微博)