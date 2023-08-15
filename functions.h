#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <QSettings>
static QString Getvalue(QString tagName)
{
    QSettings settings("/databaseConfiguration.ini", QSettings::IniFormat);
    settings.beginGroup("database");
    QString return_value = settings.value(tagName).toString();
    settings.endGroup();
    return return_value;
}
#endif // FUNCTIONS_H

