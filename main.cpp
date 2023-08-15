// https://github.com/Enderceliik
// Ender CELIK

#include "mainwindow.h"
#include <QApplication>

#include "database.h"
#include "serialcom.h"
#include "synchronization.h"
#include "QJsonObject"

void serial_port_initializer()
{
    Database* database_object = new Database();
    database_object->database_definer();
    QSqlQuery response_serial_config = database_object->get_device_port_configuration();
    delete database_object;
    QList<QJsonObject> config_list;
    for (int var = 0; var < 2; ++var) {
        QString json_str = response_serial_config.value(0).toString();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json_str.toUtf8());
        QJsonObject jsonObject = jsonDoc.object();
        config_list.append(jsonObject);
        response_serial_config.next();
    }


    for (int var = 0; var < 2; ++var) {

        glob_pointer_array[var].reset(new SerialCom(config_list[var]));
        if(!glob_pointer_array[var]->serial_port_obj->isOpen())
        {
            glob_pointer_array[var].clear();
            glob_pointer_array[var] = nullptr;
        }
    }

}

void sync_func(QThread *thread)
{
    for (int var = 0; var < 2; ++var) {
        if(glob_pointer_array[var] != nullptr)
        {
            glob_pointer_array_sync[var].reset(new Synchronization(var));
            glob_pointer_array_sync[var]->ext_serial_obj2->moveToThread(thread);
            glob_pointer_array_sync[var]->ext_serial_obj2->serial_port_obj->moveToThread(thread);
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();
    ProcessThread * executer_object = new ProcessThread();
    serial_port_initializer();
    sync_func(executer_object);
    executer_object->start();
    QObject::connect(executer_object, SIGNAL(afterProcessSignal(QString)),&w,SLOT(afterProcessUpdateUI(QString)));
    QObject::connect(&w, SIGNAL(startThread(bool)), executer_object, SLOT(threadStarter(bool)));
    QObject::connect(&w, SIGNAL(stopThreadFuel()), executer_object, SLOT(threadStopFuel()));
    QObject::connect(executer_object, SIGNAL(afterFuelProcessSignal(double)), &w, SLOT(afterFuelProcessUpdateUI(double)));
    QObject::connect(executer_object, SIGNAL(updateFuelValue(double)), &w, SLOT(updatFuelValue(double)));
    return a.exec();
}
