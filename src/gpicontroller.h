/*
 * Header for the gpicontroller app.
 * Author: Amos Manneschmidt
 * 2017
*/
#ifndef GPICONTROLLER_H
#define GPICONTROLLER_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QComboBox>
#include <QLabel>
#include <QVector>
#include "serialBuffer.h"
#include <QTimer>
#include "qtcspinbox.h"

namespace Ui {
    class gpicontroller;
}

class gpicontroller : public QMainWindow
{
    Q_OBJECT

    public:
        explicit gpicontroller(QWidget *parent = 0);

        ~gpicontroller();
        serialBuffer* buffer;
        QString readData;
        QSerialPortInfo* portinfo;
        QSerialPort* port;

        QVector<QString> sendHistory;
        int history_pos=-1;

        void *timeout_sig;
        bool setting_serial=false;
        bool setting_date=false;
        int serialNumber=11111;

        int ipart;
        QString fpart;

        QTimer *timer=new QTimer(this);

        void open_port(QString portname);
        QString get_port();
        void refresh_comBox();
        void select_vial();
        void make_labels_normal_weight(QLabel* element);
        void send_message(QString message);

    signals:
        void data_was_read(QString);
    public slots:
        void timed_out();

    private slots:
        void unified_move_reciever();
        void update_temp_buttons(QString);
        void read_data();
        void set_needle_depth(QString);
        void needle_timeout();
        void update_encoder_position();
        void update_encoder_position_reciever(QString);
        void update_encoder_position_startup(QString);
        void set_temperature_bar(QString);
        void get_needle_depth(QString);
        void needle_depth_waiter();
        void needle_depth_waiter_2();
        void initialize(QString);
        void get_firmware(QString);
        void get_serial(QString);
        void update_temp_setpoint(QString);

        void on_buttonHome_clicked();

        void spinboxX_valueChanged();
        void spinboxY_valueChanged();
        void spinboxZ_valueChanged();
        void spinboxNeedle_valueChanged();
        void spinboxSyringe_valueChanged();

        void on_buttonSelectVial_clicked();
        void on_buttonRefresh_clicked();
        void on_buttonConnect_clicked();
        void on_buttonPark_clicked();
        void on_buttonMove_clicked();
//        void on_buttonHomeX_clicked();
//        void on_buttonHomeY_clicked();
//        void on_buttonHomeZ_clicked();
        void on_buttonSendArbitrary_clicked();
        void on_arbitrarySerialLine_returnPressed();
        void on_arbitrarySerialLine_upPressed();
        void on_arbitrarySerialLine_downPressed();

        void on_buttonSetDepth_clicked();

        void on_buttonGoToDepthSetpoint_clicked();

        void on_buttonSetSerialNumber_clicked();

        void on_buttonHomeNeedle_clicked();

        void on_buttonRefreshTempState_clicked();

        void on_buttonSetTemp_clicked();

        void on_buttonTempOn_clicked();

        void on_buttonTempOff_clicked();

        void on_buttonRinse_clicked();

        void on_buttonGetNeedleDepthSetPoint_clicked();

        void on_buttonCRefresh_clicked();

        void on_buttonGetTemp_clicked();

        void on_spinboxXin_valueChanged(double);

        void on_spinboxYin_valueChanged(double);

        void on_spinboxZmm_valueChanged(double);

        void on_buttonRefreshEncoder_clicked();

        void on_buttonNeedleUp_clicked();

        void on_buttonNeedleDown_clicked();

        void on_buttonUp_clicked();

        void on_buttonDown_clicked();

        void on_buttonLeft_clicked();

        void on_buttonRight_clicked();

        void on_buttonGetCurrentTime_clicked();

        void on_buttonUpdateMachineTime_clicked();

private:
        Ui::gpicontroller *ui;
        QString portname;
        void scrolldown();


};


#endif // GPICONTROLLER_H
