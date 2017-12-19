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
#include <QTimer>
#include <QFloat16>

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
    //connect up the move buttons so they do stuff
    connect(ui->spinboxX,SIGNAL(valueChanged(int)),this,SLOT(spinboxX_valueChanged()));
    connect(ui->spinboxY,SIGNAL(valueChanged(int)),this,SLOT(spinboxY_valueChanged()));
    connect(ui->spinboxZ,SIGNAL(valueChanged(int)),this,SLOT(spinboxZ_valueChanged()));
    connect(ui->spinboxNeedle,SIGNAL(valueChanged(double)),this,SLOT(spinboxNeedle_valueChanged()));
    connect(ui->spinboxSyringe,SIGNAL(valueChanged(int)),this,SLOT(spinboxSyringe_valueChanged()));

    //connect up the arbitrary serial line up and down arrows so command history works
    connect(ui->arbitrarySerialLine,SIGNAL(downPressed()),this,SLOT(on_arbitrarySerialLine_downPressed()));
    connect(ui->arbitrarySerialLine,SIGNAL(upPressed()),this,SLOT(on_arbitrarySerialLine_upPressed()));

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
        ui->console->append("<div style='color:LimeGreen'>"+port->portName()+" port has been opened successfully ...</div>");
        port->setBaudRate(QSerialPort::Baud9600);
        port->setStopBits(QSerialPort::OneStop);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << port->portName() << " port has been configured correctly ...";
        ui->console->append("<div style='color:LimeGreen'>"+port->portName()+" port has been configured correctly ...</div>");
        QScrollBar *vsb = ui->console->verticalScrollBar();
        QScrollBar *hsb = ui->console->horizontalScrollBar();
        vsb->setValue(vsb->maximum());
        hsb->setValue(hsb->minimum());
        ui->labelComstate->setText("Valid Port");
        connect(port, SIGNAL(readyRead()), this, SLOT(read_data()));
        timer->setInterval(5000);
        connect(timer,SIGNAL(timeout()),this,SLOT(timed_out()));
        timer->start();

        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(initialize(QString)));
        send_message("@");
    }
}
void gpicontroller::timed_out()
{
    qDebug()<<"timed out";
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(initialize(QString)));
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_serial(QString)));
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_firmware(QString)));
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position_reciever(QString)));

    ui->labelComstate->setText("Disconnected");
    timer->stop();
    ui->console->append("<div style='color:Red'>Connection timed out</div>");
    QScrollBar *vsb = ui->console->verticalScrollBar();
    QScrollBar *hsb = ui->console->horizontalScrollBar();
    vsb->setValue(vsb->maximum());
    hsb->setValue(hsb->minimum());
    disconnect(timer,SIGNAL(timeout()),this,SLOT(timed_out()));
}
void gpicontroller::scrolldown()
{
    QScrollBar *vsb = ui->console->verticalScrollBar();
    QScrollBar *hsb = ui->console->horizontalScrollBar();
    vsb->setValue(vsb->maximum());
    hsb->setValue(hsb->minimum());
}
void gpicontroller::initialize(QString data)
{
    QStringList datal;
    if (data=="0")
        send_message("@DDF");
    else
        datal=data.split(",");
    if (datal.length()==5)
    {
            disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(initialize(QString)));
            //grrab and display the needle setpoint
            data=datal[0];
            qDebug() << data.toFloat();
            int val=int(data.toFloat()+.5);
            ui->spinboxNeedle->setValue(val);
            ui->console->append("<div style='color:LimeGreen'>Needle SetPoint is: "+data+"</div>");

            //grab and display temperature setpoint
            data=datal[3];
            qDebug() << data.toFloat();
            val=int(data.toFloat()*.25-4);
            ui->spinBoxTemperature->setValue(val);

            send_message("@GSIP 1");
            timer->start();
            connect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_serial(QString)));


    }
}
void gpicontroller::get_serial(QString data){
    if (data!='0')
    {
        disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_serial(QString)));
        ui->labelSerial->setText("Serial: "+data);
        ui->labelSerial->setEnabled(true);
        timer->start();
        send_message("&");
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_firmware(QString)));
    }
}
void gpicontroller::get_firmware(QString data){
    if (data!='0')
    {
        disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_firmware(QString)));
        ui->labelFirmware->setText(data);
        ui->labelFirmware->setEnabled(true);
        ui->labelComstate->setText("Connected");
        scrolldown();
        timer->start();
        send_message("@RDZ");
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position_startup(QString)));
    }
}
void gpicontroller::update_encoder_position_startup(QString data)
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position_startup(QString)));
    timer->stop();
    disconnect(timer,SIGNAL(timeout()),this,SLOT(timed_out()));
    data=QString::number(data.toDouble()*.05);
    ui->labelEncoder->setText(data);
    on_buttonRefreshTempState_clicked();
}

void gpicontroller::select_vial()
{
    QString message = "@GTV ";
    message+=ui->spinboxSelectVial->cleanText();
    send_message(message);
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
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
    if (!setting_serial)
    {
        if ((readData.endsWith("\n") || readData.endsWith("\r")) && readData.simplified().length()>0){
            ui->console->append("<div style='color:DeepSkyBlue'>"+readData.simplified()+"</div>");
            emit data_was_read(readData.simplified());
    //        buffer.append(readData.simplified(),p);
            readData.clear();
        }
    }
    else
    {
        if (readData.simplified()=="Enter S/N (5 digits)"){
            ui->console->append("<div style='color:DeepSkyBlue'>"+readData.simplified()+"</div>");
            emit data_was_read(readData.simplified());
    //        buffer.append(readData.simplified(),p);
            readData.clear();
            setting_serial=false;
            QScrollBar * vsb = ui->console->verticalScrollBar(); //pointer to scroll bar existing for length of this read_data call
            vsb->setValue(vsb->maximum());
            send_message(QString::number(serialNumber));
            qDebug() << "now not setting serial";
            send_message("@GSIP 1");
        }
    }
    QScrollBar * vsb = ui->console->verticalScrollBar(); //pointer to scroll bar existing for length of this read_data call
    vsb->setValue(vsb->maximum());
}

void gpicontroller::on_buttonHome_clicked()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonHome_clicked()));
    send_message("@HOME");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
}

void gpicontroller::send_message(QString message)
{
    if (message!="")
    {
        sendHistory.push_front(message);
        if (ui->labelComstate->text()!="Disconnected")
        {
            qDebug() << message;
            ui->console->append(message);
            QScrollBar *vsb = ui->console->verticalScrollBar();
            QScrollBar *hsb = ui->console->horizontalScrollBar();
            vsb->setValue(vsb->maximum());
            hsb->setValue(hsb->minimum());
            if (message=="#"){
                    setting_serial=true;
            }
            message+="\r";
            port->write(message.toLatin1().data(),message.length());

            // int *r=new int;
            // buffer->append("test",r);
        }
        else if (ui->labelComstate->text()!="Connected")
        {
            ui->console->append("<div style='color:red'>Error: Message \""+message+"\" not sent");
            ui->console->append("<div style='color:red'>Not currently connected to valid machine");
            QScrollBar *vsb = ui->console->verticalScrollBar();
            QScrollBar *hsb = ui->console->horizontalScrollBar();
            vsb->setValue(vsb->maximum());
            hsb->setValue(hsb->minimum());
        }
    }

}

void gpicontroller::on_buttonPark_clicked()
{
    send_message("@PRK");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
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
    ui->spinboxXin->blockSignals(true);
    ui->spinboxXin->setValue(ui->spinboxX->value()*.00719);
    ui->spinboxXin->blockSignals(false);
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
    ui->spinboxYin->blockSignals(true);
    ui->spinboxYin->setValue(ui->spinboxY->value()*.00719);
    ui->spinboxYin->blockSignals(false);
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
    ui->spinboxZmm->blockSignals(true);
    ui->spinboxZmm->setValue(ui->spinboxZ->value()*.05);
    ui->spinboxZmm->blockSignals(false);
}
void gpicontroller::spinboxNeedle_valueChanged()
{
    ui->spinboxNeedle->blockSignals(true); // to stop from trigging self on the line below the next with "setValue()"
    make_labels_normal_weight(ui->labelNeedle);
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

    if (ui->spinboxNeedle->value())
    {
        ipart=int(ui->spinboxNeedle->value());
        if (ipart==ui->spinboxNeedle->value())
        {
                fpart="0";
                send_message("@NDL "+QString::number(ipart));

        }
        else
        {
            if ( ui->spinboxNeedle->value() > ui->labelEncoder->text().toDouble() )
            {
                fpart=QString::number((ui->spinboxNeedle->value()-ipart)/.05);//steps to go up
                if (ui->labelEncoder->text().toInt()==ipart)
                    send_message("@");
                else
                    send_message("@NDL "+QString::number(ipart));
            }
            else
            {
                fpart=QString::number(-(1-(ui->spinboxNeedle->value()-ipart))/.05);//steps to go down
                if (ui->labelEncoder->text().toInt()==ipart+1)
                    send_message("@");
                else
                    send_message("@NDL "+QString::number(ipart+1));
            }
        }
        qDebug() << "fpart: " << fpart;
        qDebug() << "ipart: " << ipart;
        if (fpart!="0")
            connect(this,SIGNAL(data_was_read(QString)),this,SLOT(unified_move_reciever()));
        else
            connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
        return;
    }
    if (QApplication::queryKeyboardModifiers()==Qt::AltModifier)
    {
        if (ui->spinboxX->value()) send_message("@MVX "+QString::number(-(ui->spinboxX->value())));
        if (ui->spinboxY->value()) send_message("@MVY "+QString::number(-(ui->spinboxY->value())));
        if (ui->spinboxZ->value()) send_message("@MVZ "+QString::number(-(ui->spinboxZ->value())));
    }
    else
    {
        if (ui->spinboxX->value()) send_message("@MVX "+QString::number(ui->spinboxX->value()));
        if (ui->spinboxY->value()) send_message("@MVY "+QString::number(ui->spinboxY->value()));
        if (ui->spinboxZ->value()) send_message("@MVZ "+QString::number(ui->spinboxZ->value()));
    }

    if (ui->spinboxSyringe->value() || ui->labelSyringe->font().bold()) send_message("@SYR "+QString::number(ui->spinboxSyringe->value()));
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));

}
void gpicontroller::unified_move_reciever()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(unified_move_reciever()));
    send_message("@MVZ "+fpart);
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
}

//disabled until it is implemented on the microcontroller side
//void gpicontroller::on_buttonHomeX_clicked()
//{
//    send_message("@MVX -1480");
//}

//void gpicontroller::on_buttonHomeY_clicked()
//{
//    send_message("@MVY -1950");
//}

//void gpicontroller::on_buttonHomeZ_clicked()
//{
//    send_message("@MVZ -3500");
//}



void gpicontroller::on_arbitrarySerialLine_returnPressed()
{
    on_buttonSendArbitrary_clicked();
    history_pos=0;
}
void gpicontroller::on_arbitrarySerialLine_downPressed()
{
    disconnect(ui->arbitrarySerialLine,SIGNAL(downPressed()),this,SLOT(on_arbitrarySerialLine_downPressed()));

    ui->arbitrarySerialLine->clear();
    if (history_pos>0)
        history_pos--;
    if (history_pos==-1)
        history_pos=0;

    if (sendHistory.length()>0)
        ui->arbitrarySerialLine->setText(sendHistory[history_pos]);
    connect(ui->arbitrarySerialLine,SIGNAL(downPressed()),this,SLOT(on_arbitrarySerialLine_downPressed()));
}
void gpicontroller::on_arbitrarySerialLine_upPressed()
{
    disconnect(ui->arbitrarySerialLine,SIGNAL(upPressed()),this,SLOT(on_arbitrarySerialLine_upPressed()));
    ui->arbitrarySerialLine->clear();
    if (history_pos==-1)
        history_pos=0;
    if (sendHistory.length()>0)
        ui->arbitrarySerialLine->setText(sendHistory[history_pos]);
    if (history_pos<(sendHistory.length()-1))
        history_pos++;
    connect(ui->arbitrarySerialLine,SIGNAL(upPressed()),this,SLOT(on_arbitrarySerialLine_upPressed()));
}
void gpicontroller::on_buttonSendArbitrary_clicked()
{
    send_message(ui->arbitrarySerialLine->text().simplified()); //simplified removes whitespace from front and back
    ui->arbitrarySerialLine->clear();
    history_pos=0;
}
void gpicontroller::set_needle_depth(QString data)
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(set_needle_depth(QString)));
    qDebug()<<"@SND "<<data;
    data=data.split(" ")[0];//discard second zero if it comes
    float data_s=data.toFloat()*.05;
    qDebug()<<"@SND "+QString::number(data_s);
    send_message("@SND "+QString::number(data_s));
    send_message("@SDF");//Saves the present values for inj needle depth, syringe speed, rinse strokes, temp set, and temp controller condition to the CF
}
void gpicontroller::needle_timeout()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(set_needle_depth(QString)));
//
    timer->stop();
    qDebug()<<"timed out!";
    ui->console->append("<div style='color:red'>timed out!</div>");
}
void gpicontroller::on_buttonSetDepth_clicked()
{
    if (QApplication::queryKeyboardModifiers()==Qt::ShiftModifier)
    {
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(set_needle_depth(QString)));
        send_message("@RDS");
    }
    else
    {
        send_message("@SND "+QString::number(ui->spinboxSetInjDepth->value()));
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(needle_depth_waiter()));
    }
}

void gpicontroller::needle_depth_waiter()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(needle_depth_waiter()));
    send_message("@SDF");//Saves the present values for inj needle depth, syringe speed, rinse strokes, temp set, and temp controller condition to the CF
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(needle_depth_waiter_2()));
}

void gpicontroller::needle_depth_waiter_2()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(needle_depth_waiter_2()));
    send_message("@HOME");//Saves the present values for inj needle depth, syringe speed, rinse strokes, temp set, and temp controller condition to the CF
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonGoToDepthSetpoint_clicked()));
}

void gpicontroller::on_buttonGoToDepthSetpoint_clicked()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonGoToDepthSetpoint_clicked()));
    send_message("@NDL 99");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonRefreshEncoder_clicked()));
}

void gpicontroller::on_buttonSetSerialNumber_clicked()
{
    serialNumber=ui->spinBoxSerialNumber->value();
    setting_serial=true;
    qDebug()<<"setting serial";
    send_message("#");
}

void gpicontroller::on_buttonHomeNeedle_clicked()
{
    send_message("@NDL 0");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonRefreshEncoder_clicked()));
}

void gpicontroller::on_buttonRefreshTempState_clicked()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonRefreshTempState_clicked()));
    send_message("@GSIP 12");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_temp_buttons(QString)));
}
void gpicontroller::update_temp_buttons(QString data)
{
    if (data=="OFF"){
        ui->buttonTempOff->setEnabled(false);
        ui->buttonTempOn->setEnabled(true);
    }
    else {
        ui->buttonTempOff->setEnabled(true);
        ui->buttonTempOn->setEnabled(false);
    }
    qDebug() << data;
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_temp_buttons(QString)));
    send_message("@RTT");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(set_temperature_bar(QString)));
}
void gpicontroller::set_temperature_bar(QString data)
{
    qDebug() <<data;
    ui->tempBar->setValue(data.toFloat());
    if (ui->buttonTempOff->isEnabled())
        ui->tempBar->setEnabled(true);
    else
        ui->tempBar->setEnabled(false);
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(set_temperature_bar(QString)));
}
void gpicontroller::on_buttonSetTemp_clicked()
{
    int number=ui->spinBoxTemperature->value();
    send_message("@STT "+QString::number(number));
}

void gpicontroller::on_buttonTempOn_clicked()
{
    send_message("@TPCL ON");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonRefreshTempState_clicked()));
}

void gpicontroller::on_buttonTempOff_clicked()
{
    send_message("@TPCL OFF");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonRefreshTempState_clicked()));
}

void gpicontroller::on_buttonRinse_clicked()
{
    QString number=QString::number(ui->spinboxStrokes->value());
    send_message("@IRNS 100,"+number);
}

void gpicontroller::on_buttonGetNeedleDepthSetPoint_clicked()
{
    send_message("@");
    connect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_needle_depth(QString)));
}
void gpicontroller::get_needle_depth(QString data)
{
    QStringList datal;
    if (data=="0")
        send_message("@DDF");
    else
        datal=data.split(",");
    if (datal.length()==5)
    {
            disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(get_needle_depth(QString)));
            data=datal[0];
            qDebug() << data.toFloat();
            float val=data.toFloat();
            ui->spinboxSetInjDepth->setValue(val);
            ui->spinboxNeedle->setValue(val);
            ui->console->append("<div style='color:LimeGreen'>Needle SetPoint is: "+data+"</div>");
            QScrollBar *vsb = ui->console->verticalScrollBar();
            QScrollBar *hsb = ui->console->horizontalScrollBar();
            vsb->setValue(vsb->maximum());
            hsb->setValue(hsb->minimum());
    }
}


void gpicontroller::on_buttonCRefresh_clicked()
{
    if (ui->buttonCRefresh->text()=="Lock Refresh")
    {
        ui->buttonCRefresh->setText("Unlock Refresh");
        const bool val=false;
        ui->buttonHome->setEnabled(val);
        ui->buttonPark->setEnabled(val);
        ui->buttonHomeNeedle->setEnabled(val);
        ui->buttonMove->setEnabled(val);
        ui->buttonSetDepth->setEnabled(val);
        ui->buttonGoToDepthSetpoint->setEnabled(val);
        ui->buttonGetNeedleDepthSetPoint->setEnabled(val);
        ui->buttonSendArbitrary->setEnabled(val);
        ui->buttonConnect->setEnabled(val);
        ui->buttonTempOff->setEnabled(val);
        ui->buttonTempOn->setEnabled(val);
        ui->buttonSelectVial->setEnabled(val);
        ui->buttonRinse->setEnabled(val);
        ui->buttonSetTemp->setEnabled(val);
        ui->buttonSetSerialNumber->setEnabled(val);
        ui->buttonRefreshTempState->setEnabled(val);
        ui->buttonGetTemp->setEnabled(val);
        timer->setInterval(1100);
        connect(timer,SIGNAL(timeout()),this,SLOT(on_buttonRefreshTempState_clicked()));
        timer->start();
    }
    else
    {
        ui->buttonCRefresh->setText("Lock Refresh");
        const bool val=true;
        ui->buttonHome->setEnabled(val);
        ui->buttonPark->setEnabled(val);
        ui->buttonHomeNeedle->setEnabled(val);
        ui->buttonMove->setEnabled(val);
        ui->buttonSetDepth->setEnabled(val);
        ui->buttonGoToDepthSetpoint->setEnabled(val);
        ui->buttonGetNeedleDepthSetPoint->setEnabled(val);
        ui->buttonSendArbitrary->setEnabled(val);
        ui->buttonConnect->setEnabled(val);
        ui->buttonSelectVial->setEnabled(val);
        ui->buttonRinse->setEnabled(val);
        ui->buttonSetTemp->setEnabled(val);
        ui->buttonSetSerialNumber->setEnabled(val);
        ui->buttonRefreshTempState->setEnabled(val);
        ui->buttonGetTemp->setEnabled(val);
        timer->stop();
        disconnect(timer,SIGNAL(timeout()),this,SLOT(on_buttonRefreshTempState_clicked()));
        QTimer::singleShot(800,this,SLOT(on_buttonRefreshTempState_clicked()));//provide one final update to the temp state
    }
}

void gpicontroller::on_buttonGetTemp_clicked()
{
   send_message("@DDF");
   connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_temp_setpoint(QString)));
}
void gpicontroller::update_temp_setpoint(QString data)
{
    QStringList datal;
    datal=data.split(",");
    if (datal.length()==5)
    {
        disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_temp_setpoint(QString)));
        data=datal[3];
        qDebug() << data.toFloat();
        int val=int(data.toFloat()*.25-4);
        ui->spinBoxTemperature->setValue(val);
    }
}

void gpicontroller::on_spinboxXin_valueChanged(double val)
{
    ui->spinboxX->blockSignals(true);
    val/=.00719;
    qDebug()<<"double: "<<val;//needed for some reason
    const int ival=int(val);
    ui->spinboxX->setValue(ival);
    make_labels_normal_weight(ui->labelX);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxY->setValue(0);
    ui->spinboxZ->setValue(0);
    ui->spinboxSyringe->setValue(0);
    ui->spinboxX->blockSignals(false);
}

void gpicontroller::on_spinboxYin_valueChanged(double val)
{
    ui->spinboxY->blockSignals(true);
    val/=.00719;
    qDebug()<<"double: "<<val;//needed for some reason
    const int ival=int(val);
    ui->spinboxY->setValue(ival);
    make_labels_normal_weight(ui->labelY);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxZ->setValue(0);
    ui->spinboxX->setValue(0);
    ui->spinboxSyringe->setValue(0);
    ui->spinboxY->blockSignals(false);
}

void gpicontroller::on_spinboxZmm_valueChanged(double val)
{
    ui->spinboxZ->blockSignals(true);
    val/=.05;
    qDebug()<<"double: "<<val;//needed for some reason
    const int ival=int(val);
    ui->spinboxZ->setValue(ival);
    make_labels_normal_weight(ui->labelZ);
    ui->spinboxNeedle->blockSignals(true);
    ui->spinboxNeedle->setValue(0);
    ui->spinboxNeedle->blockSignals(false);
    ui->spinboxY->setValue(0);
    ui->spinboxX->setValue(0);
    ui->spinboxSyringe->setValue(0);
    ui->spinboxZ->blockSignals(false);
}

void gpicontroller::update_encoder_position()
{
   disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
   send_message("@RDZ");
   connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position_reciever(QString)));
}
void gpicontroller::update_encoder_position_reciever(QString data)
{
    data=QString::number(data.toDouble()*.05);
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position_reciever(QString)));
    ui->labelEncoder->setText(data);
}


void gpicontroller::on_buttonRefreshEncoder_clicked()
{
    disconnect(this,SIGNAL(data_was_read(QString)),this,SLOT(on_buttonRefreshEncoder_clicked()));
    update_encoder_position();
}

void gpicontroller::on_buttonNeedleUp_clicked()
{
    if (QApplication::queryKeyboardModifiers()==Qt::ShiftModifier)
    {
        send_message("@MVZ -120");
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
    }
    else
    {
        send_message("@MVZ -5");
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
    }
}

void gpicontroller::on_buttonNeedleDown_clicked()
{
    if (QApplication::queryKeyboardModifiers()==Qt::ShiftModifier)
    {
        send_message("@MVZ 120");
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
    }
    else
    {
        send_message("@MVZ 5");
        connect(this,SIGNAL(data_was_read(QString)),this,SLOT(update_encoder_position()));
    }
}

void gpicontroller::on_buttonUp_clicked()
{
    send_message("@MVY 35");
}

void gpicontroller::on_buttonDown_clicked()
{
    send_message("@MVY -35");
}

void gpicontroller::on_buttonLeft_clicked()
{
    send_message("@MVX -35");
}

void gpicontroller::on_buttonRight_clicked()
{
    send_message("@MVX 35");
}
