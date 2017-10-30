#include "gpicontroller.h"
#include <QApplication>
#include <QDebug>
#include <QSerialPort>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gpicontroller w;
    w.show();
    return a.exec();
}
