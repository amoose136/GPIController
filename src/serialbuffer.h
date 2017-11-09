#ifndef SERIALBUFFER_H
#define SERIALBUFFER_H

#include <QString>
#include <QVector>
#include <QtCore>
#include <QObject>
#include <typeinfo>

class generic_command
{   protected:
        QString name;
    public:
        generic_command(const QString n){
            name=n;
        };
        generic_command(){};
        virtual void set_val(QString){};
        virtual ~generic_command(){};
};

template <class T>
class command : public generic_command{
    protected:
        T* data;
        QString type;
    public:
        command(const QString n,T* d): generic_command(n){
            data=d;
            type=typeid(T).name();
            qDebug()<<"command created "<<name;
        }

//        T get_val();
        T auto_convert(QString d)
        {
            QString n=typeid(T).name();
            if (n==typeid(int).name()){
                return d.toInt();
            }
            else if (n==typeid(bool).name())
            {
                if (d.toLower()=="true"||d=="1")
                        return true;
                    else
                        return false;
            }
            else if (n==typeid(double).name())
                    return d.toDouble();
            else
                return *data;
        };
        void set_val(QString d) override {
            T data_con=auto_convert(d);
            *data=data_con;
            qDebug() << "ahh";
        }; //returns zero on success
};


class serialBuffer : public QObject
{
    Q_OBJECT
    public:
        explicit serialBuffer():QObject(){
            waiting=false;
        };
        explicit serialBuffer(const serialBuffer &other):QObject(){
            waiting=other.waiting;
            commandList=other.commandList;
            last=other.last;
        };
        virtual ~serialBuffer(){};

        //member data items
        bool waiting; // the state of the read/write buffer
        QVector<generic_command*> commandList; //vector of (command_name,pointer to data)
        generic_command last; // the last command and data item pair


        //member functions
        int data_recieved(QString);
        template<class T>
        int append(QString c,T* p)
        {
            if (commandList.length()<100)
            {
                command<T>* comm = new command<T>(c,p);
                commandList.push_back(comm);
                last=*commandList.back();
                return 0;
            }
            else
                return -1;//buffer out of bounds
        };
};



#endif // SERIALBUFFER_H
