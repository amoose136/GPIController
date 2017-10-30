#include "gpicontroller.h"
#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include "DarkStyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CDarkStyle::assign();
    gpicontroller w;
    w.show();
    return a.exec();
}
