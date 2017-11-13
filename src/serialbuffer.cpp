#include "serialbuffer.h"
#include <QString>
/////////////////////////////////////////
//bits to handle all the data conversions
template<>
int command<int>::auto_convert(QString d){
   return d.toInt();
}
template<>
bool command<bool>::auto_convert(QString d){
    if (d.toLower()=="true"||d=="1")
        return true;
    else
        return false;
}
template<>
QString command<QString>::auto_convert(QString d){
    return d.simplified();
}
template<>
double command<double>::auto_convert(QString d){
    return d.toDouble();
}
template<>
float command<float>::auto_convert(QString d){
    return d.toFloat();
}

int serialBuffer::data_recieved(QString data)
{
    if (waiting)
    {
     last.set_val(data);
     return 0;
    }
    else
        return 0;
}

