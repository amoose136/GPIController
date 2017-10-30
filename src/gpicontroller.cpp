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
    port=new QSerialPort(this);
    openport(getport());
    QString readData;
}

void gpicontroller::openport(QString portname){
    port->setPortName(portname);
    port->open(QIODevice::ReadWrite);
    if (port->error()==QSerialPort::PermissionError || this->port->error()==QSerialPort::OpenError) {
        port->close();
        port->clearError();
        port->open(QIODevice::ReadWrite);
    }
    // Check the validity of the port
    if ( !port->error()==QSerialPort::NoError ) {

        qDebug() << "\nError: " << port->portName() << " port can't be opened ...";
        ui->console->append(port->portName()+" port can't be opened ...");
        ui->labelComstate->setText("Disconnected");
    }
    else {
        qDebug() << port->portName() << " port has been opened successfully ...";
        ui->console->append(port->portName()+" port has been opened successfully ...");
        port->setBaudRate(QSerialPort::Baud9600);
        port->setStopBits(QSerialPort::OneStop);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << port->portName() << " port has been configured correctly ...";
        ui->console->append(port->portName()+" port has been configured correctly ...");
        ui->console->scroll(-5,-5);
        ui->labelComstate->setText("Connected");
    }
    connect(port, &QSerialPort::readyRead, this, &gpicontroller::read_data);
}


void gpicontroller::selectVial(){
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

void gpicontroller::read_data()
{
    readData.append(QString(port->readAll()));
    if (readData.endsWith("\n") || readData.endsWith("\r")){
        ui->console->append("<div style='color:blue'>"+readData.simplified()+"</div>");
        readData.clear();
    }
    ui->console->scroll(0,-1);
}

void gpicontroller::on_buttonHome_clicked()
{
    QString message = "@QRX";
    qDebug() << message;
    ui->console->append(message);
    ui->console->scroll(0,-1);
    message+="\r";
    port->write(message.toLatin1().data(),message.length());
}
