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
        void openport(QString portname);
        QString getport();
        QSerialPortInfo* portinfo;
        void refresh_comBox();
        QSerialPort* port;
        QString readData;
        void make_labels_normal_weight(QLabel* element);

    private slots:
        void on_buttonSelectVial_clicked();
        void on_buttonRefresh_clicked();
        void on_buttonConnect_clicked();
        void read_data();

        void on_buttonHome_clicked();
        void sendmessage(QString message);

        void on_buttonPark_clicked();

        void spinboxX_valueChanged();
        void spinboxY_valueChanged();
        void spinboxZ_valueChanged();
        void spinboxNeedle_valueChanged();
        void spinboxSyringe_valueChanged();

        void on_buttonMove_clicked();
        void on_buttonHomeX_clicked();
        void on_buttonHomeY_clicked();
        void on_buttonHomeZ_clicked();
        void on_buttonSendArbitrary_clicked();
        void on_arbitrarySerialLine_returnPressed();

private:
        Ui::gpicontroller *ui;
        QString portname;
};


#endif // GPICONTROLLER_H
