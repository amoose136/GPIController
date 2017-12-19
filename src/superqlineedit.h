#ifndef SUPERQLINEEDIT_H
#define SUPERQLINEEDIT_H
#include <QLineEdit>
#include <QEvent>
#include <QObject>
#include <QKeyEvent>

class superQLineEdit : public QLineEdit
{
    Q_OBJECT
    public:
        superQLineEdit(QWidget *parent=0);
    signals:
        void upPressed();
        void downPressed();
    protected:
        bool eventFilter(QObject *anObject, QEvent *anEvent);
};

#endif // SUPERQLINEEDIT_H
