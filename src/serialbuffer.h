#ifndef SERIALBUFFER_H
#define SERIALBUFFER_H

#include <QString>
#include <QVector>

class generic_command
{
    public:
        QString name;
        //Instiation format is command name, empty variable pointer that will later contain data
        generic_command(const QString n):
            name (n)
        {};
        generic_command(){};
//        virtual void get_val();
        virtual int set_val(QString);
};

template <class T>
class command : public generic_command{
    protected:
        T* data;
    public:
        command(QString n,T* d) : generic_command(n){
            data=d;
        }

        T get_val();
        T auto_convert(QString d)
        {
            switch (typeof(T)){
                case typeof(int):
                    return d.toInt();
                case typeof(bool):
                {
                    if (d.toLower()=='true'||d=='1')
                        return true;
                    else
                        return false;
                }
                case typeof(double):
                    return d.toDouble();
            }
        };
        int set_val(QString d) override {
            T data_con=auto_convert(d,T)
            *data=data_con;
            return 0;
        }; //returns zero on success
};


class serialBuffer
{
    public:
//        serialBuffer(QWidget *parent = 0): QObject(parent) {};
        bool waiting=false;
        QVector<generic_command> commandList; //command_name,type
        generic_command last;
        int data_recieved(QString);
        template<class T>
        int append(QString c,T* p)
        {
            if (commandList.length()<100)
            {
                const command<T> comm (c,p);
                commandList.push_back(comm);
                return 0;
            }
            else
                return -1;//buffer out of bounds
        };
};



#endif // SERIALBUFFER_H
