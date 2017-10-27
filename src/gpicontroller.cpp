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

QSerialPort* port=new QSerialPort;
QString gpicontroller::getport(){
    QString portname;
    portname=ui->comBox->currentText();
    return portname;
}
QSerialPort* gpicontroller::openport(QString portname){
//    if (&port!=nullptr && port.open(QIODevice::ReadWrite)){
//        port.close();
//    }
    QSerialPort nport;
    nport.setPortName(portname);


    // Check the validity of the port
    qDebug() << nport.open(QIODevice::ReadWrite);
    if ( !nport.open(QIODevice::ReadWrite) ) {
        qDebug() << "\nError: " << nport.portName() << " port can't be opened ...";
        ui->console->append(nport.portName()+" port can't be opened ...");
        ui->labelComstate->setText("Disconnected");
    }
    else {
        qDebug() << nport.portName() << " port has been opened successfully ...";
        ui->console->append(" port has been opened successfully ...");
        nport.setBaudRate(QSerialPort::Baud9600);
        nport.setStopBits(QSerialPort::OneStop);
        nport.setDataBits(QSerialPort::Data8);
        nport.setParity(QSerialPort::NoParity);
        nport.setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << nport.portName() << " port has been configured correctly ...";
        ui->console->append(" port has been configured correctly ...");
        ui->labelComstate->setText("Connected");
    }
    return &nport;
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
    port=openport(getport());

}

void gpicontroller::selectVial(){
    QString message = "@GTV ";
    message+=ui->spinboxSelectVial->cleanText();
    qDebug() << message;
    qDebug() << port->open(QIODevice::ReadWrite);
    ui->console->append(message);
    ui->console->scroll(0,-1);
    message+="\r";
//    port->write(message.toLatin1().data(),message.length());
}

gpicontroller::~gpicontroller()
{
    delete ui;
}

void gpicontroller::on_buttonSelectVial_clicked()
{
   selectVial();
}


void gpicontroller::on_buttonRefresh_clicked()
{
    refresh_comBox();
}

void gpicontroller::on_buttonConnect_clicked()
{
    refresh_comBox();
    openport(getport());
}
