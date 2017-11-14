/*
 * Implementation for the gpicontroller app.
 * Author: Amos Manneschmidt
 * 2017
*/
#include "gpicontroller.h"
#include "ui_gpicontroller.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QSysInfo>
#include <QScrollBar>
#include <QDesktopWidget>
#include "serialbuffer.h"

QString gpicontroller::get_port()
{
    QString portname;
    portname=ui->comBox->currentText();
    return portname;
}

void gpicontroller::refresh_comBox()
{
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
    connect(ui->spinboxX,SIGNAL(valueChanged(int)),this,SLOT(spinboxX_valueChanged()));
    connect(ui->spinboxY,SIGNAL(valueChanged(int)),this,SLOT(spinboxY_valueChanged()));
    connect(ui->spinboxZ,SIGNAL(valueChanged(int)),this,SLOT(spinboxZ_valueChanged()));
    connect(ui->spinboxNeedle,SIGNAL(valueChanged(double)),this,SLOT(spinboxNeedle_valueChanged()));
    connect(ui->spinboxSyringe,SIGNAL(valueChanged(int)),this,SLOT(spinboxSyringe_valueChanged()));
    refresh_comBox();
    if (QSysInfo::productType()=="osx" && ui->comBox->findText("/dev/cu.usbserial")+1)
        ui->comBox->setCurrentIndex(ui->comBox->findText("/dev/cu.usbserial"));
    if (QSysInfo::productType()=="windows" && ui->comBox->findText("COM3")+1)
        ui->comBox->setCurrentIndex(ui->comBox->findText("COM3"));
    port=new QSerialPort(this);
    open_port(get_port());
    qDebug()<< this->childrenRegion().boundingRect().size();
}

void gpicontroller::open_port(QString portname)
{
    port->setPortName(portname);
    port->open(QIODevice::ReadWrite);
    if (port->error()==QSerialPort::PermissionError || this->port->error()==QSerialPort::OpenError)
    {
        port->close();
        port->clearError();
        port->open(QIODevice::ReadWrite);
    }
    // Check the validity of the port
    if ( !port->error()==QSerialPort::NoError )
    {
        qDebug() << "\nError: " << port->portName() << " port can't be opened ...";
        ui->console->append(port->portName()+" port can't be opened ...");
        ui->labelComstate->setText("Disconnected");
    }
    else
    {
        qDebug() << port->portName() << " port has been opened successfully ...";
        ui->console->append(port->portName()+" port has been opened successfully ...");
        port->setBaudRate(QSerialPort::Baud9600);
        port->setStopBits(QSerialPort::OneStop);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << port->portName() << " port has been configured correctly ...";
        ui->console->append(port->portName()+" port has been configured correctly ...");
        QScrollBar *vsb = ui->console->verticalScrollBar();
        QScrollBar *hsb = ui->console->horizontalScrollBar();
        vsb->setValue(vsb->maximum());
        hsb->setValue(hsb->minimum());
        ui->labelComstate->setText("Connected");
    }
//    connect(port, &QSerialPort::readyRead, this, &gpicontroller::read_data);
//    connect(port, SIGNAL(readyRead()), this, SLOT(read_data()));

//    connect(this, SIGNAL(data_was_read(QString)), buffer, SLOT(data_recieved(QString)));
}


void gpicontroller::select_vial()
{
    QString message = "@GTV ";
    message+=ui->spinboxSelectVial->cleanText();
    qDebug() << message;
    ui->console->append(message);
    QScrollBar *vsb = ui->console->verticalScrollBar();
    QScrollBar *hsb = ui->console->horizontalScrollBar();
    vsb->setValue(vsb->maximum());
    hsb->setValue(hsb->minimum());
    message+="\r";
    port->write(message.toLatin1().data(),message.length());
}

gpicontroller::~gpicontroller()
{
    delete ui;
}

void gpicontroller::on_buttonSelectVial_clicked()
{
   select_vial();
}


void gpicontroller::on_buttonRefresh_clicked()
{
    refresh_comBox();
}

void gpicontroller::on_buttonConnect_clicked()
{
    refresh_comBox();
    open_port(get_port());
}

void gpicontroller::read_data()
{
    readData.append(QString(port->readAll()));
    if ((readData.endsWith("\n") || readData.endsWith("\r")) && readData.simplified().length()>0){
        ui->console->append("<div style='color:DeepSkyBlue'>"+readData.simplified()+"</div>");
        emit data_was_read(readData.simplified());
//        buffer.append(readData.simplified(),p);
        readData.clear();
    }
    QScrollBar * vsb = ui->console->verticalScrollBar(); //pointer to scroll bar existing for length of this read_data call
    vsb->setValue(vsb->maximum());
}

void gpicontroller::on_buttonHome_clicked()
{
    send_message("@HOME");
}

void gpicontroller::send_message(QString message)
{
    qDebug() << message;
    ui->console->append(message);
    QScrollBar *vsb = ui->console->verticalScrollBar();
    QScrollBar *hsb = ui->console->horizontalScrollBar();
    vsb->setValue(vsb->maximum());
    hsb->setValue(hsb->minimum());
    message+="\r";
//    port->write(message.toLatin1().data(),message.length());
    int *r=new int;
    buffer->append("test",r);
//    buffer->last->set_val("0");

}

void gpicontroller::on_buttonPark_clicked()
{
    send_message("@PRK");
}

void gpicontroller::spinboxX_valueChanged()
{
    make_labels_normal_weight(ui->labelX);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxY->setValue(0);
    ui->spinboxZ->setValue(0);
    ui->spinboxSyringe->setValue(0);
}
void gpicontroller::spinboxY_valueChanged()
{
    make_labels_normal_weight(ui->labelY);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxX->setValue(0);
    ui->spinboxZ->setValue(0);
    ui->spinboxSyringe->setValue(0);
}
void gpicontroller::spinboxZ_valueChanged()
{
    make_labels_normal_weight(ui->labelZ);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxY->setValue(0);
    ui->spinboxX->setValue(0);
    ui->spinboxSyringe->setValue(0);
}
void gpicontroller::spinboxNeedle_valueChanged()
{
    ui->spinboxNeedle->blockSignals(true); // to stop from trigging self on the line below the next with "setValue()"
    make_labels_normal_weight(ui->labelNeedle);
    if (ui->spinboxNeedle->value()<1) ui->spinboxNeedle->setValue(1); //actual minimum is 1 but I need to signal to user that nothing is sent so it is set to 0
    ui->spinboxX->setValue(0);
    ui->spinboxY->setValue(0);
    ui->spinboxZ->setValue(0);
    ui->spinboxSyringe->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
}
void gpicontroller::spinboxSyringe_valueChanged()
{
    make_labels_normal_weight(ui->labelSyringe);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxX->setValue(0);
    ui->spinboxY->setValue(0);
    ui->spinboxZ->setValue(0);
}
void gpicontroller::make_labels_normal_weight(QLabel* element){
    QFont font=ui->labelX->font();
    font.setBold(false);
    ui->labelX->setFont(font);
    ui->labelY->setFont(font);
    ui->labelZ->setFont(font);
    ui->labelNeedle->setFont(font);
    ui->labelSyringe->setFont(font);
    font.setBold(true);
    element->setFont(font);
}

void gpicontroller::on_buttonMove_clicked()
{
    if (ui->spinboxX->value()) send_message("@MVX "+QString::number(ui->spinboxX->value()));
    if (ui->spinboxY->value()) send_message("@MVY "+QString::number(ui->spinboxY->value()));
    if (ui->spinboxZ->value()) send_message("@MVZ "+QString::number(ui->spinboxZ->value()));
    if (ui->spinboxNeedle->value()) send_message("@NDL "+QString::number(ui->spinboxNeedle->value()));
    if (ui->spinboxSyringe->value()) send_message("@SYR "+QString::number(ui->spinboxSyringe->value()));

}

void gpicontroller::on_buttonHomeX_clicked()
{
    send_message("@MVX -1480");
}

void gpicontroller::on_buttonHomeY_clicked()
{
    send_message("@MVY -1950");
}

void gpicontroller::on_buttonHomeZ_clicked()
{
    send_message("@MVZ -3500");
}



void gpicontroller::on_arbitrarySerialLine_returnPressed()
{
    on_buttonSendArbitrary_clicked();
}

void gpicontroller::on_buttonSendArbitrary_clicked()
{
    send_message(ui->arbitrarySerialLine->text().simplified()); //simplified removes whitespace from front and back
    ui->arbitrarySerialLine->clear();
}
void gpicontroller::request(int timeout){
    qDebug() << timeout;
}
void gpicontroller::on_buttonSetDepth_clicked()
{
    request(10);
    send_message("@SND");
}


