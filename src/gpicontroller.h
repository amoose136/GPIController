/*
 * Header for the gpicontroller app.
 * Author: Amos Manneschmidt
 * 2017
*/
#ifndef GPICONTROLLER_H
#define GPICONTROLLER_H

#include <QMainWindow>
#include <QSerialPort>
#include <QComboBox>

namespace Ui {
    class gpicontroller;
}

class gpicontroller : public QMainWindow
{
    Q_OBJECT

    public:
        explicit gpicontroller(QWidget *parent = 0);
        void sendCommand(QSerialPort* port);
        ~gpicontroller();
        QSerialPort* openport(QString portname);
        QString getport();
        QSerialPortInfo* portinfo;
        QSerialPort* port;

    private slots:
        void on_buttonSelectVial_clicked();
        void on_buttonRefresh_clicked();
        void on_buttonConnect_clicked();

    private:
        Ui::gpicontroller *ui;
        QString portname;
        void refresh_comBox();


};



#endif // GPICONTROLLER_H
