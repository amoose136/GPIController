#include "superqlineedit.h"
#include <QDebug>
superQLineEdit::superQLineEdit(QWidget *parent) : QLineEdit(parent)
{
    installEventFilter(this);
}
bool superQLineEdit::eventFilter(QObject *anObject, QEvent *anEvent)
{
  if (anEvent->type() == QEvent::KeyRelease)
  {
      QKeyEvent *key= (QKeyEvent *)anEvent;
      if (key->key() == Qt::Key_Up)
      {
        qDebug()<<"up";
        emit upPressed();
        return true;
      }
      else if (key->key() == Qt::Key_Down)
      {
        qDebug()<<"down";
        emit downPressed();
        return true;
      }
  }
  return false;
  return QLineEdit::eventFilter(anObject, anEvent);
}
