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
    ~gpicontroller();

private:
    Ui::gpicontroller *ui;
    void on_send_Button_clicked();
};

#endif // GPICONTROLLER_H
