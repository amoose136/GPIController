#ifndef QTCSPINBOX_H
#define QTCSPINBOX_H

#include <QDoubleSpinBox>
#include <QObject>
#include <QEvent>
#include <QKeyEvent>

class QtcSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    public:
        QtcSpinBox(QWidget *parent=0);

    signals:

    public slots:
    protected:
        bool eventFilter(QObject *anObject, QEvent *anEvent);
};

#endif // QTCSPINBOX_H
