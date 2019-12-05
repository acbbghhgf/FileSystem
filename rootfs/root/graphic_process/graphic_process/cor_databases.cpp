#include "cor_databases.h"

CorDataBases::CorDataBases()
{
}

CorDataBases::~CorDataBases()
{
}

int CorDataBases::open_databases()
{
    qDebug() << "qsqldatabase::drivers = " << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("testDB.db");
    db.setUserName("root");
    db.setPassword("123456");
    qDebug() << "qsqldatabase set success";
    return 0;
}

int CorDataBases::create_databases(QString table_name)
{
    bool isOk = db.open();
        if(!isOk){
            qDebug()<<"error info :"<<db.lastError();
        }
        else{
            QSqlQuery query;
            // create table test
            QString creatTableStr = QString("CREATE TABLE %1\
                    ( \
                       field_id int, \
                        x1 int, \
                        y1 int, \
                        x2 int, \
                        y2 int, \
                        yz int, \
                        black_threshold_1 int, \
                        white_threshold_1 int, \
                        second_threshold_1 int, \
                        black_threshold_2 int, \
                        white_threshold_2 int, \
                        second_threshold_2 int, \
                        black_threshold_4 int, \
                        white_threshold_4 int, \
                        second_threshold_4 int, \
                        black_threshold_8 int, \
                        white_threshold_8 int, \
                        second_threshold_8 int, \
                        judge_1 int, \
                        judge_2 int, \
                        judge_4 int, \
                        judge_8 int);").arg(table_name);
            query.prepare(creatTableStr);
            if(!query.exec()){
                qDebug()<<"query error :"<<query.lastError();
                query.finish();
            }
            else{
                qDebug()<<"creat table success!";
                query.finish();
            }
        }
        return 0;
}
