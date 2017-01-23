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

    if (doesTableExist(prepareTableName("{prefix}_user_behavior")) ) // check if db table exists and remove it to prepare for current use
		deleteBehaviorDatabaseTable();

   if (createBehaviorDatabaseTable()) // create memory table for use
        qDebug() << "Judgement day has arrived";
}

bool Skynet::addUserToBehaviorTable(const QString &user)
{
	qDebug() << "Adding user " << user << " to behavior table";
	QSqlQuery query;
	QSqlDatabase skynetDB = QSqlDatabase::database();
	query.prepare("insert into cockatrice_user_behavior (name) values (:user_name)");
	query.bindValue(":user_name", user);

	if (query.exec())
		return true;
	return false;
}

bool Skynet::createBehaviorDatabaseTable()
{
	qDebug() << "Creating behavior database table";
	QSqlDatabase skynetDB = QSqlDatabase::database();
    QSqlQuery query(
        "CREATE TABLE IF NOT EXISTS `cockatrice_user_behavior` ("
		"`id` int(7) unsigned zerofill NOT NULL auto_increment,"
		"`name` varchar(35),"
        "`kicks` int(1) DEFAULT 0,"
		"`badwords` int(1) DEFAULT 0,"
        "PRIMARY KEY(`id`)) ENGINE = MEMORY;");
	
	if (query.exec())
		return true;

    return false;
}

bool Skynet::deleteBehaviorDatabaseTable()
{
	qDebug() << "Removing behavior database table";
	QSqlDatabase skynetDB = QSqlDatabase::database();
	QSqlQuery *query = prepareQuery("drop table {prefix}_user_behavior");

	if (query->exec())
		return true;

    return false;
}

bool Skynet::doesTableExist(const QString &table)
{
	qDebug() << "Checking if table " << table << " exists";
	QSqlDatabase skynetDB = QSqlDatabase::database();
    if (skynetDB.tables().contains(table))
        return true;

    return false;
}

QSqlQuery * Skynet::prepareQuery(const QString &queryText)
{
	QString prefixedQueryText = queryText;
	QSqlDatabase skynetDB = QSqlDatabase::database();
	prefixedQueryText.replace("{prefix}", settingsCache->value("database/prefix").toString());
	QSqlQuery * query = new QSqlQuery(skynetDB);
	query->prepare(prefixedQueryText);
	return query;
}

QString Skynet::prepareTableName(const QString &table)
{
	QString results = table;
	results.replace("{prefix}", settingsCache->value("database/prefix").toString());
	return results;
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
