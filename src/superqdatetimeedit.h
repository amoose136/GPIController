#ifndef SUPERQDATETIMEEDIT_H
#define SUPERQDATETIMEEDIT_H
#include <QDateTimeEdit>
#include <QEvent>
#include <QObject>
#include <QKeyEvent>

class superQDateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
    public:
        superQDateTimeEdit(QWidget *parent=0);
    signals:
        void enterPressed();
    protected:
        bool eventFilter(QObject *anObject, QEvent *anEvent);
};

#endif // SUPERQDATETIMEEDIT_H
