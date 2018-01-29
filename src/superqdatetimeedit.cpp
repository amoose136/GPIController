#include "superqdatetimeedit.h"

superQDateTimeEdit::superQDateTimeEdit(QWidget *parent) : QDateTimeEdit(parent)
{
    installEventFilter(this);
}

bool superQDateTimeEdit::eventFilter(QObject *anObject, QEvent *anEvent)
{
    if (anEvent->type() == QEvent::KeyPress)
    {
        QKeyEvent *key= (QKeyEvent *)anEvent;
        if((key->key() == Qt::Key_Return) || (key->key() == Qt::Key_Enter))
        {
          emit dateTimeChanged(this->dateTime());
          emit enterPressed();
          return true;
        }
    }
    return QDateTimeEdit::eventFilter(anObject, anEvent);
}
