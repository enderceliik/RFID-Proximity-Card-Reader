#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QTime>
#include "processthread.h"

#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void uiItemsSetEnabled(bool parameter);
    ProcessThread* executerObject;

signals:
    void startThread(bool);
    void stopThreadFuel();

public slots:
    void afterProcessUpdateUI(QString);
    void afterFuelProcessUpdateUI(double);
    void updatFuelValue(double);
    void updateInterface();

private slots:
    void updateClockLalel();

    void warningTimer();

    void on_pushButton_7_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void tank_buttons(QPushButton *obj);

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void fuelUITimer();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* timerObject;
    QMap<QString, QString> parameterMap;
    QPushButton* objData;
    int tank_data = -1;
    double fuel_start_situation;
    QTimer *timer;
    void init_state_sell_and_tank_ui(bool);
    bool update_ui_condition;
};

#endif // MAINWINDOW_H
