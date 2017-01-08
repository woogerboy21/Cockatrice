#include "skynet.h"
#include "settingscache.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

Skynet::Skynet()
{

}

Skynet::~Skynet()
{

}

void Skynet::JudgementDay()
{
	initDatabase(); // create initial db connection

    if (dbReady())  // check if db table exists and remove it to prepare for current use
        deleteDatabaseTable();

    if (createDatabaseTable()) // create memory table for use
        qDebug() << "Judgement day has arrived";

	if (addUserToTable("testuser"))
		qDebug() << "Added testuser to table";

	if (deleteUserFromTable("testuser"))
		qDebug() << "Deleted testuser from table";
}

bool Skynet::createDatabaseTable()
{
	qDebug() << "Creating required behavior table";
	/*
    QSqlDatabase skynetDB = QSqlDatabase::addDatabase("QMYSQL");
    skynetDB.setHostName(settingsCache->value("database/hostname").toString());
    skynetDB.setDatabaseName(settingsCache->value("database/database").toString());
    skynetDB.setUserName(settingsCache->value("database/user").toString());
    skynetDB.setPassword(settingsCache->value("database/password").toString());
    skynetDB.open();
	*/
	QSqlDatabase skynetDB = QSqlDatabase::database();
    QSqlQuery query(
        "CREATE TABLE IF NOT EXISTS `cockatrice_user_behavior` ("
		"`name` varchar(35),"
        "`kicks` int(1) DEFAULT 0,"
		"`badwords` int(1) DEFAULT 0,"
        "PRIMARY KEY(`name`)) ENGINE = MEMORY;");

    if (query.exec())
        if (skynetDB.tables().contains("cockatrice_user_behavior"))
            return true;

    return false;
}

bool Skynet::deleteDatabaseTable()
{
	qDebug() << "Removing existing behavior table";
	/*
    QSqlDatabase skynetDB = QSqlDatabase::addDatabase("QMYSQL");
    skynetDB.setHostName(settingsCache->value("database/hostname").toString());
    skynetDB.setDatabaseName(settingsCache->value("database/database").toString());
    skynetDB.setUserName(settingsCache->value("database/user").toString());
    skynetDB.setPassword(settingsCache->value("database/password").toString());
    skynetDB.open();
	*/
	QSqlDatabase skynetDB = QSqlDatabase::database();
    QSqlQuery query("drop table cockatrice_user_behavior;");
    if (query.exec())
        if (!skynetDB.tables().contains("cockatrice_user_behavior"))
            return true;

    return false;
}

bool Skynet::dbReady()
{
	qDebug() << "Checking if DB contains required table(s)";
	/* 
	QSqlDatabase skynetDB = QSqlDatabase::addDatabase("QMYSQL");
    skynetDB.setHostName(settingsCache->value("database/hostname").toString());
    skynetDB.setDatabaseName(settingsCache->value("database/database").toString());
    skynetDB.setUserName(settingsCache->value("database/user").toString());
    skynetDB.setPassword(settingsCache->value("database/password").toString());
	skynetDB.open();
	*/
	QSqlDatabase skynetDB = QSqlDatabase::database();
	if (skynetDB.tables().contains("cockatrice_user_behavior"))
		return true;

	return false;
}

void Skynet::initDatabase()
{
	qDebug() << "Adding DB connection to DB connections";
	QSqlDatabase skynetDB = QSqlDatabase::addDatabase("QMYSQL");
	skynetDB.setHostName(settingsCache->value("database/hostname").toString());
	skynetDB.setDatabaseName(settingsCache->value("database/database").toString());
	skynetDB.setUserName(settingsCache->value("database/user").toString());
	skynetDB.setPassword(settingsCache->value("database/password").toString());
	skynetDB.open();
}

bool Skynet::addUserToTable(const QString &userName)
{
	if (!dbReady())
		return false;

	qDebug() << "Adding user to database behavior table: " << userName;
	QSqlDatabase skynetDB = QSqlDatabase::database();
	QSqlQuery query;
	query.prepare("insert into cockatrice_user_behavior (name) values (:username);");
	query.bindValue(":username", userName);
	
	if (query.exec())
		return true;

	return false;

}

bool Skynet::deleteUserFromTable(const QString &userName)
{
	if (!dbReady())
		return false;

	qDebug() << "Adding user to database behavior table: " << userName;
	QSqlDatabase skynetDB = QSqlDatabase::database();
	QSqlQuery query;
	query.prepare("delete from cockatrice_user_behavior where name = :username");
	query.bindValue(":username", userName);

	if (query.exec())
		return true;

	return false;
}