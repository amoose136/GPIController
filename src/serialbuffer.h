#ifndef SERIALBUFFER_H
#define SERIALBUFFER_H

#include <QString>
#include <QVector>
#include <QtCore>
#include <QObject>

class SignalsSlots : public QObject //generic class to make the command class play nice with qt
{
    Q_OBJECT
    public:
        explicit SignalsSlots(QObject *parent = 0) :
        QObject(parent) {}
    public slots:
        virtual void writeAsync(int) {}
        virtual void writeAsync(float) {}
        virtual void writeAsync(double) {}
        virtual void writeAsync(bool) {}
        virtual void writeAsync(QString) {}

    signals:
        void readAsynkPolledChanged();
        void readAsynkPolledChanged(int value);
        void readAsynkPolledChanged(float value);
        void readAsynkPolledChanged(double value);
        void readAsynkPolledChanged(bool state);
};

class generic_command //wrapper class to get rid of template notation in vector so `vector<command<some_fixed_type>> commandlist` becomes `vector<command> commandlist`
{
    protected:
        QString name;
    public:
        generic_command(){}//empty object
        generic_command(const QString n){
            name=n;
        }
        virtual void set_val(QString){}
        virtual ~generic_command(){}
};

template <class T>
class command : public SignalsSlots,public generic_command{
    protected:
        T* data;
        QString type;
    public:
        T auto_convert(QString);
        void set_val(QString d) override {
            T data_con=auto_convert(d);
            *data=data_con;
            qDebug() << "ahh";
        }
        void writeAsync(QString value) override {
            set_val(value);
        }
        command(const QString n,T* d): generic_command(n){
            data=d;
            type=typeid(T).name();
            qDebug()<<"command created "<<name;
        }

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
        virtual ~serialBuffer(){}

        //member data items
        bool waiting; // the state of the read/write buffer
        QVector<generic_command*> commandList; //vector of (command_name,pointer to data)
        generic_command last; // the last command and data item pair


        //member functions
        int data_recieved(QString);
        template<class T>
        int append(QString c,T* p)
        {
            if (commandList.length()<10)
            {
                command<T>* comm = new command<T>(c,p);
                commandList.push_back(comm);
                last=*commandList.back();
                return 0;
            }
            else
                return -1;//buffer out of bounds
        };
        template<class T,class Functor>
        int append(QString c,T* p,Functor F) {
            if (append<T>(c,p)==0) {
                F(0);
                return 0;
            }
            else {
                F(-1);
                return -1;//buffer out of bounds
            }

        }

};


#endif // SERIALBUFFER_H
