#include "gpicontroller.h"
#include "ui_gpicontroller.h"

gpicontroller::gpicontroller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gpicontroller)
{
    ui->setupUi(this);
}

gpicontroller::~gpicontroller()
{
    delete ui;
}
