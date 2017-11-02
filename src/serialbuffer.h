#ifndef SERIALBUFFER_H
#define SERIALBUFFER_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QVector>
#include <QString>

template <typename T>
class command
{
    public:
        T* return_var;
        QString command_name;
        command(QString, T*);
};

class serialBuffer
{
    Q_OBJECT
    public:
        serialBuffer();
        ~serialBuffer();
        QVector<command<void*>> bufferList; //hopefully void* would except for example `int*` or `bool*`
        command<void*> lastSent;
    slots:
        void update_buffer(QString data);
    private:
        bool readInProgress=false;
};


#endif // SERIALBUFFER_H
