#ifndef QTCINTSPINBOX_H
#define QTCINTSPINBOX_H


#include <QDoubleSpinBox>
#include <QObject>
#include <QEvent>
#include <QKeyEvent>

class QtcIntSpinBox : public QSpinBox
{
    Q_OBJECT
    public:
        QtcIntSpinBox(QWidget *parent=0);

    signals:
        void enterPressed();
    public slots:
    protected:
        bool eventFilter(QObject *anObject, QEvent *anEvent);
};

#endif // QTCINTSPINBOX_H
