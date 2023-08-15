#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <QSharedPointer>
#include <QObject>

#include "serialcom.h"


class Synchronization : public QObject
{
    Q_OBJECT
public:
    Synchronization(int);
    QSharedPointer<SerialCom>ext_serial_obj2;
    QByteArray read();
};
extern QSharedPointer<Synchronization>ext_sync_obj;
extern QSharedPointer<Synchronization>glob_pointer_array_sync[1];
#endif // SYNCHRONIZATION_H
