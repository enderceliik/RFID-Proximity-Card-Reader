#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>

#include "synchronization.h"
#include "functions.h"

class Database
{
public:
    Database();
    ~Database();
    void database_definer();
    QSqlQuery get_device_port_configuration();
    bool control_card_id(QString);
    QSqlQuery get_fuel_amount(int);
    QSqlQuery get_fuel_price();
    void update_fuel_amount(QMap<QString, QVariant>);
    QSqlQuery get_fuel_price_all();
    QSqlQuery get_fuel_amount_degree_all();
    void insert_sale(QMap<QString, QVariant>);
    QSqlQueryModel* get_table();


private:
    static QSqlDatabase db_object;
};
struct my_struct
{
    int id;
    QString card_id;
    QString name;
    double fuel;
    double fuel_price;
    int tank_id;
};

extern my_struct my_stc;
#endif // DATABASE_H
