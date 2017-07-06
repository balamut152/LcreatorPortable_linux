#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>

//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("label.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Error", "Connect with database failed");
        return false;
    }

    QSqlQuery query;
    query.exec("create table if not exists  users(user_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
               "lname varchar(25),name varchar(18),pname varchar(18),otr int(2))");
    query.exec("create table if not exists addition(user_id INTEGER PRIMARY KEY NOT NULL,"
               "born_d varchar(10),stat varchar(50), period varchar(30),begin varchar(10),end varchar(10))");
    query.exec("create table if not exists special(user_id INTEGER PRIMATY KEY NOT NULL,"
               "cat INTEGER,prof INTEGER");


    return true;
}

//! [0]

#endif
