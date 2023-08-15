// https://github.com/enderceliik
// Ender CELIK

#include "processthread.h"
#include "serialcom.h"
#include "database.h"
#include "synchronization.h"

#include "mainwindow.h"

void ProcessThread::run()
{
    QEventLoop loop;
    QByteArray responseFromCommunication;
    while(true)
    {
        while(threadWorkSitutation)
        {
            qDebug() << "Serial port reading in progress...";
            responseFromCommunication = glob_pointer_array_sync[0]->read();
            if(responseFromCommunication.isEmpty())
            {
                emit afterProcessSignal("Port did not open");
                threadWorkSitutation = false;
                return;
            }
            if(!strcmp(responseFromCommunication, "#####") == 0) // if: If the response differs from the protocol we have sent
            {
                qDebug() << responseFromCommunication;
                database_com(responseFromCommunication);
            }
        }
        while(threadFuelWorkSituation)
        {
            qDebug() << "Fuel is being supplied...";
            fuel_counter = fuel_counter + 0.1;
            emit updateFuelValue(fuel_counter);
            msleep(200);
            loop.processEvents();
            if((fuel_counter >= my_stc.fuel) || (!threadFuelWorkSituation))
            {
                emit afterFuelProcessSignal(fuel_counter);
                threadFuelWorkSituation = false;
                fuel_counter = 0;
                break;
            }

        }
        while(true)
        {
            qDebug() << "Waiting ...";
            msleep(750);
            loop.processEvents();
            if(threadWorkSitutation||threadFuelWorkSituation)
                break;
        }
    }
}
void ProcessThread::database_com(QByteArray response)
{
    QByteArray responseHex = response.toHex();
    QString officerID = QString(responseHex.mid(#, #));
    qDebug() << officerID;
    Database* databaseObject = new Database();
    if(databaseObject->control_card_id(officerID)) // false: doesn't exist
    {
        messageForUpdateUI = "Registered officer found";
    }
    else
    {
        messageForUpdateUI = "Registered officer not found";
    }
    delete databaseObject;
    threadWorkSitutation = false;
    emit afterProcessSignal(messageForUpdateUI);
}
void ProcessThread::threadStarter(bool parameter)
{
    qDebug() << "Thread starting...";
    if(parameter)
    {
        this->threadWorkSitutation = true;
        QTimer::singleShot(10000, this, SLOT(fifteenSecondControllerTerminate()));
    }
    else
    {
        threadFuelWorkSituation = true;
    }

}
void ProcessThread::fifteenSecondControllerTerminate()
{
    if(threadWorkSitutation)
    {
        qDebug() << " 10 seconds ";
        threadWorkSitutation = false;
        emit afterProcessSignal("Card not read");
    }
}

void ProcessThread::threadStopFuel()
{
    threadFuelWorkSituation = false;
}
