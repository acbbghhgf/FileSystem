#ifndef COR_DATABASES_H
#define COR_DATABASES_H

#include <QtSql>
#include <QDebug>


class CorDataBases
{
public:
    CorDataBases();
    ~CorDataBases();
    int open_databases();
    int create_databases(QString table_name);


private:


private:
    //use database
    QSqlDatabase db;


};

#endif // COR_DATABASES_H
