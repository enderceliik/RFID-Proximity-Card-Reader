// https://github.com/Enderceliik
// Ender CELIK

#include "serialcom.h"
QSharedPointer<SerialCom>ext_serial_obj;
QSharedPointer<SerialCom>glob_pointer_array[1] = {nullptr};
SerialCom::SerialCom(QJsonObject port_config)
{
    serial_port_obj = new QSerialPort();

    port_definer(port_config);
    protocol_definer();
    open_serial_port();
}

void SerialCom::port_definer(QJsonObject port_config)
{
    QString portName = port_config["portName"].toString();
    QString baudRate = port_config["baudRate"].toString();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(port_config["dataBits"].toString().toInt());

    QString parityStr = port_config["parity"].toString();
    QSerialPort::Parity parity;
    if (parityStr == "EvenParity") {
        parity = QSerialPort::EvenParity;
    } else if (parityStr == "OddParity") {
        parity = QSerialPort::OddParity;
    } else if (parityStr == "NoParity") {
        parity = QSerialPort::NoParity;
    } else {
        parity = QSerialPort::NoParity;
    }

    QString stopBitsStr = port_config["stopBits"].toString();
    QSerialPort::StopBits stopBits;
    if (stopBitsStr == "OneStop") {
        stopBits = QSerialPort::OneStop;
    } else if (stopBitsStr == "OneAndHalfStop") {
        stopBits = QSerialPort::OneAndHalfStop;
    } else if (stopBitsStr == "TwoStop") {
        stopBits = QSerialPort::TwoStop;
    } else {
        stopBits = QSerialPort::OneStop;
    }

    QString flowControlStr = port_config["flowControl"].toString();
    QSerialPort::FlowControl flowControl;
    if (flowControlStr == "NoFlowControl") {
        flowControl = QSerialPort::NoFlowControl;
    } else if (flowControlStr == "HardwareControl") {
        flowControl = QSerialPort::HardwareControl;
    } else if (flowControlStr == "SoftwareControl") {
        flowControl = QSerialPort::SoftwareControl;
    } else {
        flowControl = QSerialPort::NoFlowControl;
    }

    serial_port_obj->setPortName(portName);
    serial_port_obj->setBaudRate(baudRate.toInt());
    serial_port_obj->setDataBits(dataBits);
    serial_port_obj->setParity(parity);
    serial_port_obj->setStopBits(stopBits);
    serial_port_obj->setFlowControl(flowControl);
}

void SerialCom::protocol_definer()
{
    for (int num = 0; num < prtclString.length(); ++num) {
        if (num%2==0) {
            protocolList+=prtclString.mid(num,2);
        }
    }
    for (int var = 0; var < #; ++var) {
        protocolByteArray.append(QString(protocolList.value(var)).toUInt(nullptr,16));
    }
}

void SerialCom::open_serial_port()
{
    if (serial_port_obj->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial Port open successfuly!";
    }
    else
    {
        qDebug() << "Serial Port didn't open!" << serial_port_obj->error();
    }
}

QByteArray SerialCom::communication()
{
    QByteArray response;
    if (serial_port_obj->isOpen()) {
        serial_port_obj->write(protocolByteArray);
        if (serial_port_obj->waitForBytesWritten(3000)) {
            QThread::sleep(3);
            //response = ext_serial_obj->read_data();
            response = read_data();
        }
    }
    return response;
}

QByteArray SerialCom::read_data()
{
    QByteArray response;
    if (serial_port_obj->waitForReadyRead(5000)) {
        response.append(serial_port_obj->readAll());
    }
    return response;
}

