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

namespace Ui {
    class gpicontroller;
}

class gpicontroller : public QMainWindow
{
    Q_OBJECT

    public:
        explicit gpicontroller(QWidget *parent = 0);

        ~gpicontroller();

        QString readData;
        QSerialPortInfo* portinfo;
        QVector<QString> sendHistory;
        QSerialPort* port;
        bool readingState=false;
        QVector<QString> readBuffer;
        QVector<Qstring> sendBuffer;

        void open_port(QString portname);
        QString get_port();
        void refresh_comBox();
        void select_vial();
        void make_labels_normal_weight(QLabel* element);
        void request(int timeout);



    private slots:

        void read_data();

        void on_buttonHome_clicked();
        void send_message(QString message);



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

private:
        Ui::gpicontroller *ui;
        QString portname;
};


#endif // GPICONTROLLER_H
