#include "qtcspinbox.h"
#include <QDebug>

QtcSpinBox::QtcSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
    installEventFilter(this);
}

bool QtcSpinBox::eventFilter(QObject *anObject, QEvent *anEvent)
{
  if (anEvent->type() == QEvent::KeyPress)
  {
      QKeyEvent *key= (QKeyEvent *)anEvent;
      if((key->key() == Qt::Key_Return) || (key->key() == Qt::Key_Enter))
      {
        qDebug() << "ahh";
        return true;
      }
  }
  return QDoubleSpinBox::eventFilter(anObject, anEvent);
}
