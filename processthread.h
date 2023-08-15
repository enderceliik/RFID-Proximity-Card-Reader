#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>
#include <QEventLoop>
#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QTimer>

class ProcessThread : public QThread
{
    Q_OBJECT
public:
    ProcessThread(){}
    ~ProcessThread(){
        quit();
        wait();
    }
    void run() override;
    bool databaseMethodProcessType; // true: add new ID to database ||  false: check ID for sell

signals:
    void afterProcessSignal(QString param);
    void afterFuelProcessSignal(double);
    void updateFuelValue(double);
public slots:
    void threadStarter(bool);
    void threadStopFuel();
private:
    QString messageForUpdateUI;
    bool threadWorkSitutation = false;
    bool threadFuelWorkSituation = false;
    double fuel_counter = 0.0;
    double total_fuel = 0.0;
    void database_com(QByteArray response);
private slots:
    void fifteenSecondControllerTerminate();
};

#endif // PROCESSTHREAD_H
