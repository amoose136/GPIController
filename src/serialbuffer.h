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
        void readAsynkPolledChanged(int value);
        void readAsynkPolledChanged(float value);
        void readAsynkPolledChanged(double value);
        void readAsynkPolledChanged(bool state);
};

class generic_command //wrapper class to get rid of template notation in vector so `vector<command<some_fixed_type>> commandlist` becomes `vector<command> commandlist`
{
    public:
        QString name;
        generic_command(){}//empty object
        generic_command(const QString n){
            name=n;
        }
        //copy constructor required by vector
        generic_command(const generic_command &other);
        //assignment operator required by vector
        generic_command &operator=(generic_command &other){
            return other;
        }
//        virtual void* get_val();
        virtual void set_val(QString){}
        virtual ~generic_command(){}
};

template <class T>
class command : public SignalsSlots,public generic_command{
    protected:
        T* data;
        QString type;
    public:
        QString name;
        command():generic_command(){}
        command(const QString &n,T* d): generic_command(n){
            data=d;
            type=typeid(T).name();
            name=n;
            qDebug()<<"command created "<<name;
        }
        //copy constructor required by vector
        command(const command &other);
        //assignment operator required by vector
        T auto_convert(QString);
        void set_val(QString d) override {
            T data_con=auto_convert(d);
            *data=data_con;
            qDebug() << "ahh";
            emit readAsynkPolledChanged(*data);
        }
        void writeAsync(QString value) override {
            set_val(value);
        }
//        void* get_val() override {
//            return data;
//        }

 };




class serialBuffer : public QObject
{
    Q_OBJECT
    public:
        //constructors
        explicit serialBuffer():QObject(){
            waiting=false;
            last=new generic_command;
        };
        explicit serialBuffer(const serialBuffer &other):QObject(){ //copying
            waiting=other.waiting;
            commandList=other.commandList;
            last=other.last;
            commandList2=other.commandList2;
        };
        virtual ~serialBuffer(){} //destructor

        //member data items
        bool waiting; // the state of the read/write buffer
        QVector<generic_command> commandList; //vector of (command_name,pointer to data)
        generic_command* last; // the last command and data item pair
        QVector<command<int>> commandList2;

        //member functions
        template<class T>
        int append(const QString &c,T* p)
        {
//            if (commandList.)
//            {
//                command<T>(c,p);
//                command<T> co (c,p);
//                command<T> cn (c+"e",p);
//                qDebug()<<co.name <<cn.name;
//                co=cn;
//                QVector<command<T>> d;
//                d.push_back(co);
//                qDebug()<<co.name <<cn.name;
//                commandList2[0]=co;
//                last=commandList.back();
                return 0;
//            }
//            else
//                return -1;//buffer out of bounds
        }
//        template<class T>1
//        int append(command<T> &comm){
////            if (commandList.length()<10)
////            {
//                commandList.push_back(comm);
//                return 0;
////            }
////            else
////                return -1;//buffer out of bounds
//        }
        int send_next();
    signals:
        void send_data(QString);
        void processed_data(int);
    public slots:
        void data_recieved(QString);
//        template<class T>
//        int append(command<T> comm) {
//            if (append(comm)==0) {
////                connect(comm,SIGNAL(comm.readAsynkPolledChanged(),F,SLOT(F))
//                return 0;
//            }
//            else {
//                return -1;//buffer out of bounds
//            }

//        }

};


#endif // SERIALBUFFER_H
