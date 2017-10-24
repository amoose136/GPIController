#include "gpicontroller.h"
#include "ui_gpicontroller.h"
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
gpicontroller::gpicontroller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gpicontroller)
{
    ui->setupUi(this);
    QString portname = "COM3";
    const char MESSAGE[] = "@GTV 0";
    QSerialPort *port = new QSerialPort;
    port->setPortName(portname);
    // Check the validity of the port
    if ( !port->open(QIODevice::ReadWrite) ) {
        qDebug() << "\nError: " << port->portName() << " port can't be opened ...";
    }
    else {
        qDebug() << '\n' << port->portName() << " port has been opened successfully ...";
        port->setBaudRate(QSerialPort::Baud9600);
        port->setStopBits(QSerialPort::OneStop);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << port->portName() << " port has been configured correctly ...";
    }
}

void gpicontroller::sendCommand(){
    qDebug() << "helloworld";
}

gpicontroller::~gpicontroller()
{
    delete ui;
}

void gpicontroller::on_send_Button_clicked()
{
    gpicontroller::sendCommand();
}
