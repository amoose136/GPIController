#ifndef SERIALBUFFER_H
#define SERIALBUFFER_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QVector>
#include <QString>
class generic_command
{
    protected:
        //possible datatypes:
        QString name;
    public:
        //Instiation format is command name, empty variable pointer that will later contain data
        generic_command(QString n){
            name=n;
        }
        virtual ~generic_command();
};

template <class T>
class command : generic_command{
    protected:
        T* data;
    public:
        command(QString n,T* d) : generic_command(n){
            data=d;
        }

        T get_val();
        int set_val(T); //returns zero on success
};

class serialBuffer : QObject
{
    Q_OBJECT
    public:
        serialBuffer();

        bool waiting=false;
        QVector<generic_command> commandList; //command_name,type
        generic_command last;
        int data_recieved(QString);
        template<class T>
        int append(QString,T*);

};


#endif // SERIALBUFFER_H
