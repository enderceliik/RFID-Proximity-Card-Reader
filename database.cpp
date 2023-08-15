// https://github.com/Enderceliik
// Ender CELIK

#include "database.h"
my_struct my_stc;

#include <iostream>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
QSqlDatabase Database::db_object;
Database::Database()
{
    db_object = QSqlDatabase::addDatabase(Getvalue("DatabaseType"));
    db_object.setConnectOptions();
    db_object.setHostName(Getvalue("HostName"));
    db_object.setPort(Getvalue("Port").toInt());
    db_object.setDatabaseName(Getvalue("DatabaseName"));
    db_object.setUserName(Getvalue("UserName"));
    db_object.setPassword(Getvalue("password"));
    if(db_object.open())
    {
        qDebug() << "Connected to Database Succesfully!";
    }
    else
    {
        qDebug() << "Database connection error!";
    }
}

Database::~Database()
{
    db_object.close();
}

void Database::database_definer()
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    query.prepare("SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = 'card') AS table_existence;");
    query.exec();
    query.next();
    if(!query.value(0).toBool())
    {
        query.next();
        query.prepare("CREATE TABLE public.card"
                      "(id integer NOT NULL GENERATED ALWAYS AS IDENTITY ( INCREMENT 2 START 0 MINVALUE 0 MAXVALUE 2147483647 CACHE 1 ),"
                          "card_id TEXT NOT NULL,"
                          "name TEXT NOT NULL,"
                          "CONSTRAINT carf_pkey PRIMARY KEY (id))"
                      "WITH (OIDS = FALSE)"
                      "TABLESPACE pg_default;"
                      "ALTER TABLE public.card"
                          "OWNER to postgres;");
        if(query.exec())
        {
            qDebug(" => officer table created!");
        }
        else
        {
            qDebug() << " => error: " << db_object.lastError();
        }
    }
    else
    {
        qDebug() << "card table already exist!";
    }
}

QSqlQuery Database::get_device_port_configuration()
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    QString queryStr = QString("SELECT to_json(port_config) FROM port_config");
    query.prepare(queryStr);
    if(!query.exec())
        qDebug() << query.lastError();
    query.next();
    return query;
}

bool Database::control_card_id(QString officer_id)
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    QString queryStr = QString("SELECT * FROM public.card where card_id = '%1'").arg(officer_id);
    query.prepare(queryStr);
    query.exec();
    query.next();
    if(query.value(0).toString().isEmpty() && query.value(1).toString().isEmpty())
    {
        return false;
    }
    else
    {
        my_stc.id = query.value(0).toInt();
        my_stc.card_id = query.value(1).toString();
        my_stc.name = query.value(2).toString();
        return true;
    }
}

QSqlQuery Database::get_fuel_amount(int tank_id)
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    QString query_string = QString("SELECT tank_size FROM public.fuel_tanks WHERE tank_id = %1").arg(tank_id);
    query.prepare(query_string);
    if(!query.exec())
        qDebug() << query.lastError();
    query.next();
    return query;
}

QSqlQuery Database::get_fuel_price()
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    QString query_string = QString("SELECT price FROM public.fuel_prices WHERE id = %1").arg(1);
    query.prepare(query_string);
    if(!query.exec())
        qDebug() << query.lastError();
    query.next();
    return query;
}

void Database::update_fuel_amount(QMap<QString, QVariant> updateMap)
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    QString query_string = QString("UPDATE public.fuel_tanks SET tank_size = tank_size - %1 WHERE tank_id = %2").arg(updateMap.value("fuel").toDouble()).arg(updateMap.value("tank_id").toInt());
    query.prepare(query_string);
    if(!query.exec())
        qDebug() << query.lastError();
    return;
}

QSqlQuery Database::get_fuel_price_all()
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    query.prepare("SELECT price FROM public.fuel_prices");
    if(!query.exec())
        qDebug() << query.lastError();
    query.next();
    return query;
}

QSqlQuery Database::get_fuel_amount_degree_all()
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    query.prepare("SELECT tank_id, tank_size, degree FROM public.fuel_tanks order by tank_id");
    if(!query.exec())
        qDebug() << query.lastError();
    query.next();
    return query;
}

void Database::insert_sale(QMap<QString, QVariant> updateMap)
{
    if(!db_object.isOpen())
        db_object.open();
    QSqlQuery query(db_object);
    qDebug() << updateMap.value("officerID").toString();
    qDebug() << updateMap.value("fuel_amount").toDouble();
    qDebug() << updateMap.value("fee").toDouble();
    qDebug() << updateMap.value("tank_id").toInt();

    query.prepare("INSERT INTO den(officerID, fuel_amount) VALUES (?,?);");
    query.addBindValue(updateMap.value("officerID").toString());
    query.addBindValue(updateMap.value("fuel_amount").toDouble());
//    query.addBindValue(updateMap.value("fee").toDouble());
//    query.addBindValue(updateMap.value("tank_id").toInt());

    if (!query.exec())
        qDebug() << query.lastError().text();
}
QSqlQueryModel* Database::get_table()
{
    if (!db_object.isOpen()) {
        db_object.open();
    }
    QString queryString = QString("SELECT * FROM public.sales");
    QSqlQueryModel *model  = new QSqlQueryModel();
    model->setQuery(queryString);
    return model;
}
