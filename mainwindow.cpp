// https://github.com/enderceliik
// Ender CELIK

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);
    parameterMap.insert("Registered officer found" , "/images/checkmark.png");
    parameterMap.insert("Registered officer not found" , "/images/warning.png");
    parameterMap.insert("Port did not open" , "/images/block.png");
    parameterMap.insert("Card not read" , "/images/warning.png");
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),this, SLOT(updateInterface()));
    timerObject = new QTimer(this);
    connect(timerObject,SIGNAL(timeout()),this,SLOT(updateClockLalel()));
    timerObject->start(100);
    ui->setupUi(this);
    ui->groupBox_2->hide();
    ui->groupBox_3->hide();
    ui->groupBox_4->hide();
    ui->groupBox_5->hide();
    ui->pushButton_15->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uiItemsSetEnabled(bool parameter)
{
    ui->pushButton_7->setEnabled(parameter);
    ui->pushButton_8->setEnabled(parameter);
    ui->pushButton_9->setEnabled(parameter);
    ui->pushButton_10->setEnabled(parameter);
    ui->pushButton_12->setEnabled(parameter);
    ui->pushButton_13->setEnabled(parameter);
    ui->pushButton_14->setEnabled(parameter);
    ui->groupBox->setEnabled(parameter);
}

void MainWindow::afterProcessUpdateUI(QString parameter)
{
    if(parameter == "Port did not open")
    {
        ui->label_3->setStyleSheet("border-image: url(:" + parameterMap.value(parameter) + ");");
        ui->label_4->setText(parameter);
        ui->groupBox_2->show();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Failed to connect with Reader. After checking your cables, turn them on again.!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        if(QMessageBox::Ok)
        {
            QApplication::quit();
        }
    }
    else if(parameter == "Registered officer found")
    {
        ui->groupBox_6->hide();
        uiItemsSetEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->label_6->setText(my_stc.name);
        ui->groupBox_3->show();
    }
    else
    {
        QTimer::singleShot(3000, this, SLOT(warningTimer()));
        ui->label_3->setStyleSheet("border-image: url(:" + parameterMap.value(parameter) + ");");
        ui->label_4->setText(parameter);
        ui->groupBox_2->show();
    }
}

void MainWindow::warningTimer()
{
    ui->groupBox_2->hide();
    uiItemsSetEnabled(true);
}

void MainWindow::fuelUITimer()
{
    ui->groupBox_3->hide();
    tank_buttons(ui->pushButton_10);
    uiItemsSetEnabled(true);
    ui->label->clear();
    ui->label_5->clear();
    ui->label_6->clear();
    ui->groupBox_6->show();
}

void MainWindow::updateClockLalel()
{
    ui->label_2->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::on_pushButton_clicked()
{
   tank_buttons(ui->pushButton);
}

void MainWindow::on_pushButton_2_clicked()
{
    tank_buttons(ui->pushButton_2);
}

void MainWindow::tank_buttons(QPushButton *obj)
{
    if(ui->pushButton == obj)
    {
        tank_data = 0;
        obj->setStyleSheet("QPushButton{"
                           "border:none;"
                           "background-color: rgba(255, 163, 0, 1);}");
        ui->pushButton_2->setStyleSheet(
                    "QPushButton{"
                    "border:none;"
                    "background-color: rgba(173, 235, 253, 255);}"
                    "QPushButton::pressed {"
                    "border:none;"
                    "background-color: rgba(255, 163, 0, 1);}");
    }
    else if(ui->pushButton_2 == obj)
    {
        tank_data = 2;
        obj->setStyleSheet("QPushButton{"
                           "border:none;"
                           "background-color: rgba(255, 163, 0, 1);}");
        ui->pushButton->setStyleSheet(
                    "QPushButton{"
                    "border:none;"
                    "background-color: rgba(173, 235, 253, 255);}"
                    "QPushButton::pressed {"
                    "border:none;"
                    "background-color: rgba(255, 163, 0, 1);}");
    }
    else
    {
        ui->pushButton->setStyleSheet(
                    "QPushButton{"
                    "border:none;"
                    "background-color: rgba(173, 235, 253, 255);}"
                    "QPushButton::pressed {"
                    "border:none;"
                    "background-color: rgba(255, 163, 0, 1);}");
        ui->pushButton_2->setStyleSheet(
                    "QPushButton{"
                    "border:none;"
                    "background-color: rgba(173, 235, 253, 255);}"
                    "QPushButton::pressed {"
                    "border:none;"
                    "background-color: rgba(255, 163, 0, 1);}");
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    tank_data = -1;
    ui->pushButton->setStyleSheet(
                "QPushButton{"
                "border:none;"
                "background-color: rgba(173, 235, 253, 255);}"
                "QPushButton::pressed {"
                "border:none;"
                "background-color: rgba(255, 163, 0, 1);}");
    ui->pushButton_2->setStyleSheet(
                "QPushButton{"
                "border:none;"
                "background-color: rgba(173, 235, 253, 255);}"
                "QPushButton::pressed {"
                "border:none;"
                "background-color: rgba(255, 163, 0, 1);}");
}

void MainWindow::on_pushButton_10_clicked() // 10 start 11 finish
{
    if(tank_data != -1)
    {
        ui->pushButton_11->setEnabled(true);
        Database * db_object = new Database();
        fuel_start_situation = db_object->get_fuel_amount(tank_data).value(0).toDouble();

        if(fuel_start_situation > 0.0)
        {
            my_stc.tank_id = tank_data;
            my_stc.fuel_price = db_object->get_fuel_price().value(0).toDouble();
            my_stc.fuel = fuel_start_situation;
            startThread(false);
            uiItemsSetEnabled(false);
        }
        delete db_object;
    }
}

void MainWindow::afterFuelProcessUpdateUI(double total_fuel)
{
    QMap<QString, QVariant> updateMap;
    updateMap.insert("fuel",total_fuel);
    updateMap.insert("tank_id", tank_data);
    Database *db_object = new Database();
    db_object->update_fuel_amount(updateMap);
    updateMap.clear();
    double fee = total_fuel * my_stc.fuel_price;
    updateMap.insert("officerID", my_stc.card_id);
    updateMap.insert("fuel_amount", total_fuel);
    updateMap.insert("fee", fee);
    updateMap.insert("tank_id", my_stc.tank_id);
    db_object->insert_sale(updateMap);
    delete db_object;
    ui->label->setText(QString::number(total_fuel) + " lt");
    ui->label_5->setText(QString::number(fee) + " €");
    QTimer::singleShot(3000, this, SLOT(fuelUITimer()));
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->pushButton_11->setEnabled(false);
    stopThreadFuel();
}

void MainWindow::updatFuelValue(double parameter)
{
    ui->label->setText(QString::number(parameter) + " lt");
    ui->label_5->setText(QString::number(parameter * my_stc.fuel_price) + " €");
}

void MainWindow::on_pushButton_13_clicked()
{
    ui->groupBox_3->hide();
    tank_buttons(ui->pushButton_10);
    uiItemsSetEnabled(true);
    ui->label->clear();
    ui->label_5->clear();
    ui->label_6->clear();
    ui->groupBox_6->show();
}

void MainWindow::updateInterface()
{
    Database * db_object = new Database();
    QSqlQuery query;
    if(update_ui_condition)
    {
        query = db_object->get_fuel_amount_degree_all();
        QSqlQueryModel *model = db_object->get_table();
        ui->tableView->setModel(model);
        delete db_object;
        ui->label_35->setText("Tank ID: " + QString::number(query.value(0).toInt()));
        int tank_percent_value1 = (query.value(1).toDouble() * 100.0)/500.0;
        QString tank_degre1 = query.value(2).toString();
        ui->label_36->setText("Fuel tank filling ratio: (" + QString::number(query.value(1).toDouble()) + " lt)");
        query.next();
        ui->label_37->setText("Tank ID: " + QString::number(query.value(0).toInt()));
        int tank_percent_value2 = (query.value(1).toDouble() * 100.0)/500.0;
        QString tank_degre2 = query.value(2).toString();
        ui->label_38->setText("Fuel tank filling ratio: (" + QString::number(query.value(1).toDouble()) + " lt)");
        ui->progressBar->setValue(tank_percent_value1);
        ui->progressBar_2->setValue(tank_percent_value2);
        ui->label_33->setText("Temperature: " + tank_degre1 + " C°");
        ui->label_34->setText("Temperature: " + tank_degre2 + " C°");
    }
    else
    {
        query = db_object->get_fuel_price_all();
        delete db_object;
        ui->label_7->setText(query.value(0).toString());
        query.next();
        ui->label_9->setText(query.value(0).toString());
        query.next();
        ui->label_15->setText(query.value(0).toString());
        query.next();
        ui->label_17->setText(query.value(0).toString());
    }

}
void MainWindow::on_pushButton_7_clicked() // Card Reader Start Button
{
    uiItemsSetEnabled(false);
    startThread(true);
}

void MainWindow::on_pushButton_8_clicked() // Tank Monitoring Screen Button
{
    init_state_sell_and_tank_ui(true);
    update_ui_condition = true;
    timer->start(7000);
    ui->groupBox_6->hide();
    ui->pushButton_15->show();
    ui->groupBox_5->show();
}

void MainWindow::on_pushButton_14_clicked() // Sales Screen Button
{
    init_state_sell_and_tank_ui(false);
    update_ui_condition = false;
    timer->start(7000);
    ui->groupBox_6->hide();
    ui->pushButton_15->show();
    ui->groupBox_4->show();
}

void MainWindow::on_pushButton_12_clicked() // Home Screen Exit Button
{
    QApplication::quit();
}

void MainWindow::on_pushButton_15_clicked() // Exit button from tank monitoring and sales screen
{
    ui->pushButton_15->hide();
    ui->groupBox_5->hide();
    ui->groupBox_4->hide();
    timer->stop();
    ui->groupBox_6->show();
}
void MainWindow::init_state_sell_and_tank_ui(bool parameter)
{
    Database * db_object = new Database();
    QSqlQuery query;
    if(parameter)
    {
        query = db_object->get_fuel_amount_degree_all();
        delete db_object;
        int tank_percent_value1 = (query.value(1).toDouble() * 100.0)/500.0;
        QString tank_degre1 = query.value(2).toString();
        ui->label_36->setText("Fuel tank filling ratio: (" + QString::number(query.value(1).toDouble()) + " lt)");
        ui->label_35->setText("Tank ID: " + QString::number(query.value(0).toInt()));
        query.next();
        ui->label_37->setText("Tank ID: " + QString::number(query.value(0).toInt()));
        int tank_percent_value2 = (query.value(1).toDouble() * 100.0)/500.0;
        QString tank_degre2 = query.value(2).toString();
        ui->label_38->setText("Fuel tank filling ratio: (" + QString::number(query.value(1).toDouble()) + " lt)");
        ui->progressBar->setValue(tank_percent_value1);
        ui->progressBar_2->setValue(tank_percent_value2);
        ui->label_33->setText("Temperature: " + tank_degre1 + " C°");
        ui->label_34->setText("Temperature: " + tank_degre2 + " C°");
    }
    else
    {
        query = db_object->get_fuel_price_all();
        QSqlQueryModel *model = db_object->get_table();
        ui->tableView->setModel(model);
        ui->tableView->show();
        delete db_object;
        ui->label_7->setText(query.value(0).toString());
        query.next();
        ui->label_9->setText(query.value(0).toString());
        query.next();
        ui->label_15->setText(query.value(0).toString());
        query.next();
        ui->label_17->setText(query.value(0).toString());
    }
}

