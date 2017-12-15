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
        QVector<QString> sendHistory;
        QSerialPort* port;
        void *timeout_sig;
        bool setting_serial=false;
        int serialNumber=11111;
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
        void update_temp_buttons(QString);
        void read_data();
        void set_needle_depth(QString);
        void needle_timeout();
        void set_temperature_bar(QString);
        void get_needle_depth(QString);
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
        void on_buttonHomeX_clicked();
        void on_buttonHomeY_clicked();
        void on_buttonHomeZ_clicked();
        void on_buttonSendArbitrary_clicked();
        void on_arbitrarySerialLine_returnPressed();

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

private:
        Ui::gpicontroller *ui;
        QString portname;
        void scrolldown();


};


#endif // GPICONTROLLER_H
