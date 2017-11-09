#include "gpicontroller.h"
#include <QApplication>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include "DarkStyle.h"
#include "framelesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CDarkStyle::assign();
    FramelessWindow framelessWindow;
    gpicontroller* mainw=new gpicontroller;
    framelessWindow.setContent(mainw);
    framelessWindow.show();
    return a.exec();
}
