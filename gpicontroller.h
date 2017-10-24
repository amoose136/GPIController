#ifndef GPICONTROLLER_H
#define GPICONTROLLER_H

#include <QMainWindow>

namespace Ui {
class gpicontroller;
}

class gpicontroller : public QMainWindow
{
    Q_OBJECT

public:
    explicit gpicontroller(QWidget *parent = 0);
    void sendCommand();
    ~gpicontroller();

private slots:
    void on_send_Button_clicked();

private:
    Ui::gpicontroller *ui;
    QString portname;
};

#endif // GPICONTROLLER_H
