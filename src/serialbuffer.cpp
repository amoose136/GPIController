#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QVector>
#include <QString>
#include <serialbuffer.h>
#include <gpicontroller.h>

command::command(QString N, T* r){
    return_var=r;
    command_name=N;
}

serialBuffer::update_buffer(QString data){
    if (readInProgress)
    {
        lastSent.return_var=data;

        lastSent=bufferList.back();
        send_message(lastSent.command_name);
    }
}
