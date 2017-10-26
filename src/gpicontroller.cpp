/*
 * Implementation for the gpicontroller app.
 * Author: Amos Manneschmidt
 * 2017
*/
#include "gpicontroller.h"
#include "ui_gpicontroller.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSysInfo>

QString gpicontroller::getport(){
    QString portname;
    portname=ui->comBox->currentText();
    return portname;
}

QSerialPort* gpicontroller::openport(QString portname){
    QSerialPortInfo *portinfo=new QSerialPortInfo(&gpicontroller::port);
    if (portinfo!=nullptr)
    {
        port->close();
    }

    port->setPortName(portname);


    // Check the validity of the port
    if ( !port->open(QIODevice::ReadWrite) ) {
        qDebug() << "\nError: " << port->portName() << " port can't be opened ...";
        ui->console->append(" port can't be opened ...");
        ui->labelComstate->setText("Disconnected");
    }
    else {
        qDebug() << '\n' << port->portName() << " port has been opened successfully ...";
        ui->console->append(" port has been opened successfully ...");
        port->setBaudRate(QSerialPort::Baud9600);
        port->setStopBits(QSerialPort::OneStop);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << port->portName() << " port has been configured correctly ...";
        ui->console->append(" port has been configured correctly ...");
        ui->labelComstate->setText("Connected");
    }
    return port;
}

void gpicontroller::refresh_comBox(){
    QString current=ui->comBox->currentText();
    ui->comBox->clear();
    for (int i=0; i<QSerialPortInfo::availablePorts().length();i++){
        if (QSysInfo::productType()=="osx")
            ui->comBox->addItem("/dev/" + QSerialPortInfo::availablePorts()[i].portName());
        else
            ui->comBox->addItem(QSerialPortInfo::availablePorts()[i].portName());
    }
    if (ui->comBox->findText(current)!=-1)
        ui->comBox->setCurrentIndex(ui->comBox->findText(current));
    else
    {
        ui->comBox->insertItem(ui->comBox->currentIndex(),"Select A Port");
        ui->comBox->setCurrentIndex(ui->comBox->currentIndex()-1);
    }
}

gpicontroller::gpicontroller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gpicontroller)
{
    ui->setupUi(this);
    refresh_comBox();
    if (QSysInfo::productType()=="osx" && ui->comBox->findText("/dev/cu.usbserial")+1)
        ui->comBox->setCurrentIndex(ui->comBox->findText("/dev/cu.usbserial"));
    if (QSysInfo::productType()=="windows" && ui->comBox->findText("COM3")+1)
        ui->comBox->setCurrentIndex(ui->comBox->findText("COM3"));
    QSerialPort * port = new QSerialPort;
    port=openport(getport());

}

void gpicontroller::sendCommand( QSerialPort* port){
    QString message = "@GTV ";
    message+=ui->spinboxSelectVial->cleanText();
    qDebug() << message;
    ui->console->append(message);
    ui->console->scroll(0,-1);
    message+="\r";
    port->write(message.toLatin1().data(),message.length());
}

gpicontroller::~gpicontroller()
{
    delete ui;
}

void gpicontroller::on_buttonSelectVial_clicked()
{
   sendCommand(port);
}


void gpicontroller::on_buttonRefresh_clicked()
{
    refresh_comBox();
}

void gpicontroller::on_buttonConnect_clicked()
{
    refresh_comBox();
    port=openport(getport());
}
