#include "serialbuffer.h"
#include <QString>




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

