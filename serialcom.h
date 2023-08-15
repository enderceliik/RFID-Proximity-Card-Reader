#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QSerialPort>
#include <QSharedPointer>
#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QDebug>
#include <QSettings>
class SerialCom : public QObject
{
    Q_OBJECT
public:
    SerialCom(QJsonObject);

    QSerialPort *serial_port_obj;
    QByteArray communication();
    QByteArray read_data();

    void open_serial_port();
    void protocol_definer();

    QString prtclString = "####";
    QStringList protocolList;
    QByteArray protocolByteArray;
    void port_definer(QJsonObject);

};
//extern QSharedPointer<SerialCom>ext_serial_obj;
extern QSharedPointer<SerialCom>glob_pointer_array[1];
#endif // SERIALCOM_H
