#include "gpicontroller.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gpicontroller w;
    w.show();
    w.sendCommand();
    return a.exec();
}
