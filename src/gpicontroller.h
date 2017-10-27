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
        void selectVial();
        ~gpicontroller();
        QSerialPort* openport(QString portname);
        QString getport();
        QSerialPortInfo* portinfo;
        static QSerialPort* port;
        void refresh_comBox();

    private slots:
        void on_buttonSelectVial_clicked();
        void on_buttonRefresh_clicked();
        void on_buttonConnect_clicked();

    private:
        Ui::gpicontroller *ui;
        QString portname;



};



#endif // GPICONTROLLER_H
