#include "qtcintspinbox.h"
#include <QDebug>

QtcIntSpinBox::QtcIntSpinBox(QWidget *parent) : QSpinBox(parent)
{
    installEventFilter(this);
}

bool QtcIntSpinBox::eventFilter(QObject *anObject, QEvent *anEvent)
{
  if (anEvent->type() == QEvent::KeyPress)
  {
      QKeyEvent *key= (QKeyEvent *)anEvent;
      if((key->key() == Qt::Key_Return) || (key->key() == Qt::Key_Enter))
      {
        emit valueChanged(this->value());
        emit enterPressed();
        return true;
      }
  }
  return QSpinBox::eventFilter(anObject, anEvent);
}
