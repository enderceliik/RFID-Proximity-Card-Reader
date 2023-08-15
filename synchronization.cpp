// https://github.com/Enderceliik
// Ender CELIK

#include "synchronization.h"

QSharedPointer<Synchronization>ext_sync_obj;
QSharedPointer<Synchronization>glob_pointer_array_sync[1] = {nullptr};
Synchronization::Synchronization(int index)
{
//    ext_serial_obj2 = ext_serial_obj;
    ext_serial_obj2 = glob_pointer_array[index];
}

QByteArray Synchronization::read()
{
    QByteArray ba;
    ba = ext_serial_obj2->communication();
    return ba;
}
